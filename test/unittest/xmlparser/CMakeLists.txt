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

if(WIN32)
    add_definitions(
        -D_WIN32_WINNT=0x0601
        -D_CRT_SECURE_NO_WARNINGS
        )
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/test_xml_duration.xml
    ${CMAKE_CURRENT_BINARY_DIR}/test_xml_duration.xml
    COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/test_xml_root_library_settings.xml
    ${CMAKE_CURRENT_BINARY_DIR}/test_xml_root_library_settings.xml
    COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/test_xml_profiles.xml
    ${CMAKE_CURRENT_BINARY_DIR}/test_xml_profiles.xml
    COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/test_xml_security_profiles.xml
    ${CMAKE_CURRENT_BINARY_DIR}/test_xml_security_profiles.xml
    COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/test_xml_profiles_rooted.xml
    ${CMAKE_CURRENT_BINARY_DIR}/test_xml_profiles_rooted.xml
    COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/log_stdouterr.xml
    ${CMAKE_CURRENT_BINARY_DIR}/log_stdouterr.xml
    COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/log_stdouterr_wrong_property_name.xml
    ${CMAKE_CURRENT_BINARY_DIR}/log_stdouterr_wrong_property_name.xml
    COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/log_stdouterr_wrong_property_value.xml
    ${CMAKE_CURRENT_BINARY_DIR}/log_stdouterr_wrong_property_value.xml
    COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/log_stdouterr_two_thresholds.xml
    ${CMAKE_CURRENT_BINARY_DIR}/log_stdouterr_two_thresholds.xml
    COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/log_def_file.xml
    ${CMAKE_CURRENT_BINARY_DIR}/log_def_file.xml
    COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/log_inactive.xml
    ${CMAKE_CURRENT_BINARY_DIR}/log_inactive.xml
    COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/log_node_file_append.xml
    ${CMAKE_CURRENT_BINARY_DIR}/log_node_file_append.xml
    COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tls_config.xml
    ${CMAKE_CURRENT_BINARY_DIR}/tls_config.xml
    COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/UDP_transport_descriptors_config.xml
    ${CMAKE_CURRENT_BINARY_DIR}/UDP_transport_descriptors_config.xml
    COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/SHM_transport_descriptors_config.xml
    ${CMAKE_CURRENT_BINARY_DIR}/SHM_transport_descriptors_config.xml
    COPYONLY)
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/regressions" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

###################################  XMLProfileParserTests  ####################################################
set(XMLPROFILEPARSER_SOURCE
    XMLProfileParserTests.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/rtps/xmlparser/XMLProfileManager.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/rtps/xmlparser/XMLParser.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/rtps/xmlparser/XMLDynamicParser.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/rtps/xmlparser/XMLElementParser.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/rtps/xmlparser/XMLParserCommon.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/fastdds/publisher/qos/WriterQos.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/fastdds/subscriber/qos/ReaderQos.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/rtps/flowcontrol/ThroughputControllerDescriptor.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/rtps/flowcontrol/FlowControllerConsts.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/utils/IPFinder.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/utils/IPLocator.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/utils/string_convert.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/AnnotationDescriptor.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicData.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicDataFactory.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicType.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicPubSubType.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicTypePtr.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicDataPtr.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicTypeBuilder.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicTypeBuilderPtr.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicTypeBuilderFactory.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicTypeMember.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/TypeDescriptor.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/MemberDescriptor.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/AnnotationParameterValue.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/TypeIdentifier.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/TypeIdentifierTypes.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/TypeObject.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/TypeObjectFactory.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/TypeObjectHashId.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/TypeNamesGenerator.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/TypesBase.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/BuiltinAnnotationsTypeObject.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/utils/md5.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/rtps/common/Time_t.cpp
    )

# External sources
if(TINYXML2_SOURCE_DIR)
    list(APPEND XMLPROFILEPARSER_SOURCE
        ${TINYXML2_SOURCE_DIR}/tinyxml2.cpp
        )
endif()

# Only include tinyxml2 headers manually if the associated config predates the use of CMake targets
if(TINYXML2_INCLUDE_DIR)
    include_directories(${TINYXML2_INCLUDE_DIR})
endif(TINYXML2_INCLUDE_DIR)

add_executable(XMLProfileParserTests ${XMLPROFILEPARSER_SOURCE})
target_compile_definitions(XMLProfileParserTests PRIVATE
    BOOST_ASIO_STANDALONE
    ASIO_STANDALONE
    $<$<AND:$<NOT:$<BOOL:${WIN32}>>,$<STREQUAL:"${CMAKE_BUILD_TYPE}","Debug">>:__DEBUG>
    $<$<BOOL:${INTERNAL_DEBUG}>:__INTERNALDEBUG> # Internal debug activated.
    $<$<BOOL:${MSVC}>:NOMINMAX> # avoid conflic with std::min & std::max in visual studio
    )
