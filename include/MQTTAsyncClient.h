#pragma once

#include "MQTTAsync.h"
#include <string>

using namespace std;

namespace MQTT {
    
class AsyncClient 
{
public:
    AsyncClient(const string &url, const string &id);
    ~AsyncClient();
    void connect(const string &userName, const string &password);
    void disconnect();
    void publish(const string &topic, const string &payload, int qos);
    void subscribe(const string &topic, int qos);
private:
    static void onDisconnect(void* context, MQTTAsync_successData* response);
    static void onConnect(void* context, MQTTAsync_successData* response);
    static void onConnectFailure(void* context, MQTTAsync_failureData* response);
    static void connlost(void *context, char *cause);
    static void onSubscribe(void* context, MQTTAsync_successData* response);
    static void onSubscribeFailure(void* context, MQTTAsync_failureData* response);
    static int msgarrvd(void *context, char *topicName, int topicLen, MQTTAsync_message *message);
    MQTTAsync client;
    MQTTAsync_connectOptions connOpts;
    volatile MQTTAsync_token deliveredToken;
};

};

