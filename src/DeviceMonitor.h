#pragma once

#include <list>
#include "TimedTask.h"
#include "Network.h"
#include "Database.h"
#include "inifile.h"
#include "InfoCollector.h"

using namespace inifile;

class DeviceMonitor : public MyModule
{
public:
    DeviceMonitor(int argc, char *argv[]);
    void exec();
private:
    void registerTimedTask(const TimedTask &task);
    void *setTaskDuration(void *arg);
    string getOptions(int argc, char *argv[]);
    void printUsage();
    int setOptions(const string &ini);
    list<TimedTask> tasks;
    bool initSuccess;
    Network net;
    DBConnector db;
    InfoCollector info;
    IniFile ini;
};