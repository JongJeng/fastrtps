// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*
 * @file StatelessReader.cpp
 *
 */

#include <fastdds/rtps/reader/StatelessReader.h>

#include <cassert>
#include <mutex>
#include <thread>

#include "reader_utils.hpp"
#include <fastdds/dds/log/Log.hpp>
#include <fastdds/rtps/builtin/BuiltinProtocols.h>
#include <fastdds/rtps/builtin/liveliness/WLP.h>
#include <fastdds/rtps/common/CacheChange.h>
#include <fastdds/rtps/history/ReaderHistory.h>
#include <fastdds/rtps/reader/ReaderListener.h>
#include <fastdds/rtps/writer/LivelinessManager.h>
#include <rtps/DataSharing/DataSharingListener.hpp>
#include <rtps/DataSharing/ReaderPool.hpp>
#include <rtps/participant/RTPSParticipantImpl.h>
#include <rtps/RTPSDomainImpl.hpp>

#define IDSTRING "(ID:" << std::this_thread::get_id() << ") " <<

using namespace eprosima::fastrtps::rtps;

StatelessReader::~StatelessReader()
{
    logInfo(RTPS_READER, "Removing reader " << m_guid);

    // Datasharing listener must be stopped to avoid processing notifications
    // while the reader is being destroyed
    if (is_datasharing_compatible_)
    {
        datasharing_listener_->stop();
    }
}

StatelessReader::StatelessReader(
        RTPSParticipantImpl* pimpl,
        const GUID_t& guid,
        const ReaderAttributes& att,
        ReaderHistory* hist,
        ReaderListener* listen)
    : RTPSReader(pimpl, guid, att, hist, listen)
    , matched_writers_(att.matched_writers_allocation)
{
}

StatelessReader::StatelessReader(
        RTPSParticipantImpl* pimpl,
        const GUID_t& guid,
        const ReaderAttributes& att,
        const std::shared_ptr<IPayloadPool>& payload_pool,
        ReaderHistory* hist,
        ReaderListener* listen)
    : RTPSReader(pimpl, guid, att, payload_pool, hist, listen)
    , matched_writers_(att.matched_writers_allocation)
{
}

StatelessReader::StatelessReader(
        RTPSParticipantImpl* pimpl,
        const GUID_t& guid,
        const ReaderAttributes& att,
        const std::shared_ptr<IPayloadPool>& payload_pool,
        const std::shared_ptr<IChangePool>& change_pool,
        ReaderHistory* hist,
        ReaderListener* listen)
    : RTPSReader(pimpl, guid, att, payload_pool, change_pool, hist, listen)
    , matched_writers_(att.matched_writers_allocation)
{
}

