cmake_minimum_required (VERSION 3.0)
project (DeviceMonitor)
set (CMAKE_CXX_FLAGS "-std=c++11 -Wall -g")
find_library ( 
  /usr/lib  
  /usr/local/lib 
  /usr/local/lib64
  ${PROJECT_SOURCE_DIR}/lib 
  NO_DEFAULT_PATH
)
include_directories (${PROJECT_SOURCE_DIR}/include)
aux_source_directory (${PROJECT_SOURCE_DIR}/src SRC_LIST)
add_executable (${PROJECT_NAME} ${SRC_LIST})
target_link_libraries (${PROJECT_NAME} -lpaho-mqtt3c -lpaho-mqtt3a)