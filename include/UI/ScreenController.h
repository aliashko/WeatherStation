#include "HardwareModules/HardwareModulesRegistry.h"
#include "OLED.h"
#include "Models/WeatherMonitorData.h"

class ScreenController
{
public:
    ScreenController(HardwareModulesRegistry* hardwareModulesRegistry);
    void clearScreen();
    void showSplashScreen();
    void showIndoorWeather(WeatherMonitorData weatherData);
    void showOutdoorTemperature(WeatherMonitorData weatherData);
    void showOutdoorHumidityAndPressure(WeatherMonitorData weatherData);
    void showAirQualityMeasurements(WeatherMonitorData weatherData);
    void showAirPollution(WeatherMonitorData weatherData);

private:
    OLED* _oled;
};