target_include_directories(XMLProfileParserTests PRIVATE
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/Log
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/SharedMemTransportDescriptor
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/TCPTransportDescriptor
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/TCPv4TransportDescriptor
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/TCPv6TransportDescriptor
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/UDPTransportDescriptor
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/UDPv4TransportDescriptor
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/UDPv6TransportDescriptor
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/RTPSParticipantAttributes
    ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
    ${PROJECT_SOURCE_DIR}/src/cpp
    ${Asio_INCLUDE_DIR}
    )

target_link_libraries(XMLProfileParserTests GTest::gmock
    $<$<BOOL:${WIN32}>:iphlpapi$<SEMICOLON>Shlwapi>
    $<$<BOOL:${WIN32}>:ws2_32>
    ${TINYXML2_LIBRARY}
    fastcdr
    )
add_gtest(XMLProfileParserTests SOURCES XMLProfileParserTests.cpp)
###################################  XMLProfileParserTests  ####################################################



######################################  XMLParserTests  ########################################################
set(XMLPARSER_SOURCE
    XMLParserTests.cpp
    XMLElementParserTests.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/rtps/xmlparser/XMLProfileManager.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/rtps/xmlparser/XMLParser.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/rtps/xmlparser/XMLDynamicParser.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/rtps/xmlparser/XMLElementParser.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/rtps/xmlparser/XMLParserCommon.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/fastdds/publisher/qos/WriterQos.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/fastdds/subscriber/qos/ReaderQos.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/rtps/flowcontrol/ThroughputControllerDescriptor.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/rtps/flowcontrol/FlowControllerConsts.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/fastdds/log/Log.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/fastdds/log/OStreamConsumer.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/fastdds/log/StdoutConsumer.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/fastdds/log/StdoutErrConsumer.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/fastdds/log/FileConsumer.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/utils/IPFinder.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/utils/IPLocator.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/utils/string_convert.cpp

    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/AnnotationDescriptor.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicData.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicDataFactory.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicType.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicPubSubType.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicTypePtr.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicDataPtr.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicTypeBuilder.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicTypeBuilderPtr.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicTypeBuilderFactory.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicTypeMember.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/TypeDescriptor.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/MemberDescriptor.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/AnnotationParameterValue.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/TypeIdentifier.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/TypeIdentifierTypes.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/TypeObject.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/TypeObjectFactory.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/TypeObjectHashId.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/TypeNamesGenerator.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/TypesBase.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/BuiltinAnnotationsTypeObject.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/utils/md5.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/rtps/common/Time_t.cpp
    )

# External sources
if(TINYXML2_SOURCE_DIR)
    list(APPEND XMLPARSER_SOURCE
        ${TINYXML2_SOURCE_DIR}/tinyxml2.cpp
        )
endif()

add_executable(XMLParserTests ${XMLPARSER_SOURCE})
target_compile_definitions(XMLParserTests PRIVATE
    BOOST_ASIO_STANDALONE
    ASIO_STANDALONE
    $<$<AND:$<NOT:$<BOOL:${WIN32}>>,$<STREQUAL:"${CMAKE_BUILD_TYPE}","Debug">>:__DEBUG>
    $<$<BOOL:${INTERNAL_DEBUG}>:__INTERNALDEBUG> # Internal debug activated.
    $<$<BOOL:${MSVC}>:NOMINMAX> # avoid conflic with std::min & std::max in visual studio
    )
target_include_directories(XMLParserTests PRIVATE
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/SharedMemTransportDescriptor
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/TCPTransportDescriptor
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/TCPv4TransportDescriptor
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/TCPv6TransportDescriptor
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/UDPTransportDescriptor
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/UDPv4TransportDescriptor
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/UDPv6TransportDescriptor
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/RTPSParticipantAttributes
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/include
    ${PROJECT_SOURCE_DIR}/src/cpp
    ${Asio_INCLUDE_DIR}
    )

target_link_libraries(XMLParserTests GTest::gtest
    $<$<BOOL:${WIN32}>:iphlpapi$<SEMICOLON>Shlwapi>
    $<$<BOOL:${WIN32}>:ws2_32>
    ${TINYXML2_LIBRARY}
    fastcdr
    )
add_gtest(XMLParserTests SOURCES XMLParserTests.cpp XMLElementParserTests.cpp)
######################################  XMLParserTests  ########################################################

#######################################  XMLTreeTests  #########################################################
set(XMLTREE_SOURCE
    XMLTreeTests.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/utils/string_convert.cpp
    )

