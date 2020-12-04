#include "InfoCollector.h"
#include "MyModule.h"
#include "Utils.h"
#include <json/json.h>
#include <unistd.h>

void InfoCollector::Option::operator=(const Option &option)
{
    this->devId = option.devId;
    this->devType = option.devType;
    this->card = option.card;
    this->disk = option.disk;
    this->linknode = option.linknode;
    this->beginPoint = option.beginPoint;
    this->defaultFreq = option.defaultFreq;
    this->freq1 = option.freq1;
    this->freq2 = option.freq2;
    this->freq3 = option.freq3;
    this->freq4 = option.freq4;
    this->freq5 = option.freq5;
    
}

InfoCollector::InfoCollector()
{

}

void InfoCollector::setOptions(const Option &option)
{
    this->option = option;
}

string InfoCollector::cpuUsage()
{
    return Utils::trim(OS::execShell("top -n 1 | grep Cpu | awk '{print 100 - $8}'"));
}

string InfoCollector::cpuCoreNum()
{
    return Utils::trim(OS::execShell("cat /proc/cpuinfo |grep processor|wc -l"));
}

string InfoCollector::cpuClockRate()
{
    return Utils::trim(OS::execShell("cat /proc/cpuinfo |grep MHz|uniq|awk '{print $4}'"));
}

string InfoCollector::cpuTemperature()
{
    return Utils::trim(OS::execShell(""));
}

string InfoCollector::netcardMac()
{
    return Utils::trim(OS::execShell("ip link show "+ option.card + " | grep ether | awk '{print $2}'"));
}

string InfoCollector::netcardType()
{
    return Utils::trim(OS::execShell("lspci | grep -i ethernet | awk -F: '{print $3}'"));
}

string InfoCollector::netcardIP()
{
    return Utils::trim(OS::execShell("ifconfig " + option.card + " | grep -w inet | awk '{print $2}'"));
}

string InfoCollector::memorySpace()
{
    return Utils::trim(OS::execShell("top -n 1 | grep 'MiB Mem' | awk '{print $8/$4*100}'"));
}

string InfoCollector::diskUsage()
{
    return Utils::trim(OS::execShell("df | awk '{if($6 == \"/\")print $5}'"));
}

string InfoCollector::diskLost()
{
    return Utils::trim(OS::execShell("df | awk '{if($1 == \"" + option.disk + "\")print 1}'").empty()?"1":"0");
}

string InfoCollector::mountPathError()
{
    return Utils::trim(OS::execShell("df | awk '{if($1 == \"" + option.disk + "\")print $6}' | uniq").empty()?"1":"0");
}

string InfoCollector::currentlinkCnt()
{
    return Utils::trim(OS::execShell("netstat -nat|grep ESTABLISHED|wc -l"));    
}

string InfoCollector::waitOrCloselinkCnt()
{
    return Utils::trim(OS::execShell("netstat -nat|grep -E 'CLOSING|TIME_WAIT' |wc -l"));
}

string InfoCollector::powerStatus()
{
    return "";
}

string InfoCollector::diskType()
{
    return Utils::trim(OS::execShell("df -T| awk '{if($1 == \"" + option.disk + "\")print $2}' | uniq"));
}

string InfoCollector::offline()
{
    return Utils::trim(OS::execShell("ifconfig | grep " + option.card).empty()?"1":"0");
}

string InfoCollector::crondStatus()
{
    return Utils::trim(OS::execShell("ps aux | grep crond | grep -v grep").empty()?"0":"1");
}

string InfoCollector::crondList()
{
    return Utils::trim(OS::execShell("cat /etc/crontab | grep -v '#' | grep -vE 'SHELL|PATH|MAILTO'"));
}

string InfoCollector::enforceStatus()
{
    return Utils::trim(OS::execShell("getenforce"));
}

string InfoCollector::dbStatus()
{
    return Utils::trim(OS::execShell("ps aux | awk '{if($1==\"mysql\")print $1}'").empty()?"0":"1");
}

string InfoCollector::dbCpu()
{
    return Utils::trim(OS::execShell("ps aux | awk '{if($1==\"mysql\")print $3}'"));
}

string InfoCollector::dbMemory()
{
    return Utils::trim(OS::execShell("ps aux | awk '{if($1==\"mysql\")print $4}'"));
}

string InfoCollector::dbPort()
{
    return Utils::trim(OS::execShell(""));
}

