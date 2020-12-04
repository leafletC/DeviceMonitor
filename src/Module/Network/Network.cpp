#include "Network.h"
#include <thread>
#include <chrono>
#include <istream>

using namespace std;

Network::Network()
{
    ;
    mqtt.connect(MQTT::AsyncClient::received, std::bind(&Network::receive, this, std::placeholders::_1));
    mqtt.connect(MQTT::AsyncClient::connected, std::bind(&Network::subscribe, this, std::placeholders::_1));
    mqtt.connect(MQTT::AsyncClient::subscribedFailure, [&](void *arg){
        this_thread::sleep_for(chrono::seconds(option.mqtt.keepalive));
        return (void *)subscribe(NULL);
    });
    mqtt.connect(MQTT::AsyncClient::setWill, [&](void *arg){
        return (void *)emit(Network::setWill, NULL);
    });
}

void Network::setOptions(const Option &option)
{
    this->option = option;
}

void Network::run()
{
    mqtt.setOptions(option.mqtt);
    mqtt.run();
}

void Network::send(const string &value)
{
    mqtt.publish(option.mqtt.pubTopic, value, option.mqtt.pubQos);
}

void *Network::receive(void *arg)
{
    printf("receive\n");
    return emit(Network::received, arg);
}

void *Network::subscribe(void *arg)
{
    printf("subscribe ...\n");
    mqtt.subscribe(option.mqtt.subTopic, option.mqtt.subQos);
    return NULL;
}