bool StatelessReader::matched_writer_add(
        const WriterProxyData& wdata)
{
    {
        std::unique_lock<RecursiveTimedMutex> guard(mp_mutex);
        for (const RemoteWriterInfo_t& writer : matched_writers_)
        {
            if (writer.guid == wdata.guid())
            {
                logWarning(RTPS_READER, "Attempting to add existing writer");
                if (nullptr != mp_listener)
                {
                    // call the listener without the lock taken
                    guard.unlock();
                    mp_listener->on_writer_discovery(this, WriterDiscoveryInfo::CHANGED_QOS_WRITER, wdata.guid(),
                            &wdata);
                }
                return false;
            }
        }

        bool is_same_process = RTPSDomainImpl::should_intraprocess_between(m_guid, wdata.guid());
        bool is_datasharing = is_datasharing_compatible_with(wdata);

        RemoteWriterInfo_t info;
        info.guid = wdata.guid();
        info.persistence_guid = wdata.persistence_guid();
        info.has_manual_topic_liveliness = (MANUAL_BY_TOPIC_LIVELINESS_QOS == wdata.m_qos.m_liveliness.kind);
        info.is_datasharing = is_datasharing;

        if (is_datasharing)
        {
            if (datasharing_listener_->add_datasharing_writer(wdata.guid(),
                    m_att.durabilityKind == VOLATILE,
                    mp_history->m_att.maximumReservedCaches))
            {
                logInfo(RTPS_READER, "Writer Proxy " << wdata.guid() << " added to " << this->m_guid.entityId
                                                     << " with data sharing");
            }
            else
            {
                logError(RTPS_READER, "Failed to add Writer Proxy " << wdata.guid()
                                                                    << " to " << this->m_guid.entityId
                                                                    << " with data sharing.");
                return false;
            }

        }

        if (matched_writers_.emplace_back(info) == nullptr)
        {
            logWarning(RTPS_READER, "No space to add writer " << wdata.guid() << " to reader " << m_guid);
            if (is_datasharing)
            {
                datasharing_listener_->remove_datasharing_writer(wdata.guid());
            }
            return false;
        }
        logInfo(RTPS_READER, "Writer " << wdata.guid() << " added to reader " << m_guid);

        add_persistence_guid(info.guid, info.persistence_guid);

        m_acceptMessagesFromUnkownWriters = false;

        // Intraprocess manages durability itself
        if (is_datasharing && !is_same_process && m_att.durabilityKind != VOLATILE)
        {
            // simulate a notification to force reading of transient changes
            // this has to be done after the writer is added to the matched_writers or the processing may fail
            datasharing_listener_->notify(false);
        }
    }

    if (liveliness_lease_duration_ < c_TimeInfinite)
    {
        auto wlp = mp_RTPSParticipant->wlp();
        if ( wlp != nullptr)
        {
            wlp->sub_liveliness_manager_->add_writer(
                wdata.guid(),
                liveliness_kind_,
                liveliness_lease_duration_);
        }
        else
        {
            logError(RTPS_LIVELINESS, "Finite liveliness lease duration but WLP not enabled");
        }
    }

    if (nullptr != mp_listener)
    {
        mp_listener->on_writer_discovery(this, WriterDiscoveryInfo::DISCOVERED_WRITER, wdata.guid(), &wdata);
    }

    return true;
}

bool StatelessReader::matched_writer_remove(
        const GUID_t& writer_guid,
        bool removed_by_lease)
{
    bool ret_val = false;

    {
        std::unique_lock<RecursiveTimedMutex> guard(mp_mutex);

        //Remove cachechanges belonging to the unmatched writer
        mp_history->writer_unmatched(writer_guid, get_last_notified(writer_guid));

        ResourceLimitedVector<RemoteWriterInfo_t>::iterator it;
        for (it = matched_writers_.begin(); it != matched_writers_.end(); ++it)
        {
            if (it->guid == writer_guid)
            {
                logInfo(RTPS_READER, "Writer " << writer_guid << " removed from " << m_guid);

                if (it->is_datasharing && datasharing_listener_->remove_datasharing_writer(writer_guid))
                {
                    logInfo(RTPS_READER, "Data sharing writer " << writer_guid << " removed from " << m_guid.entityId);
                    remove_changes_from(writer_guid, true);
                }

                remove_persistence_guid(it->guid, it->persistence_guid, removed_by_lease);
                matched_writers_.erase(it);
                if (nullptr != mp_listener)
                {
                    // call the listener without lock
                    guard.unlock();
                    mp_listener->on_writer_discovery(this, WriterDiscoveryInfo::REMOVED_WRITER, writer_guid, nullptr);
                }
                ret_val = true;
                break;
            }
        }
    }

    if (ret_val && liveliness_lease_duration_ < c_TimeInfinite)
    {
        auto wlp = mp_RTPSParticipant->wlp();
        if ( wlp != nullptr)
        {
            LivelinessData::WriterStatus writer_liveliness_status;
            wlp->sub_liveliness_manager_->remove_writer(
                writer_guid,
                liveliness_kind_,
                liveliness_lease_duration_,
                writer_liveliness_status);

            if (writer_liveliness_status == LivelinessData::WriterStatus::ALIVE)
            {
                wlp->update_liveliness_changed_status(writer_guid, this, -1, 0);
            }
            else if (writer_liveliness_status == LivelinessData::WriterStatus::NOT_ALIVE)
            {
                wlp->update_liveliness_changed_status(writer_guid, this, 0, -1);
            }
        }
        else
        {
            logError(RTPS_LIVELINESS,
                    "Finite liveliness lease duration but WLP not enabled, cannot remove writer");
        }
    }

    return ret_val;
}

