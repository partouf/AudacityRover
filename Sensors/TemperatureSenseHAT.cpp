#include "TemperatureSenseHAT.h"

#include <OpenALRF/Common/Timing.h>

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
      if (!isnan(data))
      {
         AValue.Timestamp = OpenALRF::GetCurrentTimestamp();
         AValue.Data1 = data;
         AValue.Data2 = 0;
         AValue.Data3 = 0;

         LatestSensorData = AValue;

         return true;
      }
   }

   return false;
}
