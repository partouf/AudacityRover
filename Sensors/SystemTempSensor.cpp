#include "SystemTempSensor.h"

#include <fstream>
#include <string>
#include <iostream>
#include <OpenALRF/Common/Timing.h>
#include <Groundfloor/Atoms/Defines.h>
#include <Groundfloor/Bookshelfs/BValue.h>

AudacityRover::SystemTempSensor::SystemTempSensor(OpenALRF::sensorid_t AIdentifier) : OpenALRF::ISensor(AIdentifier)
{
   UsedUnit = OpenALRF::unitCelcius;
   Type = OpenALRF::sensorTemperature;
}

void AudacityRover::SystemTempSensor::PowerOff()
{
}

void AudacityRover::SystemTempSensor::PowerOn()
{
}

bool AudacityRover::SystemTempSensor::IsPowered() const
{
   return true;
}

bool AudacityRover::SystemTempSensor::NextValue(OpenALRF::Sensor3DData & AValue)
{
#ifdef GF_OS_WIN32
   std::ifstream temperatureFile("./temp.txt");
#else
   std::ifstream temperatureFile("/sys/class/thermal/thermal_zone0/temp");
#endif
   if (temperatureFile.is_open())
   {
      std::string val;

      temperatureFile >> val;
      temperatureFile.close();

      AValue.Timestamp = OpenALRF::GetCurrentTimestamp();

      AValue.Data1 = std::stoi(val) / 1000.0;
      AValue.Data2 = 0;
      AValue.Data3 = 0;

      return true;
   }

   return false;
}