string InfoCollector::hardwareStatus()
{
    Json::Value root;
    Json::StreamWriterBuilder builder;
    builder["emitUTF8"] = true;
    int64_t timestamp = time_point_cast<milliseconds>(system_clock::now()).time_since_epoch().count();
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    Json::Value events;
    root["monitorId"] = option.devId;
    root["monitorSource"] = option.devType;
    root["monitorType"] = static_cast<uint8_t>(InfoType::HARDWARE);
    Json::Value event;
    event["event_code"] = "cpuusage";
    event["event_value"] = cpuUsage();
    event["create_time"] = timestamp;
    events.append(event);
    event["event_code"] = "cpunum";
    event["event_value"] = cpuCoreNum();
    event["create_time"] = timestamp;
    events.append(event);
    event["event_code"] = "clockrate";
    event["event_value"] = cpuClockRate();
    event["create_time"] = timestamp;
    events.append(event);
    event["event_code"] = "mac";
    event["event_value"] = netcardMac();
    event["create_time"] = timestamp;
    events.append(event);
    event["event_code"] = "netcardmodel";
    event["event_value"] = netcardType();
    event["create_time"] = timestamp;
    events.append(event);
    event["event_code"] = "ip";
    event["event_value"] = netcardIP();
    event["create_time"] = timestamp;
    events.append(event);
    event["event_code"] = "memoryusage";
    event["event_value"] = memorySpace();
    event["create_time"] = timestamp;
    events.append(event);
    event["event_code"] = "disklost";
    event["event_value"] = diskLost();
    event["create_time"] = timestamp;
    events.append(event);
    event["event_code"] = "mountexception";
    event["event_value"] = mountPathError();
    event["create_time"] = timestamp;
    events.append(event);
    event["event_code"] = "diskusage";
    event["event_value"] = diskUsage();
    event["create_time"] = timestamp;
    events.append(event);
    event["event_code"] = "disktype";
    event["event_value"] = diskType();
    event["create_time"] = timestamp;
    events.append(event);
    root["monitorLevel"] = 0;
    root["monitorDetail"] = events;
    root["monitorCreateDate"] = timestamp;
    std::ostringstream os;
    writer->write(root, &os);
    return os.str();
}

InfoCollector::NetQos InfoCollector::netQos()
{
    NetQos qos;
    string src = OS::execShell("ping -Ac10 " + option.linknode + "| grep -E 'packet|rtt'");
    vector<string> res = Utils::split(src, " ");
    qos.lostPacketRate = res[6];
    if(res.size() > 15)
    {
        res = Utils::split(res[13], "/");
        if(res.size() >= 4)
        {
            qos.delay = res[1];
            Utils::trim(qos.lostPacketRate);
            Utils::trim(qos.delay);
            return qos;
        }    
    }
    qos.delay = "NaN";
    return qos;
}

string InfoCollector::netStatus()
{
    NetQos qos = netQos();
    Json::Value root;
    Json::StreamWriterBuilder builder;
    builder["emitUTF8"] = true;
    int64_t timestamp = time_point_cast<milliseconds>(system_clock::now()).time_since_epoch().count();
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    Json::Value events;
    root["monitorId"] = option.devId;
    root["monitorSource"] = option.devType;
    root["monitorType"] = static_cast<uint8_t>(InfoType::NETWORK);
    Json::Value event;
    event["event_code"] = "offline";
    event["event_value"] = offline();
    event["create_time"] = timestamp;
    event["event_code"] = "packelossrate";
    event["event_value"] = qos.lostPacketRate;
    event["create_time"] = timestamp;
    events.append(event);
    event["event_code"] = "delaytime";
    event["event_value"] = qos.delay;
    event["create_time"] = timestamp;
    events.append(event);
    event["event_code"] = "onlinenum";
    event["event_value"] = currentlinkCnt();
    event["create_time"] = timestamp;
    events.append(event);
    event["event_code"] = "closewaitstate";
    event["event_value"] = waitOrCloselinkCnt();
    event["create_time"] = timestamp;
    events.append(event);
    root["monitorLevel"] = 0;
    root["monitorDetail"] = events;
    root["monitorCreateDate"] = timestamp;
    std::ostringstream os;
    writer->write(root, &os);
    return os.str();
}

string InfoCollector::osStatus()
{
    Json::Value root;
    Json::StreamWriterBuilder builder;
    builder["emitUTF8"] = true;
    int64_t timestamp = time_point_cast<milliseconds>(system_clock::now()).time_since_epoch().count();
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    Json::Value events;
    root["monitorId"] = option.devId;
    root["monitorSource"] = option.devType;
    root["monitorType"] = static_cast<uint8_t>(InfoType::NETWORK);
    Json::Value event;
    event["event_code"] = "startup";
    event["event_value"] = crondStatus();
    event["create_time"] = timestamp;
    events.append(event);
    event["event_code"] = "contentlist";
    event["event_value"] = crondList();
    event["create_time"] = timestamp;
    events.append(event);
    event["event_code"] = "accesscontrolpolicy";
    event["event_value"] = enforceStatus();
    event["create_time"] = timestamp;
    events.append(event);
    root["monitorLevel"] = 0;
    root["monitorDetail"] = events;
    root["monitorCreateDate"] = timestamp;
    std::ostringstream os;
    writer->write(root, &os);
    return os.str();
}

