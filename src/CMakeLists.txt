cmake_minimum_required(VERSION 3.7)
project(xnet)

include_directories(
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/xnet_app
        ${PROJECT_SOURCE_DIR}/xnet_tiny
        ${PROJECT_SOURCE_DIR}/../lib/xnet
)

add_executable(${PROJECT_NAME} ../lib/xnet/pcap_device.c src/app.c)
add_subdirectory(./xnet_tiny)
add_subdirectory(./xnet_app)
target_link_libraries(${PROJECT_NAME} xnet_tiny xnet_app pcap)


