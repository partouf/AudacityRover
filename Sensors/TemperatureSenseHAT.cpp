#include "TemperatureSenseHAT.h"

#include <OpenALRF/Common/Timing.h>
#include <cmath>

AudacityRover::TemperatureSenseHAT::TemperatureSenseHAT(OpenALRF::sensorid_t AIdentifier) : AudacityRover::SenseHATSensor(AIdentifier)
{
   Type = OpenALRF::sensorTemperature;
   UsedUnit = OpenALRF::unitCelcius;
}

bool AudacityRover::TemperatureSenseHAT::NextValue(OpenALRF::Sensor3DData &AValue)
{
   if (IsPowered())
   {
      auto data = HAT->get_temperature();
      if (data.valid)
      {
         AValue.Timestamp = OpenALRF::GetCurrentTimestamp();
         AValue.Data1 = data.x;
         AValue.Data2 = data.y;
         AValue.Data3 = 0;

         LatestSensorData = AValue;

         return true;
      }
   }

   return false;
}