bool StatelessReader::matched_writer_is_matched(
        const GUID_t& writer_guid)
{
    std::lock_guard<RecursiveTimedMutex> guard(mp_mutex);
    if (std::any_of(matched_writers_.begin(), matched_writers_.end(),
            [writer_guid](const RemoteWriterInfo_t& item)
            {
                return item.guid == writer_guid;
            }))
    {
        return true;
    }

    return false;
}

bool StatelessReader::change_received(
        CacheChange_t* change)
{
    // Only make the change visible if there is not another with a bigger sequence number.
    // TODO Revisar si no hay que incluirlo.
    if (!thereIsUpperRecordOf(change->writerGUID, change->sequenceNumber))
    {
        bool update_notified = true;

        decltype(matched_writers_)::iterator writer = matched_writers_.end();
        if (m_trustedWriterEntityId == change->writerGUID.entityId)
        {
            writer = std::find_if(matched_writers_.begin(), matched_writers_.end(),
                            [change](const RemoteWriterInfo_t& item)
                            {
                                return item.guid == change->writerGUID;
                            });
            bool is_matched = matched_writers_.end() != writer;
            update_notified = is_matched;
        }

        if (mp_history->received_change(change, 0))
        {
            auto payload_length = change->serializedPayload.length;

            Time_t::now(change->reader_info.receptionTimestamp);
            SequenceNumber_t previous_seq{ 0, 0 };
            if (update_notified)
            {
                previous_seq = update_last_notified(change->writerGUID, change->sequenceNumber);
            }
            ++total_unread_;

            on_data_notify(change->writerGUID, change->sourceTimestamp);

            if (getListener() != nullptr)
            {
                if (SequenceNumber_t{0, 0} != previous_seq)
                {
                    assert(previous_seq < change->sequenceNumber);
                    uint64_t tmp = (change->sequenceNumber - previous_seq).to64long() - 1;
                    int32_t lost_samples = tmp > static_cast<uint64_t>(std::numeric_limits<int32_t>::max()) ?
                            std::numeric_limits<int32_t>::max() : static_cast<int32_t>(tmp);
                    if (0 < lost_samples) // There are lost samples.
                    {
                        getListener()->on_sample_lost(this, lost_samples);
                    }
                }

                // WARNING! This method could destroy the change
                getListener()->onNewCacheChangeAdded(this, change);
            }

            new_notification_cv_.notify_all();

            // statistics callback
            on_subscribe_throughput(payload_length);

            return true;
        }
    }

    return false;
}

void StatelessReader::remove_changes_from(
        const GUID_t& writerGUID,
        bool is_payload_pool_lost)
{
    std::lock_guard<RecursiveTimedMutex> guard(mp_mutex);
    std::vector<CacheChange_t*> toremove;
    for (std::vector<CacheChange_t*>::iterator it = mp_history->changesBegin();
            it != mp_history->changesEnd(); ++it)
    {
        if ((*it)->writerGUID == writerGUID)
        {
            toremove.push_back((*it));
        }
    }

    for (std::vector<CacheChange_t*>::iterator it = toremove.begin();
            it != toremove.end(); ++it)
    {
        logInfo(RTPS_READER,
                "Removing change " << (*it)->sequenceNumber << " from " << (*it)->writerGUID);
        if (is_payload_pool_lost)
        {
            (*it)->serializedPayload.data = nullptr;
            (*it)->payload_owner(nullptr);
        }
        mp_history->remove_change(*it);
    }
}

bool StatelessReader::nextUntakenCache(
        CacheChange_t** change,
        WriterProxy** /*wpout*/)
{
    std::lock_guard<RecursiveTimedMutex> guard(mp_mutex);
    return mp_history->get_min_change(change);
}

bool StatelessReader::nextUnreadCache(
        CacheChange_t** change,
        WriterProxy** /*wpout*/)
{
    std::lock_guard<RecursiveTimedMutex> guard(mp_mutex);
    bool found = false;
    std::vector<CacheChange_t*>::iterator it = mp_history->changesBegin();
    while (it != mp_history->changesEnd())
    {
        if ((*it)->isRead)
        {
            ++it;
            continue;
        }

        found = true;
        break;
    }

    if (found)
    {
        *change = *it;
    }
    else
    {
        logInfo(RTPS_READER, "No Unread elements left");
    }

    return found;
}

bool StatelessReader::change_removed_by_history(
        CacheChange_t* ch,
        WriterProxy* /*prox*/)
{
    if (!ch->isRead)
    {
        if (0 < total_unread_)
        {
            --total_unread_;
        }
    }

    return true;
}

