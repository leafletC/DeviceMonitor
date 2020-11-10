#include "MQTTAsyncClient.h"

namespace MQTT {

AsyncClient::AsyncClient(const string &url, const string &id)
{
    MQTTAsync_create(&client, url.c_str(), id.c_str(), MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTAsync_setCallbacks(client, NULL, connlost, msgarrvd, NULL);
}

AsyncClient::~AsyncClient()
{
    MQTTAsync_destroy(&client);
}

void AsyncClient::connect(const string &userName, const string &password)
{
    int rc;
    connOpts = MQTTAsync_connectOptions_initializer;
    connOpts.username = userName.c_str();
    connOpts.password = password.c_str();
    connOpts.keepAliveInterval = 20;
    connOpts.cleansession = 1;
    connOpts.onSuccess = onConnect;
    connOpts.onFailure = onConnectFailure;
    connOpts.context = client;
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
    printf("Successful connection\n");
    opts.context = client;
    pubmsg.payload = (void *)payload.c_str();
    pubmsg.payloadlen = payload.length();
    pubmsg.qos = qos;
    pubmsg.retained = 0;
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
    opts.context = client;
    deliveredToken = 0;
    if ((rc = MQTTAsync_subscribe(client, topic.c_str(), qos, &opts)) != MQTTASYNC_SUCCESS)
    {
            printf("Failed to start subscribe, return code %d\n", rc);
            exit(EXIT_FAILURE);
    }
}

int AsyncClient::msgarrvd(void *context, char *topicName, int topicLen, MQTTAsync_message *message)
{
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: %s\n", (char *)message->payload);
    MQTTAsync_freeMessage(&message);
    MQTTAsync_free(topicName);
    return 1;
}

void AsyncClient::onSubscribe(void* context, MQTTAsync_successData* response)
{
        printf("Subscribe succeeded\n");
}

void AsyncClient::onSubscribeFailure(void* context, MQTTAsync_failureData* response)
{
        printf("Subscribe failed, rc %d\n", response ? response->code : 0);
}

void AsyncClient::onConnect(void* context, MQTTAsync_successData* response)
{
    printf("Successful connection\n"); 
}

void AsyncClient::onConnectFailure(void* context, MQTTAsync_failureData* response)
{
    printf("Connect failed, rc %d\n", response ? response->code : 0);
}

void AsyncClient::onDisconnect(void* context, MQTTAsync_successData* response)
{
    printf("Successful disconnection\n");
}

void AsyncClient::connlost(void *context, char *cause)
{
    MQTTAsync client = (MQTTAsync)context;
    MQTTAsync_connectOptions connOpts = MQTTAsync_connectOptions_initializer;
    int rc;
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
    printf("Reconnecting\n");
    connOpts.keepAliveInterval = 20;
    connOpts.cleansession = 1;
    if ((rc = MQTTAsync_connect(client, &connOpts)) != MQTTASYNC_SUCCESS)
    {
        printf("Failed to start connect, return code %d\n", rc);
    }
}

}
