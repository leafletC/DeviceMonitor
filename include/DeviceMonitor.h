#pragma once

#include "Network.h"
#include "InfoCollector.h"

class DeviceMonitor
{
public:
    DeviceMonitor(int argc, char *argv[]);
    void exec();
};