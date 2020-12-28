#include <SoftwareSerial.h>
#include <Arduino.h>
#include "PlantowerSensor.h"
#include "PMS.h"
#include "Stream.h"

static const uint32_t PMS_READ_DELAY = 30000;
static PlantowerSensor::PmsData FAILED_DATA = PlantowerSensor::PmsData { false };

PlantowerSensor::PlantowerSensor(uint8_t rxPin, uint8_t txPin, uint8_t setPin)
{
    _rxPin = rxPin;
    _txPin = txPin;
    _setPin = setPin;
    isConnected = false;
}

bool PlantowerSensor::connect(bool waitUntilConnected = false)
{
    // Create serial bus using Software serial
    _serial = new SoftwareSerial(_rxPin, _txPin); 
    _serial->begin(PMS::BAUD_RATE); 

    _pms = new PMS(*_serial);    

    // Switch to passive mode.
    _pms->passiveMode();

    // Default state after sensor power, but undefined after ESP restart e.g. by OTA flash, so we have to manually wake up the sensor for sure.
    // Some logs from bootloader is sent via Serial port to the sensor after power up. This can cause invalid first read or wake up so be patient and wait for next read cycle.
    _pms->wakeUp();    
    delay(1000);
    
    _pms->sleep();
    isInSleepMode = true;
    isConnected = true;
}

void PlantowerSensor::sleep()
{
    if(!isConnected) return;

    _pms->sleep();
    isInSleepMode = true;
}

void PlantowerSensor::wakeUp()
{
    if(!isConnected) return;

    _pms->wakeUp();
    isInSleepMode = false;
}

PlantowerSensor::PmsData PlantowerSensor::readData()
{
    if(!isConnected) return FAILED_DATA;

    PMS::DATA data;

    // Clear buffer (removes potentially old data) before read. Some data could have been also sent before switching to passive mode.
    while (_serial->available()) { _serial->read(); }

    _pms->requestRead();

    if (_pms->readUntil(data))
    {
        return PmsData {
            .isDataReceived = true,
            .PM_1_0 = data.PM_AE_UG_1_0,
            .PM_2_5 = data.PM_AE_UG_2_5,
            .PM_10_0 = data.PM_AE_UG_10_0
        };
    }

    return FAILED_DATA;
}

PlantowerSensor::PmsData PlantowerSensor::readDataSyncronioslyAndSleep()
{
    if(!isConnected) return FAILED_DATA;

    this->wakeUp();
    delay(PMS_READ_DELAY);
    PmsData data = this->readData();
    this->sleep();

    return data;
}