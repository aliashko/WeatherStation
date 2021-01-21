#ifndef HardwareDevicesHealthcheck_H
#define HardwareDevicesHealthcheck_H

#include "HardwareModules/HardwareModulesRegistry.h"

class HardwareDevicesHealthcheck
{
public:
    HardwareDevicesHealthcheck(HardwareModulesRegistry* hardwareModulesRegistry);
    void getDevicesConnectionStatus();

private:
    HardwareModulesRegistry* _hardwareModulesRegistry;
};

#endif