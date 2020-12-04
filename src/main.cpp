#include "DeviceMonitor.h"

int main(int argc, char *argv[])
{
    DeviceMonitor deviceMonitor(argc, argv);
    deviceMonitor.exec();
    return 0;
}