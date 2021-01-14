#include "ApplicationServices/UIController.h"


UIController::UIController(){
    _screen = new ScreenController();
    _ledIndicators = new LEDIndicatorsController();

    _screen->resetScreen();
}

void UIController::updateInputs() {

}

void UIController::onWeatherUpdated(WeatherMonitorData weatherMonitorData){
    _currentWeather = weatherMonitorData;
    showCurrentWeather();
}

void UIController::showCurrentWeather(){
    _screen->showWeather();
    _ledIndicators->setPollutionLevel();
    _ledIndicators->setStatusLed();
}

