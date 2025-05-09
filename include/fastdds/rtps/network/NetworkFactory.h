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

#ifndef _FASTDDS_RTPS_NETWORK_FACTORY_HPP
#define _FASTDDS_RTPS_NETWORK_FACTORY_HPP

#include <vector>
#include <memory>

#include <fastdds/rtps/common/Locator.h>
#include <fastdds/rtps/common/LocatorList.hpp>
#include <fastdds/rtps/common/LocatorSelector.hpp>
#include <fastdds/rtps/messages/MessageReceiver.h>
#include <fastdds/rtps/network/ReceiverResource.h>
#include <fastdds/rtps/network/SenderResource.h>
#include <fastdds/rtps/transport/TransportInterface.h>

namespace eprosima {
namespace fastrtps {
namespace rtps {

class RTPSParticipantAttributes;

/**
 * Provides the FastRTPS library with abstract resources, which
 * in turn manage the SEND and RECEIVE operations over some transport.
 * Once a transport is registered, it becomes invisible to the library
 * and is abstracted away for good.
 * @ingroup NETWORK_MODULE
 */
class NetworkFactory
{
public:

    NetworkFactory(
            const RTPSParticipantAttributes& PParam);

    /**
     * Allow registration of a transport statically, by specifying the transport type and
     * its associated descriptor type. This is particularly useful for user-defined transports.
     */
    template<class T, class D>
    void RegisterTransport(
            const D& descriptor)
    {
        std::unique_ptr<T> transport(new T(descriptor));
        if (transport->init())
        {
            mRegisteredTransports.emplace_back(std::move(transport));
        }
    }

    /**
     * Allow registration of a transport dynamically.
     *
     * @param descriptor Structure that defines all initial configuration for a given transport.
     * @param properties Optional policy to specify additional parameters for the created transport.
     */
    bool RegisterTransport(
            const fastdds::rtps::TransportDescriptorInterface* descriptor,
            const fastrtps::rtps::PropertyPolicy* properties = nullptr);

    /**
     * Walk over the list of transports, opening every possible channel that can send through
     * the given locator and returning a vector of Sender Resources associated with it.
     * @param locator Locator through which to send.
     */
    bool build_send_resources(
            fastdds::rtps::SendResourceList&,
            const Locator_t& locator);

    /**
     * Walk over the list of transports, opening every possible channel that we can listen to
     * from the given locator, and returns a vector of Receiver Resources for this goal.
     * @param local Locator from which to listen.
     * @param returned_resources_list List that will be filled with the created ReceiverResources.
     * @param receiver_max_message_size Max message size allowed by the message receiver.
     */
    bool BuildReceiverResources(
            Locator_t& local,
            std::vector<std::shared_ptr<ReceiverResource>>& returned_resources_list,
            uint32_t receiver_max_message_size);

    void NormalizeLocators(
            LocatorList_t& locators);

    /**
     * Transform a remote locator into a locator optimized for local communications.
     *
     * If the remote locator corresponds to one of the local interfaces, it is converted
     * to the corresponding local address.
     *
     * @param [in]  remote_locator Locator to be converted.
     * @param [out] result_locator Converted locator.
     *
     * @return false if the input locator is not supported/allowed by any of the registered transports,
     *         true otherwise.
     */
    bool transform_remote_locator(
            const Locator_t& remote_locator,
            Locator_t& result_locator) const;

    /**
     * Perform the locator selection algorithm.
     *
     * It basically consists of the following steps
     *   - selector.selection_start is called
     *   - the transport selection algorithm is called for each registered transport
     *
     * @param [in, out] selector Locator selector.
     */
    void select_locators(
            LocatorSelector& selector) const;

    bool is_local_locator(
            const Locator_t& locator) const;

    size_t numberOfRegisteredTransports() const;

    uint32_t get_max_message_size_between_transports() const
    {
        return maxMessageSizeBetweenTransports_;
    }

    uint32_t get_min_send_buffer_size()
    {
        return minSendBufferSize_;
    }

    /**
     * Fill ret_locators with the list of all possible locators in the local machine at the given
     * physical_port of the locator_kind.
     * Return if found any.
     * */
    bool generate_locators(
            uint16_t physical_port,
            int locator_kind,
            LocatorList_t& ret_locators);

    /**
     * For each transport, ask for their default output locators.
     * */
    void GetDefaultOutputLocators(
            LocatorList_t& defaultLocators);

    /**
     * Add locators to the metatraffic multicast list.
     * */
    bool getDefaultMetatrafficMulticastLocators(
            LocatorList_t& locators,
            uint32_t metatraffic_multicast_port) const;

    /**
     * Add locators to the metatraffic unicast list.
     * */
    bool getDefaultMetatrafficUnicastLocators(
            LocatorList_t& locators,
            uint32_t metatraffic_unicast_port) const;

    /**
     * Fill the locator with the metatraffic multicast configuration.
     * */
    bool fillMetatrafficMulticastLocator(
            Locator_t& locator,
            uint32_t metatraffic_multicast_port) const;

    /**
     * Fill the locator with the metatraffic unicast configuration.
     * */
    bool fillMetatrafficUnicastLocator(
            Locator_t& locator,
            uint32_t metatraffic_unicast_port) const;

    /**
     * Configure the locator with the initial peer configuration.
     * */
    bool configureInitialPeerLocator(
            uint32_t domain_id,
            Locator_t& locator,
            RTPSParticipantAttributes& m_att) const;

    /**
     * Add locators to the default unicast configuration.
     * */
    bool getDefaultUnicastLocators(
            uint32_t domain_id,
            LocatorList_t& locators,
            const RTPSParticipantAttributes& m_att) const;

    /**
     * Fill the locator with the default unicast configuration.
     * */
    bool fill_default_locator_port(
            uint32_t domain_id,
            Locator_t& locator,
            const RTPSParticipantAttributes& m_att,
            bool is_multicast) const;

    /**
     * Shutdown method to close the connections of the transports.
     */
    void Shutdown();

    /**
     * Re-scan network interfaces
     */
    void update_network_interfaces();

    /**
     * Remove the given participants from the send resource list
     *
     * @param send_resource_list List of send resources associated to the local participant.
     * @param remote_participant_locators List of locators associated to the remote participant.
     * @param participant_initial_peers List of locators of the initial peers of the local participant.
     */
    void remove_participant_associated_send_resources(
            fastdds::rtps::SendResourceList& send_resource_list,
            const LocatorList_t& remote_participant_locators,
            const LocatorList_t& participant_initial_peers) const;

private:

    std::vector<std::unique_ptr<fastdds::rtps::TransportInterface>> mRegisteredTransports;

    uint32_t maxMessageSizeBetweenTransports_;

    uint32_t minSendBufferSize_;

    // Whether unicast metatraffic on SHM transport should always be used
    bool enforce_shm_unicast_metatraffic_ = false;

    // Whether multicast metatraffic on SHM transport should always be used
    bool enforce_shm_multicast_metatraffic_ = false;

    /**
     * Calculate well-known ports.
     */
    uint16_t calculate_well_known_port(
            uint32_t domain_id,
            const RTPSParticipantAttributes& att,
            bool is_multicast) const;
};

} // namespace rtps
} // namespace fastrtps
} // namespace eprosima

#endif /* _FASTDDS_RTPS_NETWORK_FACTORY_HPP */
