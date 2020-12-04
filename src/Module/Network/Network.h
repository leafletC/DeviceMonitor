#pragma once

#include "MQTTAsyncClient.h"
#include "MyModule.h"

class Network : public MyModule
{
public:
    enum Signal
    {
        received,
        setWill,
    };
    struct Option
    {
        MQTT::AsyncClient::Option mqtt;
    };
    Network();
    void setOptions(const Option &option);
    void run();
    void send(const string &value);
private:
    void *receive(void *arg);
    void *subscribe(void *arg);
    MQTT::AsyncClient mqtt;
    Option option;
};