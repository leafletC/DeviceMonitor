#include "DeviceMonitor.h"
#include <thread>

DeviceMonitor::DeviceMonitor(int argc, char *argv[])
{

}

void DeviceMonitor::exec()
{

}

int main(int argc, char *argv[])
{
    MQTT::AsyncClient client("tcp://172.16.180.204:1883", "client_test");
    client.connect("admin", "admin");
    this_thread::sleep_for(std::chrono::seconds(1));
    client.subscribe("t1", 0);
    this_thread::sleep_for(std::chrono::seconds(1));
    client.publish("t1", "test demo", 0);
    while(true)
    {
        this_thread::sleep_for(std::chrono::seconds(5));
    }
}