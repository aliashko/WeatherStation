#include "RGBLed.h"
#include "MCPExtender.h"
#include "Models/WeatherMonitorData.h"
#include <Ticker.h>

class LEDIndicatorsController
{
public:
    LEDIndicatorsController();
    void setPollutionLevel(WeatherMonitorData weatherData);
    void setWeatherStatusLed(WeatherMonitorData weatherData);
    void updateSystemStatusLed();

private:
    RGBLed* _pollutionRGBLed;
    MCPExtender* _mcp;

    Ticker* _timerSlow;
    Ticker* _timerFast;
    bool _fastBlinkingLedOn = false;
    bool _slowBlinkingLedOn = false;
};