cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)

include(findcoredeps)

option(CLI_NULLTUN "Build ovpncli variant with null tun" OFF)
option(CLI_KOVPN "Build ovpncli variant with kovpn kernel moduke support" OFF)
option(CLI_OVPNDCO "Build ovpncli variant with ovpn-dco kernel module support" OFF)

set(KOVPN_SOURCE "${CORE_DIR}/../kovpn" CACHE PATH "ovpn-dco kernel module source tree")
set(OVPN_DCO_SOURCE "${CORE_DIR}/../ovpn-dco" CACHE PATH "ovpn-dco kernel module source tree")

add_executable(ovpncli cli.cpp)

if (OPENVPN_PLUGGABLE_TRANSPORTS)
#find_library(OBFS4 obfs4client.a ./../..//openvpn/plugin)
find_library(CLOAK ck-ovpn-plugin.a "${CORE_DIR}/openvpn/plugin")
set(CLOAK_INCLUDE "${CORE_DIR}/openvpn/plugin")
#message(${OBFS4})
message(${CLOAK})
endif ()

add_core_dependencies(ovpncli)
#target_link_libraries(ovpncli ${OBFS4})
target_link_libraries(ovpncli ${CLOAK})
target_include_directories(ovpncli PRIVATE ${CLOAK_INCLUDE})

if (${CLI_NULLTUN})
    add_executable(ovpnclinull cli.cpp)
    add_core_dependencies(ovpnclinull)
    target_compile_definitions(ovpnclinull PRIVATE -DOPENVPN_FORCE_TUN_NULL)
endif ()

if (${CLI_KOVPN})
    add_executable(ovpnclikovpn cli.cpp)
    add_core_dependencies(ovpnclikovpn)

    target_compile_definitions(ovpnclikovpn PRIVATE -DENABLE_KOVPN)

    include(kovpn)
    add_kovpn_dependency(ovpnclikovpn)
    target_include_directories(ovpnclikovpn PRIVATE ${KOVPN_SOURCE})
endif()

if (${CLI_OVPNDCO})
    add_executable(ovpncliovpndco cli.cpp)
    add_core_dependencies(ovpncliovpndco)

    target_compile_definitions(ovpncliovpndco PRIVATE -DENABLE_OVPNDCO)

    find_package(PkgConfig)
    pkg_search_module(LIBNL REQUIRED libnl-genl-3.0)

    target_include_directories(ovpncliovpndco PRIVATE ${OVPN_DCO_SOURCE}/include/uapi ${LIBNL_INCLUDE_DIRS})
    target_link_libraries(ovpncliovpndco ${LIBNL_LIBRARIES})
endif()

if (WIN32)
    # for macos, agent-enabled client requires common and is built by scripts there
    add_executable(ovpncliagent cli.cpp)
    add_core_dependencies(ovpncliagent)
    add_json_library(ovpncliagent)
    target_compile_definitions(ovpncliagent PRIVATE -DOPENVPN_COMMAND_AGENT -DOVPNAGENT_DISABLE_PATH_CHECK)
endif ()
