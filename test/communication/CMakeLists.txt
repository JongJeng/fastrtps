# Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

find_package(Python3 COMPONENTS Interpreter)

###############################################################################
# Binaries
###############################################################################
set(COMMON_SOURCE ${PROJECT_SOURCE_DIR}/test/blackbox/types/HelloWorld.cxx
    ${PROJECT_SOURCE_DIR}/test/blackbox/types/HelloWorldPubSubTypes.cxx
    ${PROJECT_SOURCE_DIR}/test/blackbox/types/FixedSized.cxx
    ${PROJECT_SOURCE_DIR}/test/blackbox/types/FixedSizedPubSubTypes.cxx
    )

set(PUBLISHER_SOURCE ${COMMON_SOURCE}
    Publisher.cpp
    PublisherMain.cpp
    )
add_executable(SimpleCommunicationPublisher ${PUBLISHER_SOURCE})
target_compile_definitions(SimpleCommunicationPublisher PRIVATE
    BOOST_ASIO_STANDALONE
    ASIO_STANDALONE
    $<$<AND:$<NOT:$<BOOL:${WIN32}>>,$<STREQUAL:"${CMAKE_BUILD_TYPE}","Debug">>:__DEBUG>
    $<$<BOOL:${INTERNAL_DEBUG}>:__INTERNALDEBUG> # Internal debug activated.
    )
