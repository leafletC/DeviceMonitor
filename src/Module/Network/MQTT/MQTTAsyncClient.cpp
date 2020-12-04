#include "MQTTAsyncClient.h"
#include <cstring>
#include <thread>
#include <chrono>
#include <memory>

namespace MQTT {

void AsyncClient::Option::operator=(const Option &option)
{
    this->brokerUrl = option.brokerUrl;
    this->user = option.user;
    this->password = option.password;
    this->pubTopic = option.pubTopic;
    this->subTopic = option.subTopic;
    this->pubQos = option.pubQos;
    this->subQos = option.subQos;
    this->clientId = option.clientId;
    this->retained = option.retained;
    this->keepalive = option.keepalive;
    this->cleansession = option.cleansession;
}

AsyncClient::AsyncClient()
{

}

AsyncClient::AsyncClient(const Option &option)
{
    setOptions(option);
}

AsyncClient::~AsyncClient()
{
}

void AsyncClient::setOptions(const Option &option)
{
    this->option = option;
}

void AsyncClient::run()
{
    MQTTAsync_create(&client, option.brokerUrl.c_str(), option.clientId.c_str(), MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTAsync_setCallbacks(client, this, connlost, msgarrvd, NULL);   
    autoConnect();
}

void AsyncClient::autoConnect()
{
    MQTTAsync_willOptions willOpts = MQTTAsync_willOptions_initializer;
    willOpts.topicName = option.pubTopic.c_str();
    std::unique_ptr<string> msg((string *)emit(setWill, NULL));
    willOpts.message = msg->c_str();
    willOpts.qos = option.pubQos;
    MQTTAsync_connectOptions connOpts;
    connOpts = MQTTAsync_connectOptions_initializer;
    connOpts.username = option.user.c_str();
    connOpts.password = option.password.c_str();
    connOpts.keepAliveInterval = option.keepalive;
    connOpts.cleansession = option.cleansession;
    connOpts.onSuccess = onConnect;
    connOpts.onFailure = onConnectFailure;
    connOpts.will = &willOpts;
    connOpts.context = this;
    int rc;
    if ((rc = MQTTAsync_connect(client, &connOpts)) != MQTTASYNC_SUCCESS)
    {
        printf("Failed to start connect, return code %d\n", rc);
    }
}

void AsyncClient::disconnect()
{
    MQTTAsync_disconnectOptions opts = MQTTAsync_disconnectOptions_initializer;
    int rc;
    printf("Message with token value %d delivery confirmed\n", deliveredToken);
    opts.onSuccess = onDisconnect;
    opts.context = client;
    if ((rc = MQTTAsync_disconnect(client, &opts)) != MQTTASYNC_SUCCESS)
    {
        printf("Failed to start sendMessage, return code %d\n", rc);
    }
}

void AsyncClient::publish(const string &topic, const string &payload, int qos)
{
    MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
    MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
    int rc;
    opts.context = this;
    pubmsg.payload = (void *)payload.c_str();
    pubmsg.payloadlen = payload.length();
    pubmsg.qos = qos;
    pubmsg.retained = option.retained;
    deliveredToken = 0;
    if ((rc = MQTTAsync_sendMessage(client, topic.c_str(), &pubmsg, &opts)) != MQTTASYNC_SUCCESS)
    {
        printf("Failed to start sendMessage, return code %d\n", rc);
    }
}

void AsyncClient::subscribe(const string &topic, int qos)
{
    MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
    int rc;
    opts.onSuccess = onSubscribe;
    opts.onFailure = onSubscribeFailure;
    opts.context = this;
    deliveredToken = 0;
    if ((rc = MQTTAsync_subscribe(client, topic.c_str(), qos, &opts)) != MQTTASYNC_SUCCESS)
    {
        printf("Failed to start subscribe, return code %d\n", rc);
    }
}

int AsyncClient::msgarrvd(void *context, char *topicName, int topicLen, MQTTAsync_message *message)
{
    AsyncClient *client = (AsyncClient *)context;
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: %s\n", (char *)message->payload);
    client->emit(received, message->payload);
    MQTTAsync_freeMessage(&message);
    MQTTAsync_free(topicName);
    return 1;
}

void AsyncClient::onSubscribe(void* context, MQTTAsync_successData* response)
{
    AsyncClient *client = (AsyncClient *)context;
    client->emit(subscribed, response);
    printf("Subscribe succeeded\n");
}

void AsyncClient::onSubscribeFailure(void* context, MQTTAsync_failureData* response)
{
    AsyncClient *client = (AsyncClient *)context;
    client->emit(subscribedFailure, response);
    printf("Subscribe failed, %s\n", response->message);
}

void AsyncClient::onConnect(void* context, MQTTAsync_successData* response)
{
    printf("Successful connection.\n"); 
    AsyncClient *client = (AsyncClient *)context;
    client->emit(connected, response);
}

void AsyncClient::onConnectFailure(void* context, MQTTAsync_failureData* response)
{
    AsyncClient *client = (AsyncClient *)context;
    client->emit(connectFailure, response);
    printf("Connect failed, %s\n", response->message);
    std::this_thread::sleep_for(std::chrono::seconds(client->option.keepalive));
    client->autoConnect();
}

void AsyncClient::onDisconnect(void* context, MQTTAsync_successData* response)
{
    AsyncClient *client = (AsyncClient *)context;
    client->emit(disconnected, response);
    printf("Successful disconnection\n");
}

void AsyncClient::connlost(void *context, char *cause)
{
    AsyncClient *client = (AsyncClient *)context;
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
    printf("Reconnecting\n");
    client->autoConnect();
}

}
