cmake_minimum_required(VERSION 3.7)
project(xnet)
include_directories(
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/xnet_app
        ${PROJECT_SOURCE_DIR}/xnet_tiny
        ${PROJECT_SOURCE_DIR}/../lib/xnet
        ${PROJECT_SOURCE_DIR}/../lib/npcap/include
        ${PROJECT_SOURCE_DIR}/../lib/npcap/include/pcap
)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/../lib/xnet/pcap_device.c ${PROJECT_SOURCE_DIR}/app.c)
add_subdirectory(${PROJECT_SOURCE_DIR}/xnet_tiny)
add_subdirectory(${PROJECT_SOURCE_DIR}/xnet_app)
target_link_libraries(${PROJECT_NAME} xnet_tiny xnet_app ${PROJECT_SOURCE_DIR}/../lib/npcap/Lib/x64/wpcap.lib)