add_executable(XMLTreeTests ${XMLTREE_SOURCE})
target_compile_definitions(XMLTreeTests PRIVATE
    $<$<AND:$<NOT:$<BOOL:${WIN32}>>,$<STREQUAL:"${CMAKE_BUILD_TYPE}","Debug">>:__DEBUG>
    $<$<BOOL:${INTERNAL_DEBUG}>:__INTERNALDEBUG> # Internal debug activated.
    )
target_include_directories(XMLTreeTests PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/include
    )

target_link_libraries(XMLTreeTests GTest::gtest
    $<$<BOOL:${WIN32}>:iphlpapi$<SEMICOLON>Shlwapi>
    $<$<BOOL:${WIN32}>:ws2_32>
    )
add_gtest(XMLTreeTests SOURCES XMLTreeTests.cpp)
#######################################  XMLTreeTests  #########################################################


###################################  XMLENDPOINTPARSERTESTS  ###################################################
set(XMLENDPOINTPARSERTESTS_SOURCE
    XMLEndpointParserTests.cpp

    # parser
    ${PROJECT_SOURCE_DIR}/src/cpp/rtps/xmlparser/XMLEndpointParser.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/rtps/xmlparser/XMLParserCommon.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/rtps/xmlparser/XMLElementParser.cpp

    # log
    ${PROJECT_SOURCE_DIR}/src/cpp/fastdds/log/Log.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/fastdds/log/OStreamConsumer.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/fastdds/log/StdoutConsumer.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/fastdds/log/StdoutErrConsumer.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/fastdds/log/FileConsumer.cpp

    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/AnnotationDescriptor.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicData.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicDataFactory.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicType.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicPubSubType.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicTypePtr.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicDataPtr.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicTypeBuilder.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicTypeBuilderPtr.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicTypeBuilderFactory.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicTypeMember.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/TypeDescriptor.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/MemberDescriptor.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/AnnotationParameterValue.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/TypeIdentifier.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/TypeIdentifierTypes.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/TypeObject.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/TypeObjectFactory.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/TypeObjectHashId.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/TypeNamesGenerator.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/TypesBase.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/BuiltinAnnotationsTypeObject.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/utils/md5.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/rtps/common/Time_t.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/rtps/flowcontrol/FlowControllerConsts.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/utils/string_convert.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/fastdds/publisher/qos/WriterQos.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/fastdds/subscriber/qos/ReaderQos.cpp

    # locators
    ${PROJECT_SOURCE_DIR}/src/cpp/utils/IPLocator.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/rtps/common/Time_t.cpp
    )

# External sources
if(TINYXML2_SOURCE_DIR)
    list(APPEND XMLENDPOINTPARSERTESTS_SOURCE
        ${TINYXML2_SOURCE_DIR}/tinyxml2.cpp
        )
endif()

add_executable(XMLEndpointParserTests ${XMLENDPOINTPARSERTESTS_SOURCE})
target_compile_definitions(XMLEndpointParserTests PRIVATE
    BOOST_ASIO_STANDALONE
    ASIO_STANDALONE
    )
target_include_directories(XMLEndpointParserTests PRIVATE
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/ReaderProxyData
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/SharedMemTransportDescriptor
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/TCPTransportDescriptor
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/TCPv4TransportDescriptor
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/TCPv6TransportDescriptor
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/UDPTransportDescriptor
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/UDPv4TransportDescriptor
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/UDPv6TransportDescriptor
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/XMLProfileManager
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/WriterProxyData
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/include
    ${PROJECT_SOURCE_DIR}/src/cpp
    ${Asio_INCLUDE_DIR}
    )

target_link_libraries(XMLEndpointParserTests GTest::gtest
    $<$<BOOL:${WIN32}>:iphlpapi$<SEMICOLON>Shlwapi>
    $<$<BOOL:${WIN32}>:ws2_32>
    ${TINYXML2_LIBRARY}
    fastcdr
    )

add_gtest(XMLEndpointParserTests SOURCES XMLEndpointParserTests.cpp)