bool StatelessReader::begin_sample_access_nts(
        CacheChange_t* /*change*/,
        WriterProxy*& /*wp*/,
        bool& is_future_change)
{
    is_future_change = false;
    return true;
}

void StatelessReader::end_sample_access_nts(
        CacheChange_t* change,
        WriterProxy*& wp,
        bool mark_as_read)
{
    change_read_by_user(change, wp, mark_as_read);
}

void StatelessReader::change_read_by_user(
        CacheChange_t* change,
        WriterProxy* /*writer*/,
        bool mark_as_read)
{
    // Mark change as read
    if (mark_as_read && !change->isRead)
    {
        change->isRead = true;
        if (0 < total_unread_)
        {
            --total_unread_;
        }
    }

}

bool StatelessReader::processDataMsg(
        CacheChange_t* change)
{
    assert(change);

    std::unique_lock<RecursiveTimedMutex> lock(mp_mutex);

    if (acceptMsgFrom(change->writerGUID, change->kind))
    {
        // Always assert liveliness on scope exit
        auto assert_liveliness_lambda = [&lock, this, change](void*)
                {
                    lock.unlock(); // Avoid deadlock with LivelinessManager.
                    assert_writer_liveliness(change->writerGUID);
                };
        std::unique_ptr<void, decltype(assert_liveliness_lambda)> p{ this, assert_liveliness_lambda };

        logInfo(RTPS_MSG_IN, IDSTRING "Trying to add change " << change->sequenceNumber << " TO reader: " << m_guid);

        // Check rejection by history
        if (!thereIsUpperRecordOf(change->writerGUID, change->sequenceNumber))
        {
            bool will_never_be_accepted = false;
            if (!mp_history->can_change_be_added_nts(change->writerGUID, change->serializedPayload.length, 0,
                    will_never_be_accepted))
            {
                if (will_never_be_accepted)
                {
                    update_last_notified(change->writerGUID, change->sequenceNumber);
                }
                return false;
            }

            if (!fastdds::rtps::change_is_relevant_for_filter(*change, m_guid, data_filter_))
            {
                update_last_notified(change->writerGUID, change->sequenceNumber);
                // Change was filtered out, so there isn't anything else to do
                return true;
            }

            // Ask the pool for a cache change
            CacheChange_t* change_to_add = nullptr;
            if (!change_pool_->reserve_cache(change_to_add))
            {
                logWarning(RTPS_MSG_IN,
                        IDSTRING "Reached the maximum number of samples allowed by this reader's QoS. Rejecting change for reader: " <<
                        m_guid );
                return false;
            }

            // Copy metadata to reserved change
            change_to_add->copy_not_memcpy(change);

            // Ask payload pool to copy the payload
            IPayloadPool* payload_owner = change->payload_owner();

            bool is_datasharing = std::any_of(matched_writers_.begin(), matched_writers_.end(),
                            [&change](const RemoteWriterInfo_t& writer)
                            {
                                return (writer.guid == change->writerGUID) && (writer.is_datasharing);
                            });

            if (is_datasharing)
            {
                //We may receive the change from the listener (with owner a ReaderPool) or intraprocess (with owner a WriterPool)
                ReaderPool* datasharing_pool = dynamic_cast<ReaderPool*>(payload_owner);
                if (!datasharing_pool)
                {
                    datasharing_pool = datasharing_listener_->get_pool_for_writer(change->writerGUID).get();
                }
                if (!datasharing_pool)
                {
                    logWarning(RTPS_MSG_IN, IDSTRING "Problem copying DataSharing CacheChange from writer "
                            << change->writerGUID);
                    change_pool_->release_cache(change_to_add);
                    return false;
                }

                datasharing_pool->get_payload(change->serializedPayload, payload_owner, *change_to_add);
            }
            else if (payload_pool_->get_payload(change->serializedPayload, payload_owner, *change_to_add))
            {
                change->payload_owner(payload_owner);
            }
            else
            {
                logWarning(RTPS_MSG_IN, IDSTRING "Problem copying CacheChange, received data is: "
                        << change->serializedPayload.length << " bytes and max size in reader "
                        << m_guid << " is "
                        << (fixed_payload_size_ > 0 ? fixed_payload_size_ : (std::numeric_limits<uint32_t>::max)()));
                change_pool_->release_cache(change_to_add);
                return false;
            }

            // Perform reception of cache change
            if (!change_received(change_to_add))
            {
                logInfo(RTPS_MSG_IN, IDSTRING "MessageReceiver not add change " << change_to_add->sequenceNumber);
                change_to_add->payload_owner()->release_payload(*change_to_add);
                change_pool_->release_cache(change_to_add);
                return false;
            }
        }
    }

    return true;
}

