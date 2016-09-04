#include "MagnometerSenseHAT.h"

#include <OpenALRF/Common/Timing.h>

AudacityRover::MagnometerSenseHAT::MagnometerSenseHAT(OpenALRF::sensorid_t AIdentifier) : AudacityRover::SenseHATSensor(AIdentifier)
{
   Type = OpenALRF::sensorMagnetometer;
   UsedUnit = OpenALRF::unitMicroTesla;
}

bool AudacityRover::MagnometerSenseHAT::NextValue(OpenALRF::Sensor3DData &AValue)
{
   if (IsPowered())
   {
      SenseHAT::d3 sensedata = HAT->get_magno();
      if (sensedata.valid)
      {
         AValue.Timestamp = OpenALRF::GetCurrentTimestamp();
         AValue.Data1 = sensedata.x;
         AValue.Data2 = sensedata.y;
         AValue.Data3 = sensedata.z;

         LatestSensorData = AValue;

         return true;
      }
   }

   return false;
}