######################################  XMLLoadFileTests  ######################################################
set(XMLLOADFILE_SOURCE
    XMLLoadFileTests.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/rtps/xmlparser/XMLProfileManager.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/rtps/xmlparser/XMLParser.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/rtps/xmlparser/XMLDynamicParser.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/rtps/xmlparser/XMLElementParser.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/rtps/xmlparser/XMLParserCommon.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/fastdds/publisher/qos/WriterQos.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/fastdds/subscriber/qos/ReaderQos.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/rtps/flowcontrol/ThroughputControllerDescriptor.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/rtps/flowcontrol/FlowControllerConsts.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/fastdds/log/Log.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/fastdds/log/OStreamConsumer.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/fastdds/log/StdoutConsumer.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/fastdds/log/StdoutErrConsumer.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/fastdds/log/FileConsumer.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/utils/IPFinder.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/utils/IPLocator.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/utils/string_convert.cpp

    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/AnnotationDescriptor.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicData.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicDataFactory.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicType.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicPubSubType.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicTypePtr.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicDataPtr.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicTypeBuilder.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicTypeBuilderPtr.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicTypeBuilderFactory.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/DynamicTypeMember.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/TypeDescriptor.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/MemberDescriptor.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/AnnotationParameterValue.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/TypeIdentifier.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/TypeIdentifierTypes.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/TypeObject.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/TypeObjectFactory.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/TypeObjectHashId.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/TypeNamesGenerator.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/TypesBase.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/dynamic-types/BuiltinAnnotationsTypeObject.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/utils/md5.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/utils/SystemInfo.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/rtps/common/Time_t.cpp
    )

# External sources
if(TINYXML2_SOURCE_DIR)
    list(APPEND XMLLOADFILE_SOURCE
        ${TINYXML2_SOURCE_DIR}/tinyxml2.cpp
        )
endif()

if(ANDROID)
    if (ANDROID_NATIVE_API_LEVEL LESS 24)
        list(APPEND XMLLOADFILE_SOURCE
            ${ANDROID_IFADDRS_SOURCE_DIR}/ifaddrs.c
            )
    endif()
endif()

add_executable(XMLLoadFileTests ${XMLLOADFILE_SOURCE})
target_compile_definitions(XMLLoadFileTests PRIVATE
    BOOST_ASIO_STANDALONE
    ASIO_STANDALONE
    $<$<BOOL:${MSVC}>:NOMINMAX> # avoid conflict with std::min & std::max in visual studio
    $<$<AND:$<NOT:$<BOOL:${WIN32}>>,$<STREQUAL:"${CMAKE_BUILD_TYPE}","Debug">>:__DEBUG>
    $<$<BOOL:${INTERNAL_DEBUG}>:__INTERNALDEBUG> # Internal debug activated.
    HAVE_LOG_NO_INFO=0 FASTDDS_ENFORCE_LOG_INFO # Need LOG_INFO in this test
    )
target_include_directories(XMLLoadFileTests PRIVATE
    ${PROJECT_SOURCE_DIR}/test/mock/dds/TypeObjectRegistry
    ${PROJECT_SOURCE_DIR}/test/mock/dds/TypeObjectUtils
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/RTPSDomainImpl # Dependency of fastdds::xtypes::type_representation
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/SharedMemTransportDescriptor
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/TCPTransportDescriptor
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/TCPv4TransportDescriptor
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/TCPv6TransportDescriptor
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/UDPTransportDescriptor
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/UDPv4TransportDescriptor
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/UDPv6TransportDescriptor
    ${PROJECT_SOURCE_DIR}/test/mock/rtps/RTPSParticipantAttributes
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/include
    ${PROJECT_SOURCE_DIR}/src/cpp
    ${Asio_INCLUDE_DIR}
    $<$<BOOL:${ANDROID}>:${ANDROID_IFADDRS_INCLUDE_DIR}>
    )

target_link_libraries(XMLLoadFileTests GTest::gtest
    $<$<BOOL:${WIN32}>:iphlpapi$<SEMICOLON>Shlwapi>
    $<$<BOOL:${WIN32}>:ws2_32>
    ${TINYXML2_LIBRARY}
    fastcdr
    )
if(QNX)
    target_link_libraries(XMLLoadFileTests socket)
endif()

add_gtest(XMLLoadFileTests SOURCES XMLLoadFileTests.cpp)

if(ANDROID)
    set_property(TARGET XMLEndpointParserTests PROPERTY CROSSCOMPILING_EMULATOR "adb;shell;cd;${CMAKE_CURRENT_BINARY_DIR};&&")
    set_property(TARGET XMLParserTests PROPERTY CROSSCOMPILING_EMULATOR "adb;shell;cd;${CMAKE_CURRENT_BINARY_DIR};&&")
    set_property(TARGET XMLProfileParserTests PROPERTY CROSSCOMPILING_EMULATOR "adb;shell;cd;${CMAKE_CURRENT_BINARY_DIR};&&")
    set_property(TARGET XMLTreeTests PROPERTY CROSSCOMPILING_EMULATOR "adb;shell;cd;${CMAKE_CURRENT_BINARY_DIR};&&")
    set_property(TARGET XMLLoadFileTests PROPERTY CROSSCOMPILING_EMULATOR "adb;shell;cd;${CMAKE_CURRENT_BINARY_DIR};&&")
endif()

