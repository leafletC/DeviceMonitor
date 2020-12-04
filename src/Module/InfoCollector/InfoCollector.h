#pragma once

#include <string>
#include <list>
#include "Database.h"
#include "MyModule.h"
#include "TimedTask.h"

using namespace std;

class InfoCollector : public MyModule
{
public:
    enum class InfoType
    {
        HARDWARE = 1,
        OS = 2,
        NETWORK = 3,
        BUSINESS = 4,
        APP = 5,
        OTHER
    };
    struct Option
    {
        string devId;
        string devType;
        string card;
        string disk;
        string linknode;
        int32_t beginPoint;
        int32_t defaultFreq;
        int32_t freq1;
        int32_t freq2;
        int32_t freq3;
        int32_t freq4;
        int32_t freq5;
        void operator=(const Option &option);
    };
    struct NetQos
    {
        string lostPacketRate;
        string delay;
    };
    enum Signal
    {
        queryDB,
    };
    InfoCollector();
    string agmEvents();
    string cpuUsage();
    string cpuTemperature();
    string cpuCoreNum();
    string cpuClockRate();
    string netcardMac();
    string netcardType();
    string netcardIP();
    string memorySpace();
    string mountPathError();
    string diskUsage();
    string diskLost();
    string powerStatus();
    string diskType();
    string hardwareStatus();
    string offline();
    NetQos netQos();
    string netStatus();
    string currentlinkCnt();
    string waitOrCloselinkCnt();
    string crondStatus();
    string crondList();
    string osStatus();
    string enforceStatus();
    string dbStatus();
    string dbCpu();
    string dbMemory();
    string dbPort();
    string appStatus();
    uint32_t getBeginPoint();
    void *connectLost(void *arg);
    int getTimedTaskOption(const string &value, list<TimedTask::Option> &opts, uint32_t *defaultFreq);
    void setOptions(const Option &option);
    void setFreq(InfoType type, uint32_t freq);
    uint32_t getFreq(InfoType type);
private:
    Option option;
};