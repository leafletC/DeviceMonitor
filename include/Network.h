#pragma once

#include "MQTTAsyncClient.h"

class Network 
{
public:
    Network();
    void send();
    void asyncSend();
private:
    MQTT::AsyncClient client;
};