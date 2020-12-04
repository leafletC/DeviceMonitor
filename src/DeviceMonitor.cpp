#include "DeviceMonitor.h"
#include <thread>
#include <functional>
#include <iostream>
#include <getopt.h>
#include "config.h"
#include "MyModule.h"

using namespace std;

DeviceMonitor::DeviceMonitor(int argc, char *argv[])
    :initSuccess(false)
{
    string iniName = getOptions(argc, argv);
    if(iniName.empty())
    {
        return ;
    }
    if(0 != setOptions(iniName))
    {
        return ;
    }
    info.connect(InfoCollector::queryDB, std::bind(&DBConnector::onQuery, &db, std::placeholders::_1));
    net.connect(Network::received, std::bind(&DeviceMonitor::setTaskDuration, this, std::placeholders::_1));
    net.connect(Network::setWill, std::bind(&InfoCollector::connectLost, &info, std::placeholders::_1));
    db.run();
    net.run();
    info.cpuUsage();
    auto timePoint = time_point_cast<minutes>(steady_clock::now()) + seconds(info.getBeginPoint());
    registerTimedTask(TimedTask((uint32_t)InfoCollector::InfoType::BUSINESS, [&](){
        net.send(info.agmEvents());
    }, timePoint, seconds(info.getFreq(InfoCollector::InfoType::BUSINESS))));

    registerTimedTask(TimedTask((uint32_t)InfoCollector::InfoType::HARDWARE, [&](){
        net.send(info.hardwareStatus());
    }, timePoint, seconds(info.getFreq(InfoCollector::InfoType::HARDWARE))));

    registerTimedTask(TimedTask((uint32_t)InfoCollector::InfoType::NETWORK, [&](){
        net.send(info.netStatus());
    }, timePoint, seconds(info.getFreq(InfoCollector::InfoType::NETWORK))));

    registerTimedTask(TimedTask((uint32_t)InfoCollector::InfoType::OS, [&](){
        net.send(info.osStatus());
    }, timePoint, seconds(info.getFreq(InfoCollector::InfoType::OS))));

    registerTimedTask(TimedTask((uint32_t)InfoCollector::InfoType::APP, [&](){
        net.send(info.appStatus());
    }, timePoint, seconds(info.getFreq(InfoCollector::InfoType::APP))));
    initSuccess = true;
}

void *DeviceMonitor::setTaskDuration(void *arg)
{
    list<TimedTask::Option> options;
    uint32_t defaultFreq;
    info.getTimedTaskOption((char *)arg, options, &defaultFreq);
    info.setFreq(InfoCollector::InfoType::OTHER, defaultFreq);
    ini.SetIntValue("info", string("freq"), defaultFreq);
    
    for(auto option:options)
    {
        info.setFreq((InfoCollector::InfoType)option.taskId, option.timeDuration.count());
        ini.SetIntValue("info", string("freq")+to_string(option.taskId), option.timeDuration.count());
        for(auto &task:tasks)
        {
            if(option.taskId == task.getTaskId())
            {
                task.setTimeDuration(option.timeDuration);
            }
        }
    }
    ini.Save();
    return NULL;
}

void DeviceMonitor::registerTimedTask(const TimedTask &task)
{
    tasks.push_back(task);
}

void DeviceMonitor::exec()
{
    if(!initSuccess)
    {
        return ;
    }
    for( ; ; )
    {
        for(auto &task:tasks)
        {
            task.run();
        }
        this_thread::sleep_for(milliseconds(100));
    }
}

string DeviceMonitor::getOptions(int argc, char *argv[])
{
    string iniName = "/etc/dm.cnf";
    struct option longOptions[] = {
        {"config", required_argument, NULL, 'i'},
        {"help", no_argument, NULL, 'h'},
    };
    int32_t optionIndex;
    int32_t ch = 0;
    while((ch = getopt_long(argc, argv, "i:h", longOptions, &optionIndex)) != -1)
    {
        switch(ch)
        {
            case 'i':iniName = optarg; break;
            case 'h':iniName.clear();
            default: printUsage(); break;
        }
    }
    return iniName;
}

int DeviceMonitor::setOptions(const string &iniName)
{
    ini.SetCommentDelimiter(";");
    if(ini.Load(iniName) != 0)
    {
        printf("Parse %s fail.\n", iniName.c_str());
        return -1;
    }
    InfoCollector::Option infoOpts;
    ini.GetStringValue("info", "id", &infoOpts.devId);
    ini.GetStringValue("info", "type", &infoOpts.devType);
    ini.GetStringValue("info", "netcard", &infoOpts.card);
    ini.GetStringValue("info", "disk", &infoOpts.disk);
    ini.GetStringValue("info", "linknode", &infoOpts.linknode);
    ini.GetIntValue("info", "beginpoint", &infoOpts.beginPoint);
    ini.GetIntValue("info", "freq", &infoOpts.defaultFreq);
    ini.GetIntValue("info", "freq1", &infoOpts.freq1);
    ini.GetIntValue("info", "freq2", &infoOpts.freq2);
    ini.GetIntValue("info", "freq3", &infoOpts.freq3);
    ini.GetIntValue("info", "freq4", &infoOpts.freq4);
    ini.GetIntValue("info", "freq5", &infoOpts.freq5);
    info.setOptions(infoOpts);

    DBConnector::Option dbOpts;
    ini.GetStringValue("db", "host", &dbOpts.host);
    ini.GetIntValue("db", "port", &dbOpts.port);
    ini.GetStringValue("db", "user", &dbOpts.user);
    ini.GetStringValue("db", "passwd", &dbOpts.password);
    ini.GetStringValue("db", "name", &dbOpts.name);
    ini.GetBoolValue("db", "reconnect", &dbOpts.reconnect);
    ini.GetIntValue("db", "timeout",  &dbOpts.timeout);
    db.setOptions(dbOpts);

    Network::Option netOpts;
    ini.GetStringValue("net", "server", &netOpts.mqtt.brokerUrl);
    ini.GetStringValue("net", "user", &netOpts.mqtt.user);
    ini.GetStringValue("net", "passwd", &netOpts.mqtt.password);
    ini.GetStringValue("net", "pub", &netOpts.mqtt.pubTopic);
    ini.GetStringValue("net", "sub", &netOpts.mqtt.subTopic);
    ini.GetIntValue("net", "pubqos", &netOpts.mqtt.pubQos);
    ini.GetIntValue("net", "subqos", &netOpts.mqtt.subQos);
    ini.GetBoolValue("net", "retained", &netOpts.mqtt.retained);
    ini.GetIntValue("net", "keepalive", &netOpts.mqtt.keepalive);
    ini.GetBoolValue("net", "cleansession", &netOpts.mqtt.cleansession);
    netOpts.mqtt.clientId = infoOpts.devId;
    net.setOptions(netOpts);
    return 0;
}

void DeviceMonitor::printUsage()
{
    printf("Device Monitor ver%x.%x.%02x Copyright@2020 XMRBI\n\n"
        "Usage: DeviceMonitor [OPTIONS]\n\n"
        "Default options are read from the following files in the given order:\n"
        "/etc/dm.cnf\n" 
        "basic user options for Device Monitor, with defaults in [ ]:\n"
        "-i --config=<file>  read default options from the given file.\n"
        "-h --help show this message\n", DeviceMonitor_VERSION_MAJOR, DeviceMonitor_VERSION_MINOR, DeviceMonitor_VERSION_PATCH);
}