bool StatelessReader::processDataFragMsg(
        CacheChange_t* incomingChange,
        uint32_t sampleSize,
        uint32_t fragmentStartingNum,
        uint16_t fragmentsInSubmessage)
{
    assert(incomingChange);

    GUID_t writer_guid = incomingChange->writerGUID;

    std::unique_lock<RecursiveTimedMutex> lock(mp_mutex);
    for (RemoteWriterInfo_t& writer : matched_writers_)
    {
        if (writer.guid == writer_guid)
        {
            // Always assert liveliness on scope exit
            auto assert_liveliness_lambda = [&lock, this, &writer_guid](void*)
                    {
                        lock.unlock(); // Avoid deadlock with LivelinessManager.
                        assert_writer_liveliness(writer_guid);
                    };
            std::unique_ptr<void, decltype(assert_liveliness_lambda)> p{ this, assert_liveliness_lambda };

            // Datasharing communication will never send fragments
            assert(!writer.is_datasharing);

            // Check if CacheChange was received.
            if (!thereIsUpperRecordOf(writer_guid, incomingChange->sequenceNumber))
            {
                logInfo(RTPS_MSG_IN, IDSTRING "Trying to add fragment " << incomingChange->sequenceNumber.to64long() <<
                        " TO reader: " << m_guid);

                // Early return if we already know about a greater sequence number
                CacheChange_t* work_change = writer.fragmented_change;
                if (work_change != nullptr && work_change->sequenceNumber > incomingChange->sequenceNumber)
                {
                    return true;
                }

                bool will_never_be_accepted = false;
                if (!mp_history->can_change_be_added_nts(writer_guid, sampleSize, 0, will_never_be_accepted))
                {
                    if (will_never_be_accepted)
                    {
                        update_last_notified(writer_guid, incomingChange->sequenceNumber);
                    }
                    return false;
                }

                CacheChange_t* change_to_add = incomingChange;

                // Check if pending fragmented change should be dropped
                if (work_change != nullptr)
                {
                    if (work_change->sequenceNumber < change_to_add->sequenceNumber)
                    {
                        SequenceNumber_t updated_seq = work_change->sequenceNumber;
                        SequenceNumber_t previous_seq{ 0, 0 };
                        previous_seq = update_last_notified(writer_guid, updated_seq);

                        // Notify lost samples
                        auto listener = getListener();
                        if (listener != nullptr)
                        {
                            if (SequenceNumber_t{ 0, 0 } != previous_seq)
                            {
                                assert(previous_seq < updated_seq);
                                uint64_t tmp = (updated_seq - previous_seq).to64long();
                                int32_t lost_samples =
                                        tmp > static_cast<uint64_t>(std::numeric_limits<int32_t>::max()) ?
                                        std::numeric_limits<int32_t>::max() : static_cast<int32_t>(tmp);
                                assert (0 < lost_samples);
                                listener->on_sample_lost(this, lost_samples);
                            }
                        }

                        // Pending change should be dropped. Check if it can be reused
                        if (sampleSize <= work_change->serializedPayload.max_size)
                        {
                            // Sample fits inside pending change. Reuse it.
                            work_change->copy_not_memcpy(change_to_add);
                            work_change->serializedPayload.length = sampleSize;
                            work_change->instanceHandle.clear();
                            work_change->setFragmentSize(change_to_add->getFragmentSize(), true);
                        }
                        else
                        {
                            // Release change, and let it be reserved later
                            releaseCache(work_change);
                            work_change = nullptr;
                        }
                    }
                }

                // Check if a new change should be reserved
                if (work_change == nullptr)
                {
                    if (reserveCache(&work_change, sampleSize))
                    {
                        if (work_change->serializedPayload.max_size < sampleSize)
                        {
                            releaseCache(work_change);
                            work_change = nullptr;
                        }
                        else
                        {
                            work_change->copy_not_memcpy(change_to_add);
                            work_change->serializedPayload.length = sampleSize;
                            work_change->instanceHandle.clear();
                            work_change->setFragmentSize(change_to_add->getFragmentSize(), true);
                        }
                    }
                }

                // Process fragment and set change_completed if it is fully reassembled
                CacheChange_t* change_completed = nullptr;
                if (work_change != nullptr)
                {
                    // Set the instanceHandle only when fragment number 1 is received
                    if (!work_change->instanceHandle.isDefined() && fragmentStartingNum == 1)
                    {
                        work_change->instanceHandle = change_to_add->instanceHandle;
                    }

                    if (work_change->add_fragments(change_to_add->serializedPayload, fragmentStartingNum,
                            fragmentsInSubmessage))
                    {
                        change_completed = work_change;
                        work_change = nullptr;
                    }
                }

                writer.fragmented_change = work_change;

                // If the change was completed, process it.
                if (change_completed != nullptr)
                {
                    // Temporarilly assign the inline qos while evaluating the data filter
                    change_completed->inline_qos = incomingChange->inline_qos;
                    bool filtered_out = !fastdds::rtps::change_is_relevant_for_filter(*change_completed, m_guid,
                                    data_filter_);
                    change_completed->inline_qos = SerializedPayload_t();

                    if (filtered_out)
                    {
                        update_last_notified(change_completed->writerGUID, change_completed->sequenceNumber);
                        releaseCache(change_completed);
                    }
                    else if (!change_received(change_completed))
                    {
                        logInfo(RTPS_MSG_IN,
                                IDSTRING "MessageReceiver not add change " <<
                                change_completed->sequenceNumber.to64long());

                        // Release CacheChange_t.
                        releaseCache(change_completed);
                    }
                }
            }

            return true;
        }
    }

    logWarning(RTPS_MSG_IN, IDSTRING "Reader " << m_guid << " received DATA_FRAG from unknown writer" << writer_guid);
    return true;
}

