#pragma once

#include "MQTTAsync.h"
#include "MyModule.h"
#include <string>
#include <functional>

using namespace std;

namespace MQTT {
    
class AsyncClient : public MyModule
{
public:
    struct Option {
        string brokerUrl;
        string user;
        string password;
        string pubTopic;
        string subTopic;
        string clientId;
        int32_t pubQos;
        int32_t subQos;
        bool retained;
        int32_t keepalive;
        bool cleansession;
        void operator=(const Option &option);
    };
    enum Signal
    {
        connected,
        connectFailure,
        disconnected,
        received,
        subscribed,
        subscribedFailure,
        setWill,
    };
    AsyncClient();
    AsyncClient(const Option &option);
    ~AsyncClient();
    void setOptions(const Option &option);
    void run();
    void disconnect();
    void publish(const string &topic, const string &payload, int qos);
    void subscribe(const string &topic, int qos);
    void autoConnect();
private:
    static void onConnect(void* context, MQTTAsync_successData* response);
    static void onDisconnect(void* context, MQTTAsync_successData* response);
    static void onConnectFailure(void* context, MQTTAsync_failureData* response);
    static void connlost(void *context, char *cause);
    static void onSubscribe(void* context, MQTTAsync_successData* response);
    static void onSubscribeFailure(void* context, MQTTAsync_failureData* response);
    static int msgarrvd(void *context, char *topicName, int topicLen, MQTTAsync_message *message);
    MQTTAsync client;
    volatile MQTTAsync_token deliveredToken;
    Option option;
};

};