void *InfoCollector::connectLost(void *arg)
{
    Json::Value root;
    Json::StreamWriterBuilder builder;
    builder["emitUTF8"] = true;
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    Json::Value events;
    root["monitorId"] = option.devId;
    root["monitorSource"] = option.devType;
    root["monitorType"] = static_cast<uint8_t>(InfoType::NETWORK);
    Json::Value event;
    event["event_code"] = "devlastwords";
    event["event_value"] = "1";
    event["create_time"] = 0;
    events.append(event);
    root["monitorLevel"] = 1;
    root["monitorDetail"] = events;
    root["monitorCreateDate"] = 0;
    std::ostringstream os;
    writer->write(root, &os);
    string *sptr = new string(os.str());
    return sptr;
}

string InfoCollector::agmEvents()
{
    string result;
    string sql = "select EVENT_CODE,DEVICE_IN_STATUS,LAST_TIME from t_slestate_event_current;";
    unique_ptr<DBConnector::Table> tblPtr((DBConnector::Table *)emit(queryDB, &sql));
    if(tblPtr.get() == nullptr)
    {
        return result;
    }
    int level = 0;
    Json::Value root;
    Json::StreamWriterBuilder builder;
    builder["emitUTF8"] = true;
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    Json::Value events;
    root["monitorId"] = option.devId;
    root["monitorSource"] = option.devType;
    root["monitorType"] = static_cast<uint8_t>(InfoType::BUSINESS);
    for(auto &row : *tblPtr)
    {
        Json::Value event;
        event["event_code"] = row[0];
        event["event_value"] = row[1];
        struct tm tmTime = {};
        strptime(row[2].c_str(), "%Y%m%d%H%M%S", &tmTime);
        event["create_time"] = mktime(&tmTime)*1000L;
        events.append(event);
        int value = stoi(row[1]);
        if(value > level)
            level = value;
    }
    root["monitorLevel"] = level;
    root["monitorDetail"] = events;
    root["monitorCreateDate"] = time_point_cast<milliseconds>(system_clock::now()).time_since_epoch().count();
    std::ostringstream os;
    writer->write(root, &os);
    result = os.str();
    time_t t = time(NULL);
    printf("event time: %s", ctime(&t));
    return result;
}

string InfoCollector::appStatus()
{
    Json::Value root;
    Json::StreamWriterBuilder builder;
    builder["emitUTF8"] = true;
    int64_t timestamp = time_point_cast<milliseconds>(system_clock::now()).time_since_epoch().count();
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    Json::Value events;
    root["monitorId"] = option.devId;
    root["monitorSource"] = option.devType;
    root["monitorType"] = static_cast<uint8_t>(InfoType::NETWORK);
    Json::Value event;
    event["event_code"] = "dbisstartup";
    event["event_value"] = dbStatus();
    event["create_time"] = timestamp;
    events.append(event);
    event["event_code"] = "dbuscpu";
    event["event_value"] = dbCpu();
    event["create_time"] = timestamp;
    events.append(event);
    event["event_code"] = "dbememory";
    event["event_value"] = dbMemory();
    event["create_time"] = timestamp;
    events.append(event);
    root["monitorLevel"] = 0;
    root["monitorDetail"] = events;
    root["monitorCreateDate"] = timestamp;
    std::ostringstream os;
    writer->write(root, &os);
    return os.str();
}

int InfoCollector::getTimedTaskOption(const string &value, list<TimedTask::Option> &opts, uint32_t *defaultFreq)
{
    Json::CharReaderBuilder builder;
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    Json::Value root;
    Json::Value arr;
    string err;
    if(!reader->parse(value.c_str(), value.c_str() + value.size(), &root, &err))
    {
        return -1;
    }
    *defaultFreq = root["defaultCollectFreq"].asInt();
    arr = root["collectFreqDetail"];
    for(uint i = 0; i < arr.size(); i++)
    {
        TimedTask::Option option;
        option.taskId = arr[i]["monitorType"].asInt();
        option.timeDuration = seconds(arr[i]["CollectFreq"].asInt());
        opts.push_back(option);
    }
    return 0;
}

void InfoCollector::setFreq(InfoType type, uint32_t freq)
{
    switch(type)
    {
        case InfoType::HARDWARE: option.freq1 = freq; break;
        case InfoType::OS: option.freq2 = freq; break;
        case InfoType::NETWORK: option.freq3 = freq; break;
        case InfoType::APP: option.freq4 = freq; break;
        case InfoType::BUSINESS: option.freq5 = freq; break;
        default: option.defaultFreq = freq; break;
    }
}

uint32_t InfoCollector::getFreq(InfoType type)
{
    uint32_t freq = 0;
    switch(type)
    {
        case InfoType::HARDWARE: freq = option.freq1; break;
        case InfoType::OS: freq = option.freq2; break;
        case InfoType::NETWORK: freq = option.freq3; break;
        case InfoType::APP: freq = option.freq4; break;
        case InfoType::BUSINESS: freq = option.freq5; break;
        default: freq = option.defaultFreq; break;
    }
    if(0 == freq)
    {
        return option.defaultFreq;
    }
    return freq;
}

uint32_t InfoCollector::getBeginPoint()
{
    return option.beginPoint;
}