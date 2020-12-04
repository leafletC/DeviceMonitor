cmake_minimum_required (VERSION 3.0)
project (DeviceMonitor VERSION 1.0.00)
set (CMAKE_CXX_FLAGS "-Wall -g")

configure_file (
    "${PROJECT_SOURCE_DIR}/src/config.h.in"
    "${PROJECT_SOURCE_DIR}/src/config.h"
)

link_directories(
    ${PROJECT_SOURCE_DIR}/lib
    ${PROJECT_SOURCE_DIR}/src/Module/Database
    ${PROJECT_SOURCE_DIR}/src/Module/Network
    ${PROJECT_SOURCE_DIR}/src/Module/InfoCollector
    ${PROJECT_SOURCE_DIR}/src/Module/Utils
)
include_directories (
    ${PROJECT_SOURCE_DIR}/src/
    ${PROJECT_SOURCE_DIR}/src/Module/InfoCollector
    ${PROJECT_SOURCE_DIR}/src/Module/Database
    ${PROJECT_SOURCE_DIR}/src/Module/Network
    ${PROJECT_SOURCE_DIR}/src/Module/Network/MQTT
    ${PROJECT_SOURCE_DIR}/src/Module/Utils  
    ${PROJECT_SOURCE_DIR}/src/Module/Utils/INI 
)

add_subdirectory (${PROJECT_SOURCE_DIR}/src/Module/Database)
add_subdirectory (${PROJECT_SOURCE_DIR}/src/Module/Network)
add_subdirectory (${PROJECT_SOURCE_DIR}/src/Module/Utils)
add_subdirectory (${PROJECT_SOURCE_DIR}/src/Module/InfoCollector)

aux_source_directory (${PROJECT_SOURCE_DIR}/src SRC_LIST)

add_executable (${PROJECT_NAME} ${SRC_LIST})
target_link_libraries (${PROJECT_NAME} Utils Network Database InfoCollector)