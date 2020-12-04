link_directories (${PROJECT_SOURCE_DIR}/lib)
aux_source_directory (. MQTT_LIST)
add_library (Mqtt ${MQTT_LIST})
target_link_libraries(Mqtt libpaho-mqtt3a.so)