#include "HardwareModules/HardwareModulesRegistry.h"
#include "Models/WeatherMonitorData.h"
#include <Ticker.h>

typedef void (*WeatherMonitorUpdatedEventCallback)(WeatherMonitorData);

class WeatherMonitor
{
public:
    WeatherMonitor(HardwareModulesRegistry* hardwareModulesRegistry);
    void run();
    void updateTimers();
    void addUpdatedEventHandler(WeatherMonitorUpdatedEventCallback callback);
    void reconnectSensors();
    void resetSensors();
    enum class WeatherMonitorState {DISABLED, IDLE, MEASURING};
    WeatherMonitorState state = WeatherMonitorState::DISABLED;

private:
    void startMeasuring();
    void finishMeasuring();

    WeatherMonitorUpdatedEventCallback _onUpdateCallback;
    Ticker* _timer;
    unsigned long _startMeasuringTimestamp;

    HardwareModulesRegistry* _hardwareModulesRegistry;
};