bool StatelessReader::processHeartbeatMsg(
        const GUID_t& /*writerGUID*/,
        uint32_t /*hbCount*/,
        const SequenceNumber_t& /*firstSN*/,
        const SequenceNumber_t& /*lastSN*/,
        bool /*finalFlag*/,
        bool /*livelinessFlag*/)
{
    return true;
}

bool StatelessReader::processGapMsg(
        const GUID_t& /*writerGUID*/,
        const SequenceNumber_t& /*gapStart*/,
        const SequenceNumberSet_t& /*gapList*/)
{
    return true;
}

bool StatelessReader::acceptMsgFrom(
        const GUID_t& writerId,
        ChangeKind_t change_kind)
{
    if (change_kind == ChangeKind_t::ALIVE)
    {
        if (m_acceptMessagesFromUnkownWriters)
        {
            return true;
        }
        else if (writerId.entityId == m_trustedWriterEntityId)
        {
            return true;
        }
    }

    return std::any_of(matched_writers_.begin(), matched_writers_.end(),
                   [&writerId](const RemoteWriterInfo_t& writer)
                   {
                       return writer.guid == writerId;
                   });
}

bool StatelessReader::thereIsUpperRecordOf(
        const GUID_t& guid,
        const SequenceNumber_t& seq)
{
    return get_last_notified(guid) >= seq;
}

void StatelessReader::assert_writer_liveliness(
        const GUID_t& guid)
{
    if (liveliness_lease_duration_ < c_TimeInfinite)
    {
        auto wlp = mp_RTPSParticipant->wlp();
        if (wlp != nullptr)
        {
            wlp->sub_liveliness_manager_->assert_liveliness(
                guid,
                liveliness_kind_,
                liveliness_lease_duration_);
        }
        else
        {
            logError(RTPS_LIVELINESS, "Finite liveliness lease duration but WLP not enabled");
        }
    }
}

bool StatelessReader::writer_has_manual_liveliness(
        const GUID_t& guid)
{
    for (const RemoteWriterInfo_t& writer : matched_writers_)
    {
        if (writer.guid == guid)
        {
            return writer.has_manual_topic_liveliness;
        }
    }
    return false;
}