target_include_directories(SimpleCommunicationPublisher PRIVATE
    ${Asio_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/test/blackbox)
target_link_libraries(SimpleCommunicationPublisher fastrtps fastcdr foonathan_memory ${CMAKE_DL_LIBS})

set(SUBSCRIBER_SOURCE ${COMMON_SOURCE}
    Subscriber.cpp
    SubscriberMain.cpp
    )
add_executable(SimpleCommunicationSubscriber ${SUBSCRIBER_SOURCE})
target_compile_definitions(SimpleCommunicationSubscriber PRIVATE
    BOOST_ASIO_STANDALONE
    ASIO_STANDALONE
    $<$<AND:$<NOT:$<BOOL:${WIN32}>>,$<STREQUAL:"${CMAKE_BUILD_TYPE}","Debug">>:__DEBUG>
    $<$<BOOL:${INTERNAL_DEBUG}>:__INTERNALDEBUG> # Internal debug activated.
    )
target_include_directories(SimpleCommunicationSubscriber PRIVATE
    ${Asio_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/test/blackbox)
target_link_libraries(SimpleCommunicationSubscriber fastrtps fastcdr foonathan_memory ${CMAKE_DL_LIBS})

set(PUBSUB_SOURCE ${COMMON_SOURCE}
    Publisher.cpp
    Subscriber.cpp
    PubSubMain.cpp
    )

add_executable(SimpleCommunicationPubSub ${PUBSUB_SOURCE})
target_compile_definitions(SimpleCommunicationPubSub PRIVATE
    BOOST_ASIO_STANDALONE
    ASIO_STANDALONE
    $<$<AND:$<NOT:$<BOOL:${WIN32}>>,$<STREQUAL:"${CMAKE_BUILD_TYPE}","Debug">>:__DEBUG>
    $<$<BOOL:${INTERNAL_DEBUG}>:__INTERNALDEBUG> # Internal debug activated.
    )
target_include_directories(SimpleCommunicationPubSub PRIVATE
    ${Asio_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/test/blackbox)
target_link_libraries(SimpleCommunicationPubSub fastrtps fastcdr foonathan_memory ${CMAKE_DL_LIBS})

###############################################################################
# Necessary files
###############################################################################
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/simple_communication.py
    ${CMAKE_CURRENT_BINARY_DIR}/simple_communication.py COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/simple_mix_communication.py
    ${CMAKE_CURRENT_BINARY_DIR}/simple_mix_communication.py COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/two_publishers_communication.py
    ${CMAKE_CURRENT_BINARY_DIR}/two_publishers_communication.py COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/liveliness_assertion.py
    ${CMAKE_CURRENT_BINARY_DIR}/liveliness_assertion.py COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/automatic_liveliness_assertion.py
    ${CMAKE_CURRENT_BINARY_DIR}/automatic_liveliness_assertion.py COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/shm_communication_subscriber_dies_while_processing_message.py
    ${CMAKE_CURRENT_BINARY_DIR}/shm_communication_subscriber_dies_while_processing_message.py COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/simple_besteffort.xml
    ${CMAKE_CURRENT_BINARY_DIR}/simple_besteffort.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/simple_reliable.xml
    ${CMAKE_CURRENT_BINARY_DIR}/simple_reliable.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/simple_reliable_besteffort.xml
    ${CMAKE_CURRENT_BINARY_DIR}/simple_reliable_besteffort.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/simple_secure_besteffort_pub.xml
    ${CMAKE_CURRENT_BINARY_DIR}/simple_secure_besteffort_pub.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/simple_secure_besteffort_sub.xml
    ${CMAKE_CURRENT_BINARY_DIR}/simple_secure_besteffort_sub.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/liveliness_assertion.xml
    ${CMAKE_CURRENT_BINARY_DIR}/liveliness_assertion.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/liveliness_assertion.120.xml
    ${CMAKE_CURRENT_BINARY_DIR}/liveliness_assertion.120.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/liveliness_assertion.360.xml
    ${CMAKE_CURRENT_BINARY_DIR}/liveliness_assertion.360.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/multiple_subs_secure_crypto_communication.py
    ${CMAKE_CURRENT_BINARY_DIR}/multiple_subs_secure_crypto_communication.py COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/secure_msg_crypto_besteffort_pub.xml
    ${CMAKE_CURRENT_BINARY_DIR}/secure_msg_crypto_besteffort_pub.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/secure_msg_crypto_besteffort_sub.xml
    ${CMAKE_CURRENT_BINARY_DIR}/secure_msg_crypto_besteffort_sub.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/secure_msg_submsg_crypto_besteffort_pub.xml
    ${CMAKE_CURRENT_BINARY_DIR}/secure_msg_submsg_crypto_besteffort_pub.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/secure_msg_submsg_crypto_besteffort_sub.xml
    ${CMAKE_CURRENT_BINARY_DIR}/secure_msg_submsg_crypto_besteffort_sub.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/secure_submsg_crypto_besteffort_pub.xml
    ${CMAKE_CURRENT_BINARY_DIR}/secure_submsg_crypto_besteffort_pub.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/secure_submsg_crypto_besteffort_sub.xml
    ${CMAKE_CURRENT_BINARY_DIR}/secure_submsg_crypto_besteffort_sub.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/shm_communication_subscriber_dies_while_processing_message.xml
    ${CMAKE_CURRENT_BINARY_DIR}/shm_communication_subscriber_dies_while_processing_message.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/simple_secure_allow_unauthenticated_pub_profile.xml
    ${CMAKE_CURRENT_BINARY_DIR}/simple_secure_allow_unauthenticated_pub_profile.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/simple_allow_unauthenticated_sub_profile.xml
    ${CMAKE_CURRENT_BINARY_DIR}/simple_allow_unauthenticated_sub_profile.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/secure_ds_pubsub_secure_crypto_communication.py
    ${CMAKE_CURRENT_BINARY_DIR}/secure_ds_pubsub_secure_crypto_communication.py COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/multiple_secure_ds_pubsub_secure_crypto_communication.py
    ${CMAKE_CURRENT_BINARY_DIR}/multiple_secure_ds_pubsub_secure_crypto_communication.py COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/secure_ds_simple_secure_msg_crypto_pub.xml
    ${CMAKE_CURRENT_BINARY_DIR}/secure_ds_simple_secure_msg_crypto_pub.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/secure_ds_simple_secure_msg_crypto_sub.xml
    ${CMAKE_CURRENT_BINARY_DIR}/secure_ds_simple_secure_msg_crypto_sub.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/secure_ds_simple_secure_msg_crypto_ds_1_sub.xml
    ${CMAKE_CURRENT_BINARY_DIR}/secure_ds_simple_secure_msg_crypto_ds_1_sub.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/secure_ds_simple_secure_no_discovery_no_rtps_protection_pub.xml
    ${CMAKE_CURRENT_BINARY_DIR}/secure_ds_simple_secure_no_discovery_no_rtps_protection_pub.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/secure_ds_simple_secure_no_discovery_no_rtps_protection_sub.xml
    ${CMAKE_CURRENT_BINARY_DIR}/secure_ds_simple_secure_no_discovery_no_rtps_protection_sub.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/secure_ds_simple_secure_no_discovery_no_rtps_protection_ds_1_sub.xml
    ${CMAKE_CURRENT_BINARY_DIR}/secure_ds_simple_secure_no_discovery_no_rtps_protection_ds_1_sub.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/secure_ds_simple_secure_no_discovery_protection_pub.xml
    ${CMAKE_CURRENT_BINARY_DIR}/secure_ds_simple_secure_no_discovery_protection_pub.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/secure_ds_simple_secure_no_discovery_protection_sub.xml
    ${CMAKE_CURRENT_BINARY_DIR}/secure_ds_simple_secure_no_discovery_protection_sub.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/secure_ds_simple_secure_no_discovery_protection_ds_1_sub.xml
    ${CMAKE_CURRENT_BINARY_DIR}/secure_ds_simple_secure_no_discovery_protection_ds_1_sub.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/secure_ds_simple_secure_no_rtps_protection_pub.xml
    ${CMAKE_CURRENT_BINARY_DIR}/secure_ds_simple_secure_no_rtps_protection_pub.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/secure_ds_simple_secure_no_rtps_protection_sub.xml
    ${CMAKE_CURRENT_BINARY_DIR}/secure_ds_simple_secure_no_rtps_protection_sub.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/secure_ds_simple_secure_no_rtps_protection_ds_1_sub.xml
    ${CMAKE_CURRENT_BINARY_DIR}/secure_ds_simple_secure_no_rtps_protection_ds_1_sub.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/secure_simple_ds_server.xml
    ${CMAKE_CURRENT_BINARY_DIR}/secure_simple_ds_server.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/secure_simple_ds_server_idx_1.xml
    ${CMAKE_CURRENT_BINARY_DIR}/secure_simple_ds_server_idx_1.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/secure_simple_ds_server_no_discovery_no_rtps_protection.xml
    ${CMAKE_CURRENT_BINARY_DIR}/secure_simple_ds_server_no_discovery_no_rtps_protection.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/secure_simple_ds_server_no_discovery_no_rtps_protection_idx_1.xml
    ${CMAKE_CURRENT_BINARY_DIR}/secure_simple_ds_server_no_discovery_no_rtps_protection_idx_1.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/secure_simple_ds_server_no_discovery_protection.xml
    ${CMAKE_CURRENT_BINARY_DIR}/secure_simple_ds_server_no_discovery_protection.xml COPYONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/secure_simple_ds_server_no_discovery_protection_idx_1.xml
    ${CMAKE_CURRENT_BINARY_DIR}/secure_simple_ds_server_no_discovery_protection_idx_1.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/secure_simple_ds_server_no_rtps_protection.xml
    ${CMAKE_CURRENT_BINARY_DIR}/secure_simple_ds_server_no_rtps_protection.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/secure_simple_ds_server_no_rtps_protection_idx_1.xml
    ${CMAKE_CURRENT_BINARY_DIR}/secure_simple_ds_server_no_rtps_protection_idx_1.xml COPYONLY)

if(SECURITY)
    configure_file(${PROJECT_SOURCE_DIR}/test/certs/maincacert.pem
        ${CMAKE_CURRENT_BINARY_DIR}/maincacert.pem COPYONLY)
    configure_file(${PROJECT_SOURCE_DIR}/test/certs/mainpubcert.pem
        ${CMAKE_CURRENT_BINARY_DIR}/mainpubcert.pem COPYONLY)
    configure_file(${PROJECT_SOURCE_DIR}/test/certs/mainpubkey.pem
        ${CMAKE_CURRENT_BINARY_DIR}/mainpubkey.pem COPYONLY)
    configure_file(${PROJECT_SOURCE_DIR}/test/certs/mainsubcert.pem
        ${CMAKE_CURRENT_BINARY_DIR}/mainsubcert.pem COPYONLY)
    configure_file(${PROJECT_SOURCE_DIR}/test/certs/mainsubkey.pem
        ${CMAKE_CURRENT_BINARY_DIR}/mainsubkey.pem COPYONLY)
    configure_file(${PROJECT_SOURCE_DIR}/test/certs/governance_helloworld_all_enable.smime
        ${CMAKE_CURRENT_BINARY_DIR}/governance_helloworld_all_enable.smime COPYONLY)
    configure_file(${PROJECT_SOURCE_DIR}/test/certs/governance_disable_discovery_disable_rtps_helloworld_all_enable.smime
        ${CMAKE_CURRENT_BINARY_DIR}/governance_disable_discovery_disable_rtps_helloworld_all_enable.smime COPYONLY)
    configure_file(${PROJECT_SOURCE_DIR}/test/certs/governance_disable_discovery_helloworld_all_enable.smime
        ${CMAKE_CURRENT_BINARY_DIR}/governance_disable_discovery_helloworld_all_enable.smime COPYONLY)
    configure_file(${PROJECT_SOURCE_DIR}/test/certs/governance_disable_rtps_helloworld_all_enable.smime
        ${CMAKE_CURRENT_BINARY_DIR}/governance_disable_rtps_helloworld_all_enable.smime COPYONLY)
    configure_file(${PROJECT_SOURCE_DIR}/test/certs/permissions_helloworld.smime
        ${CMAKE_CURRENT_BINARY_DIR}/permissions_helloworld.smime COPYONLY)
    configure_file(${PROJECT_SOURCE_DIR}/test/certs/governance_allow_unauth_all_disabled_access_none.smime
        ${CMAKE_CURRENT_BINARY_DIR}/governance_allow_unauth_all_disabled_access_none.smime COPYONLY)
    configure_file(${PROJECT_SOURCE_DIR}/test/certs/permissions_helloworld_securehelloworld.smime
        ${CMAKE_CURRENT_BINARY_DIR}/permissions_helloworld_securehelloworld.smime COPYONLY)
endif()

if(Python3_Interpreter_FOUND)
    add_test(NAME SimpleCommunicationBestEffort
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/simple_communication.py)

    # Set test with label NoMemoryCheck
    set_property(TEST SimpleCommunicationBestEffort PROPERTY LABELS "NoMemoryCheck")

    set_property(TEST SimpleCommunicationBestEffort PROPERTY ENVIRONMENT
        "SIMPLE_COMMUNICATION_PUBLISHER_BIN=$<TARGET_FILE:SimpleCommunicationPublisher>")
    set_property(TEST SimpleCommunicationBestEffort APPEND PROPERTY ENVIRONMENT
        "SIMPLE_COMMUNICATION_SUBSCRIBER_BIN=$<TARGET_FILE:SimpleCommunicationSubscriber>")
    set_property(TEST SimpleCommunicationBestEffort APPEND PROPERTY ENVIRONMENT
        "XML_FILE=simple_besteffort.xml")
    if(WIN32)
        string(REPLACE ";" "\\;" WIN_PATH "$ENV{PATH}")
        set_property(TEST SimpleCommunicationBestEffort APPEND PROPERTY ENVIRONMENT
            "PATH=$<TARGET_FILE_DIR:${PROJECT_NAME}>\\;$<TARGET_FILE_DIR:fastcdr>\\;${WIN_PATH}")
    endif()

    add_test(NAME SimpleCommunicationReliable
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/simple_communication.py)

    # Set test with label NoMemoryCheck
    set_property(TEST SimpleCommunicationReliable PROPERTY LABELS "NoMemoryCheck")

    set_property(TEST SimpleCommunicationReliable PROPERTY ENVIRONMENT
        "SIMPLE_COMMUNICATION_PUBLISHER_BIN=$<TARGET_FILE:SimpleCommunicationPublisher>")
    set_property(TEST SimpleCommunicationReliable APPEND PROPERTY ENVIRONMENT
        "SIMPLE_COMMUNICATION_SUBSCRIBER_BIN=$<TARGET_FILE:SimpleCommunicationSubscriber>")
    set_property(TEST SimpleCommunicationReliable APPEND PROPERTY ENVIRONMENT
        "XML_FILE=simple_reliable.xml")
    if(WIN32)
        string(REPLACE ";" "\\;" WIN_PATH "$ENV{PATH}")
        set_property(TEST SimpleCommunicationReliable APPEND PROPERTY ENVIRONMENT
            "PATH=$<TARGET_FILE_DIR:${PROJECT_NAME}>\\;$<TARGET_FILE_DIR:fastcdr>\\;${WIN_PATH}")
    endif()

    add_test(NAME SimpleCommunicationReliableBestEffort
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/simple_communication.py)

    # Set test with label NoMemoryCheck
    set_property(TEST SimpleCommunicationReliableBestEffort PROPERTY LABELS "NoMemoryCheck")

    set_property(TEST SimpleCommunicationReliableBestEffort PROPERTY ENVIRONMENT
        "SIMPLE_COMMUNICATION_PUBLISHER_BIN=$<TARGET_FILE:SimpleCommunicationPublisher>")
    set_property(TEST SimpleCommunicationReliableBestEffort APPEND PROPERTY ENVIRONMENT
        "SIMPLE_COMMUNICATION_SUBSCRIBER_BIN=$<TARGET_FILE:SimpleCommunicationSubscriber>")
    set_property(TEST SimpleCommunicationReliableBestEffort APPEND PROPERTY ENVIRONMENT
        "XML_FILE=simple_reliable_besteffort.xml")
    if(WIN32)
        string(REPLACE ";" "\\;" WIN_PATH "$ENV{PATH}")
        set_property(TEST SimpleCommunicationReliableBestEffort APPEND PROPERTY ENVIRONMENT
            "PATH=$<TARGET_FILE_DIR:${PROJECT_NAME}>\\;$<TARGET_FILE_DIR:fastcdr>\\;${WIN_PATH}")
    endif()

    add_test(NAME SimpleMixCommunicationBestEffort
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/simple_mix_communication.py)

    # Set test with label NoMemoryCheck
    set_property(TEST SimpleMixCommunicationBestEffort PROPERTY LABELS "NoMemoryCheck")

    set_property(TEST SimpleMixCommunicationBestEffort PROPERTY ENVIRONMENT
        "SIMPLE_COMMUNICATION_PUBLISHER_BIN=$<TARGET_FILE:SimpleCommunicationPublisher>")
    set_property(TEST SimpleMixCommunicationBestEffort APPEND PROPERTY ENVIRONMENT
        "SIMPLE_COMMUNICATION_SUBSCRIBER_BIN=$<TARGET_FILE:SimpleCommunicationPubSub>")
    set_property(TEST SimpleMixCommunicationBestEffort APPEND PROPERTY ENVIRONMENT
        "XML_FILE=simple_besteffort.xml")
    if(WIN32)
        string(REPLACE ";" "\\;" WIN_PATH "$ENV{PATH}")
        set_property(TEST SimpleMixCommunicationBestEffort APPEND PROPERTY ENVIRONMENT
            "PATH=$<TARGET_FILE_DIR:${PROJECT_NAME}>\\;$<TARGET_FILE_DIR:fastcdr>\\;${WIN_PATH}")
    endif()

    add_test(NAME SimpleMixCommunicationReliable
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/simple_mix_communication.py)

    # Set test with label NoMemoryCheck
    set_property(TEST SimpleMixCommunicationReliable PROPERTY LABELS "NoMemoryCheck")

    set_property(TEST SimpleMixCommunicationReliable PROPERTY ENVIRONMENT
        "SIMPLE_COMMUNICATION_PUBLISHER_BIN=$<TARGET_FILE:SimpleCommunicationPublisher>")
    set_property(TEST SimpleMixCommunicationReliable APPEND PROPERTY ENVIRONMENT
        "SIMPLE_COMMUNICATION_SUBSCRIBER_BIN=$<TARGET_FILE:SimpleCommunicationPubSub>")
    set_property(TEST SimpleMixCommunicationReliable APPEND PROPERTY ENVIRONMENT
        "XML_FILE=simple_reliable.xml")
    if(WIN32)
        string(REPLACE ";" "\\;" WIN_PATH "$ENV{PATH}")
        set_property(TEST SimpleMixCommunicationReliable APPEND PROPERTY ENVIRONMENT
            "PATH=$<TARGET_FILE_DIR:${PROJECT_NAME}>\\;$<TARGET_FILE_DIR:fastcdr>\\;${WIN_PATH}")
    endif()

    if(SECURITY)
        add_test(NAME SimpleCommunicationSecureBestEffort
            COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/simple_communication.py)

        # Set test with label NoMemoryCheck
        set_property(TEST SimpleCommunicationSecureBestEffort PROPERTY LABELS "NoMemoryCheck")

        set_property(TEST SimpleCommunicationSecureBestEffort PROPERTY ENVIRONMENT
            "SIMPLE_COMMUNICATION_PUBLISHER_BIN=$<TARGET_FILE:SimpleCommunicationPublisher>")
        set_property(TEST SimpleCommunicationSecureBestEffort APPEND PROPERTY ENVIRONMENT
            "SIMPLE_COMMUNICATION_SUBSCRIBER_BIN=$<TARGET_FILE:SimpleCommunicationSubscriber>")
        set_property(TEST SimpleCommunicationSecureBestEffort APPEND PROPERTY ENVIRONMENT
            "XML_FILE_PUB=simple_secure_besteffort_pub.xml")
        set_property(TEST SimpleCommunicationSecureBestEffort APPEND PROPERTY ENVIRONMENT
            "XML_FILE_SUB=simple_secure_besteffort_sub.xml")
        set_property(TEST SimpleCommunicationSecureBestEffort APPEND PROPERTY ENVIRONMENT
            "EXTRA_PUB_ARG=--wait 2")
        if(WIN32)
            string(REPLACE ";" "\\;" WIN_PATH "$ENV{PATH}")
            set_property(TEST SimpleCommunicationSecureBestEffort APPEND PROPERTY ENVIRONMENT
                "PATH=$<TARGET_FILE_DIR:${PROJECT_NAME}>\\;$<TARGET_FILE_DIR:fastcdr>\\;${WIN_PATH}")
        endif()

        add_test(NAME SimpleCommunicationSecureMsgCryptoBestEffort
            COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/multiple_subs_secure_crypto_communication.py
            --pub $<TARGET_FILE:SimpleCommunicationPublisher>
            --xml-pub secure_msg_crypto_besteffort_pub.xml
            --sub $<TARGET_FILE:SimpleCommunicationSubscriber>
            --xml-sub secure_msg_crypto_besteffort_sub.xml
            --samples 10 --wait 2 --n-subs 5)

        # Set test with label NoMemoryCheck
        set_property(TEST SimpleCommunicationSecureMsgCryptoBestEffort PROPERTY LABELS "NoMemoryCheck")

        if(WIN32)
            string(REPLACE ";" "\\;" WIN_PATH "$ENV{PATH}")
            set_property(TEST SimpleCommunicationSecureMsgCryptoBestEffort APPEND PROPERTY ENVIRONMENT
                "PATH=$<TARGET_FILE_DIR:${PROJECT_NAME}>\\;$<TARGET_FILE_DIR:fastcdr>\\;${WIN_PATH}")
        endif()

        add_test(NAME SimpleCommunicationSecureMsgSubmsgCryptoBestEffort
            COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/multiple_subs_secure_crypto_communication.py
            --pub $<TARGET_FILE:SimpleCommunicationPublisher>
            --xml-pub secure_msg_submsg_crypto_besteffort_pub.xml
            --sub $<TARGET_FILE:SimpleCommunicationSubscriber>
            --xml-sub secure_msg_submsg_crypto_besteffort_sub.xml
            --samples 10 --wait 2 --n-subs 5)

        # Set test with label NoMemoryCheck
        set_property(TEST SimpleCommunicationSecureMsgSubmsgCryptoBestEffort PROPERTY LABELS "NoMemoryCheck")

        if(WIN32)
            string(REPLACE ";" "\\;" WIN_PATH "$ENV{PATH}")
            set_property(TEST SimpleCommunicationSecureMsgSubmsgCryptoBestEffort APPEND PROPERTY ENVIRONMENT
                "PATH=$<TARGET_FILE_DIR:${PROJECT_NAME}>\\;$<TARGET_FILE_DIR:fastcdr>\\;${WIN_PATH}")
        endif()

        add_test(NAME SimpleCommunicationSecureSubmsgCryptoBestEffort
            COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/multiple_subs_secure_crypto_communication.py
            --pub $<TARGET_FILE:SimpleCommunicationPublisher>
            --xml-pub secure_submsg_crypto_besteffort_pub.xml
            --sub $<TARGET_FILE:SimpleCommunicationSubscriber>
            --xml-sub secure_submsg_crypto_besteffort_sub.xml
            --samples 10 --wait 2 --n-subs 5)

        # Set test with label NoMemoryCheck
        set_property(TEST SimpleCommunicationSecureSubmsgCryptoBestEffort PROPERTY LABELS "NoMemoryCheck")

        if(WIN32)
            string(REPLACE ";" "\\;" WIN_PATH "$ENV{PATH}")
            set_property(TEST SimpleCommunicationSecureSubmsgCryptoBestEffort APPEND PROPERTY ENVIRONMENT
                "PATH=$<TARGET_FILE_DIR:${PROJECT_NAME}>\\;$<TARGET_FILE_DIR:fastcdr>\\;${WIN_PATH}")
        endif()

        add_test(NAME AllowUnauthenticatedSimplePubSecureNoRTPSProtectionSubNonSecure
            COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/multiple_subs_secure_crypto_communication.py
            --n-subs 1
            --pub $<TARGET_FILE:SimpleCommunicationPublisher>
            --xml-pub simple_secure_allow_unauthenticated_pub_profile.xml
            --sub $<TARGET_FILE:SimpleCommunicationSubscriber>
            --xml-sub simple_allow_unauthenticated_sub_profile.xml
            --samples 10)

        # Set test with label NoMemoryCheck
        set_property(TEST AllowUnauthenticatedSimplePubSecureNoRTPSProtectionSubNonSecure PROPERTY LABELS "NoMemoryCheck")

        if(WIN32)
            string(REPLACE ";" "\\;" WIN_PATH "$ENV{PATH}")
            set_property(TEST AllowUnauthenticatedSimplePubSecureNoRTPSProtectionSubNonSecure APPEND PROPERTY ENVIRONMENT
                "PATH=$<TARGET_FILE_DIR:${PROJECT_NAME}>\\;$<TARGET_FILE_DIR:fastcdr>\\;${WIN_PATH}")
        endif()

        add_test(NAME SecureDiscoverServerSimplePubSubSecureMsgCrypto
            COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/secure_ds_pubsub_secure_crypto_communication.py
            --pub $<TARGET_FILE:SimpleCommunicationPublisher>
            --xml-pub secure_ds_simple_secure_msg_crypto_pub.xml
            --sub $<TARGET_FILE:SimpleCommunicationSubscriber>
            --xml-sub secure_ds_simple_secure_msg_crypto_sub.xml
            --samples 10
            --ds $<TARGET_FILE:fast-discovery-server>
            --xml-ds secure_simple_ds_server.xml
            --server-id 0)

        # Set test with label NoMemoryCheck
        set_property(TEST SecureDiscoverServerSimplePubSubSecureMsgCrypto PROPERTY LABELS "NoMemoryCheck")

        if(WIN32)
            string(REPLACE ";" "\\;" WIN_PATH "$ENV{PATH}")
            set_property(TEST SecureDiscoverServerSimplePubSubSecureMsgCrypto APPEND PROPERTY ENVIRONMENT
                "PATH=$<TARGET_FILE_DIR:${PROJECT_NAME}>\\;$<TARGET_FILE_DIR:fastcdr>\\;${WIN_PATH}")
        endif()

        add_test(NAME SecureDiscoverServerSimplePubSubSecureNoDiscoveryNoRTPSProtectionMsgCrypto
            COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/secure_ds_pubsub_secure_crypto_communication.py
            --pub $<TARGET_FILE:SimpleCommunicationPublisher>
            --xml-pub secure_ds_simple_secure_no_discovery_no_rtps_protection_pub.xml
            --sub $<TARGET_FILE:SimpleCommunicationSubscriber>
            --xml-sub secure_ds_simple_secure_no_discovery_no_rtps_protection_sub.xml
            --samples 10
            --ds $<TARGET_FILE:fast-discovery-server>
            --xml-ds secure_simple_ds_server_no_discovery_no_rtps_protection.xml
            --server-id 0)

        # Set test with label NoMemoryCheck
        set_property(TEST SecureDiscoverServerSimplePubSubSecureNoDiscoveryNoRTPSProtectionMsgCrypto PROPERTY LABELS "NoMemoryCheck")

        if(WIN32)
            string(REPLACE ";" "\\;" WIN_PATH "$ENV{PATH}")
            set_property(TEST SecureDiscoverServerSimplePubSubSecureNoDiscoveryNoRTPSProtectionMsgCrypto APPEND PROPERTY ENVIRONMENT
                "PATH=$<TARGET_FILE_DIR:${PROJECT_NAME}>\\;$<TARGET_FILE_DIR:fastcdr>\\;${WIN_PATH}")
        endif()

        add_test(NAME SecureDiscoverServerSimplePubSubSecureNoDiscoveryProtectionMsgCrypto
            COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/secure_ds_pubsub_secure_crypto_communication.py
            --pub $<TARGET_FILE:SimpleCommunicationPublisher>
            --xml-pub secure_ds_simple_secure_no_discovery_protection_pub.xml
            --sub $<TARGET_FILE:SimpleCommunicationSubscriber>
            --xml-sub secure_ds_simple_secure_no_discovery_protection_sub.xml
            --samples 10
            --ds $<TARGET_FILE:fast-discovery-server>
            --xml-ds secure_simple_ds_server_no_discovery_protection.xml
            --server-id 0)

        # Set test with label NoMemoryCheck
        set_property(TEST SecureDiscoverServerSimplePubSubSecureNoDiscoveryProtectionMsgCrypto PROPERTY LABELS "NoMemoryCheck")

        if(WIN32)
            string(REPLACE ";" "\\;" WIN_PATH "$ENV{PATH}")
            set_property(TEST SecureDiscoverServerSimplePubSubSecureNoDiscoveryProtectionMsgCrypto APPEND PROPERTY ENVIRONMENT
                "PATH=$<TARGET_FILE_DIR:${PROJECT_NAME}>\\;$<TARGET_FILE_DIR:fastcdr>\\;${WIN_PATH}")
        endif()

        add_test(NAME SecureDiscoverServerSimplePubSubSecureNoRTPSProtectionMsgCrypto
            COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/secure_ds_pubsub_secure_crypto_communication.py
            --pub $<TARGET_FILE:SimpleCommunicationPublisher>
            --xml-pub secure_ds_simple_secure_no_rtps_protection_pub.xml
            --sub $<TARGET_FILE:SimpleCommunicationSubscriber>
            --xml-sub secure_ds_simple_secure_no_rtps_protection_sub.xml
            --samples 10
            --ds $<TARGET_FILE:fast-discovery-server>
            --xml-ds secure_simple_ds_server_no_rtps_protection.xml
            --server-id 0)

        # Set test with label NoMemoryCheck
        set_property(TEST SecureDiscoverServerSimplePubSubSecureNoRTPSProtectionMsgCrypto PROPERTY LABELS "NoMemoryCheck")

        if(WIN32)
            string(REPLACE ";" "\\;" WIN_PATH "$ENV{PATH}")
            set_property(TEST SecureDiscoverServerSimplePubSubSecureNoRTPSProtectionMsgCrypto APPEND PROPERTY ENVIRONMENT
                "PATH=$<TARGET_FILE_DIR:${PROJECT_NAME}>\\;$<TARGET_FILE_DIR:fastcdr>\\;${WIN_PATH}")
        endif()

        add_test(NAME TwoSecureDiscoverServersSimplePubSubSecureMsgCrypto
            COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/multiple_secure_ds_pubsub_secure_crypto_communication.py
            --pub $<TARGET_FILE:SimpleCommunicationPublisher>
            --xml-pub secure_ds_simple_secure_msg_crypto_pub.xml
            --sub $<TARGET_FILE:SimpleCommunicationSubscriber>
            --xml-sub secure_ds_simple_secure_msg_crypto_ds_1_sub.xml
            --samples 10
            --servers $<TARGET_FILE:fast-discovery-server> $<TARGET_FILE:fast-discovery-server>
            --xml-servers secure_simple_ds_server.xml secure_simple_ds_server_idx_1.xml)

        # Set test with label NoMemoryCheck
        set_property(TEST TwoSecureDiscoverServersSimplePubSubSecureMsgCrypto PROPERTY LABELS "NoMemoryCheck")

        if(WIN32)
            string(REPLACE ";" "\\;" WIN_PATH "$ENV{PATH}")
            set_property(TEST TwoSecureDiscoverServersSimplePubSubSecureMsgCrypto APPEND PROPERTY ENVIRONMENT
                "PATH=$<TARGET_FILE_DIR:${PROJECT_NAME}>\\;$<TARGET_FILE_DIR:fastcdr>\\;${WIN_PATH}")
        endif()

        add_test(NAME TwoSecureDiscoverServersSimplePubSubSecureNoDiscoveryNoRTPSProtectionMsgCrypto
            COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/multiple_secure_ds_pubsub_secure_crypto_communication.py
            --pub $<TARGET_FILE:SimpleCommunicationPublisher>
            --xml-pub secure_ds_simple_secure_no_discovery_no_rtps_protection_pub.xml
            --sub $<TARGET_FILE:SimpleCommunicationSubscriber>
            --xml-sub secure_ds_simple_secure_no_discovery_no_rtps_protection_ds_1_sub.xml
            --samples 10
            --servers $<TARGET_FILE:fast-discovery-server> $<TARGET_FILE:fast-discovery-server>
            --xml-servers secure_simple_ds_server_no_discovery_no_rtps_protection.xml secure_simple_ds_server_no_discovery_no_rtps_protection_idx_1.xml)

        # Set test with label NoMemoryCheck
        set_property(TEST TwoSecureDiscoverServersSimplePubSubSecureNoDiscoveryNoRTPSProtectionMsgCrypto PROPERTY LABELS "NoMemoryCheck")

        if(WIN32)
            string(REPLACE ";" "\\;" WIN_PATH "$ENV{PATH}")
            set_property(TEST TwoSecureDiscoverServersSimplePubSubSecureNoDiscoveryNoRTPSProtectionMsgCrypto APPEND PROPERTY ENVIRONMENT
                "PATH=$<TARGET_FILE_DIR:${PROJECT_NAME}>\\;$<TARGET_FILE_DIR:fastcdr>\\;${WIN_PATH}")
        endif()

        add_test(NAME TwoSecureDiscoverServersSimplePubSubSecureNoDiscoveryProtectionMsgCrypto
            COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/multiple_secure_ds_pubsub_secure_crypto_communication.py
            --pub $<TARGET_FILE:SimpleCommunicationPublisher>
            --xml-pub secure_ds_simple_secure_no_discovery_protection_pub.xml
            --sub $<TARGET_FILE:SimpleCommunicationSubscriber>
            --xml-sub secure_ds_simple_secure_no_discovery_protection_ds_1_sub.xml
            --samples 10
            --servers $<TARGET_FILE:fast-discovery-server> $<TARGET_FILE:fast-discovery-server>
            --xml-servers secure_simple_ds_server_no_discovery_protection.xml secure_simple_ds_server_no_discovery_protection_idx_1.xml)

        # Set test with label NoMemoryCheck
        set_property(TEST TwoSecureDiscoverServersSimplePubSubSecureNoDiscoveryProtectionMsgCrypto PROPERTY LABELS "NoMemoryCheck")

        if(WIN32)
            string(REPLACE ";" "\\;" WIN_PATH "$ENV{PATH}")
            set_property(TEST TwoSecureDiscoverServersSimplePubSubSecureNoDiscoveryProtectionMsgCrypto APPEND PROPERTY ENVIRONMENT
                "PATH=$<TARGET_FILE_DIR:${PROJECT_NAME}>\\;$<TARGET_FILE_DIR:fastcdr>\\;${WIN_PATH}")
        endif()

        add_test(NAME TwoSecureDiscoverServersSimplePubSubSecureNoRTPSProtectionMsgCrypto
            COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/multiple_secure_ds_pubsub_secure_crypto_communication.py
            --pub $<TARGET_FILE:SimpleCommunicationPublisher>
            --xml-pub secure_ds_simple_secure_no_rtps_protection_pub.xml
            --sub $<TARGET_FILE:SimpleCommunicationSubscriber>
            --xml-sub secure_ds_simple_secure_no_rtps_protection_ds_1_sub.xml
            --samples 10
            --servers $<TARGET_FILE:fast-discovery-server> $<TARGET_FILE:fast-discovery-server>
            --xml-servers secure_simple_ds_server_no_rtps_protection.xml secure_simple_ds_server_no_rtps_protection_idx_1.xml)

        # Set test with label NoMemoryCheck
        set_property(TEST TwoSecureDiscoverServersSimplePubSubSecureNoRTPSProtectionMsgCrypto PROPERTY LABELS "NoMemoryCheck")

        if(WIN32)
            string(REPLACE ";" "\\;" WIN_PATH "$ENV{PATH}")
            set_property(TEST TwoSecureDiscoverServersSimplePubSubSecureNoRTPSProtectionMsgCrypto APPEND PROPERTY ENVIRONMENT
                "PATH=$<TARGET_FILE_DIR:${PROJECT_NAME}>\\;$<TARGET_FILE_DIR:fastcdr>\\;${WIN_PATH}")
        endif()

    endif()

    add_test(NAME LivelinessAssertion
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/liveliness_assertion.py)

    # Set test with label NoMemoryCheck
    set_property(TEST LivelinessAssertion PROPERTY LABELS "NoMemoryCheck")

    set_property(TEST LivelinessAssertion PROPERTY ENVIRONMENT
        "SIMPLE_COMMUNICATION_PUBLISHER_BIN=$<TARGET_FILE:SimpleCommunicationPublisher>")
    set_property(TEST LivelinessAssertion APPEND PROPERTY ENVIRONMENT
        "SIMPLE_COMMUNICATION_SUBSCRIBER_BIN=$<TARGET_FILE:SimpleCommunicationSubscriber>")
    if(WIN32)
        string(REPLACE ";" "\\;" WIN_PATH "$ENV{PATH}")
        set_property(TEST LivelinessAssertion APPEND PROPERTY ENVIRONMENT
            "PATH=$<TARGET_FILE_DIR:${PROJECT_NAME}>\\;$<TARGET_FILE_DIR:fastcdr>\\;${WIN_PATH}")
    endif()
    add_test(NAME AutomaticLivelinessAssertion
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/automatic_liveliness_assertion.py)

    # Set test with label NoMemoryCheck
    set_property(TEST AutomaticLivelinessAssertion PROPERTY LABELS "NoMemoryCheck")

    set_property(TEST AutomaticLivelinessAssertion PROPERTY ENVIRONMENT
        "SIMPLE_COMMUNICATION_PUBLISHER_BIN=$<TARGET_FILE:SimpleCommunicationPublisher>")
    set_property(TEST AutomaticLivelinessAssertion APPEND PROPERTY ENVIRONMENT
        "SIMPLE_COMMUNICATION_SUBSCRIBER_BIN=$<TARGET_FILE:SimpleCommunicationSubscriber>")
    if(WIN32)
        string(REPLACE ";" "\\;" WIN_PATH "$ENV{PATH}")
        set_property(TEST AutomaticLivelinessAssertion APPEND PROPERTY ENVIRONMENT
            "PATH=$<TARGET_FILE_DIR:${PROJECT_NAME}>\\;$<TARGET_FILE_DIR:fastcdr>\\;${WIN_PATH}")
    endif()

    add_test(NAME TwoPublishersCommunicationBestEffort
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/two_publishers_communication.py)

    # Set test with label NoMemoryCheck
    set_property(TEST TwoPublishersCommunicationBestEffort PROPERTY LABELS "NoMemoryCheck")

    set_property(TEST TwoPublishersCommunicationBestEffort PROPERTY ENVIRONMENT
        "SIMPLE_COMMUNICATION_PUBLISHER_BIN=$<TARGET_FILE:SimpleCommunicationPublisher>")
    set_property(TEST TwoPublishersCommunicationBestEffort APPEND PROPERTY ENVIRONMENT
        "SIMPLE_COMMUNICATION_SUBSCRIBER_BIN=$<TARGET_FILE:SimpleCommunicationSubscriber>")
    set_property(TEST TwoPublishersCommunicationBestEffort APPEND PROPERTY ENVIRONMENT
        "XML_FILE=simple_besteffort.xml")
    if(WIN32)
        string(REPLACE ";" "\\;" WIN_PATH "$ENV{PATH}")
        set_property(TEST TwoPublishersCommunicationBestEffort APPEND PROPERTY ENVIRONMENT
            "PATH=$<TARGET_FILE_DIR:${PROJECT_NAME}>\\;$<TARGET_FILE_DIR:fastcdr>\\;${WIN_PATH}")
    endif()

    add_test(NAME TwoPublishersCommunicationReliable
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/two_publishers_communication.py)

    # Set test with label NoMemoryCheck
    set_property(TEST TwoPublishersCommunicationReliable PROPERTY LABELS "NoMemoryCheck")

    set_property(TEST TwoPublishersCommunicationReliable PROPERTY ENVIRONMENT
        "SIMPLE_COMMUNICATION_PUBLISHER_BIN=$<TARGET_FILE:SimpleCommunicationPublisher>")
    set_property(TEST TwoPublishersCommunicationReliable APPEND PROPERTY ENVIRONMENT
        "SIMPLE_COMMUNICATION_SUBSCRIBER_BIN=$<TARGET_FILE:SimpleCommunicationSubscriber>")
    set_property(TEST TwoPublishersCommunicationReliable APPEND PROPERTY ENVIRONMENT
        "XML_FILE=simple_reliable.xml")
    if(WIN32)
        string(REPLACE ";" "\\;" WIN_PATH "$ENV{PATH}")
        set_property(TEST TwoPublishersCommunicationReliable APPEND PROPERTY ENVIRONMENT
            "PATH=$<TARGET_FILE_DIR:${PROJECT_NAME}>\\;$<TARGET_FILE_DIR:fastcdr>\\;${WIN_PATH}")
    endif()

    add_test(NAME SimpleCommunicationBestEffortFixed
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/simple_communication.py)

    # Set test with label NoMemoryCheck
    set_property(TEST SimpleCommunicationBestEffortFixed PROPERTY LABELS "NoMemoryCheck")

    set_property(TEST SimpleCommunicationBestEffortFixed PROPERTY ENVIRONMENT
        "SIMPLE_COMMUNICATION_PUBLISHER_BIN=$<TARGET_FILE:SimpleCommunicationPublisher>")
    set_property(TEST SimpleCommunicationBestEffortFixed APPEND PROPERTY ENVIRONMENT
        "SIMPLE_COMMUNICATION_SUBSCRIBER_BIN=$<TARGET_FILE:SimpleCommunicationSubscriber>")
    set_property(TEST SimpleCommunicationBestEffortFixed APPEND PROPERTY ENVIRONMENT
        "XML_FILE=simple_besteffort.xml")
    set_property(TEST SimpleCommunicationBestEffortFixed APPEND PROPERTY ENVIRONMENT
        "EXTRA_PUB_ARG=--fixed_type")
    set_property(TEST SimpleCommunicationBestEffortFixed APPEND PROPERTY ENVIRONMENT
        "EXTRA_SUB_ARG=--fixed_type")
    if(WIN32)
        string(REPLACE ";" "\\;" WIN_PATH "$ENV{PATH}")
        set_property(TEST SimpleCommunicationBestEffortFixed APPEND PROPERTY ENVIRONMENT
            "PATH=$<TARGET_FILE_DIR:${PROJECT_NAME}>\\;$<TARGET_FILE_DIR:fastcdr>\\;${WIN_PATH}")
    endif()

    add_test(NAME SHMCommunicationSubscriberDiesWhileProcessingMessage
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/shm_communication_subscriber_dies_while_processing_message.py)

    # Set test with label NoMemoryCheck
    set_property(TEST SHMCommunicationSubscriberDiesWhileProcessingMessage PROPERTY LABELS "NoMemoryCheck")

    set_property(TEST SHMCommunicationSubscriberDiesWhileProcessingMessage PROPERTY ENVIRONMENT
        "SIMPLE_COMMUNICATION_PUBLISHER_BIN=$<TARGET_FILE:SimpleCommunicationPublisher>")
    set_property(TEST SHMCommunicationSubscriberDiesWhileProcessingMessage APPEND PROPERTY ENVIRONMENT
        "SIMPLE_COMMUNICATION_SUBSCRIBER_BIN=$<TARGET_FILE:SimpleCommunicationSubscriber>")
    set_property(TEST SHMCommunicationSubscriberDiesWhileProcessingMessage APPEND PROPERTY ENVIRONMENT
        "XML_FILE=shm_communication_subscriber_dies_while_processing_message.xml")
    if(WIN32)
        string(REPLACE ";" "\\;" WIN_PATH "$ENV{PATH}")
        set_property(TEST SHMCommunicationSubscriberDiesWhileProcessingMessage APPEND PROPERTY ENVIRONMENT
            "PATH=$<TARGET_FILE_DIR:${PROJECT_NAME}>\\;$<TARGET_FILE_DIR:fastcdr>\\;${WIN_PATH}")
    endif()

endif()

# Add 'xfail' label to flaky tests
set(COMMUNICATION_XFAIL_LIST XFAIL_COMMUNICATION)

foreach(COMMUNICATION_XFAIL_TEST ${COMMUNICATION_XFAIL_LIST})
    add_xfail_label(${CMAKE_CURRENT_SOURCE_DIR}/${COMMUNICATION_XFAIL_TEST}.list)
endforeach()
