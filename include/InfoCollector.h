#pragma once

class InfoCollector
{
public:
    static int cpuTemperature();
    static int powerStatus();
    static int memorySpace();
    static int freeDiskSpace();
};