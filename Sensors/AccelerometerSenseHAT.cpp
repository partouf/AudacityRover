#include "AccelerometerSenseHAT.h"

#include <OpenALRF/Common/Timing.h>

AudacityRover::AccelerometerSenseHAT::AccelerometerSenseHAT(OpenALRF::sensorid_t AIdentifier) : AudacityRover::SenseHATSensor(AIdentifier)
{
   Type = OpenALRF::sensorAccelerometer;
   UsedUnit = OpenALRF::unitMetersPerSecond;
}

bool AudacityRover::AccelerometerSenseHAT::NextValue(OpenALRF::Sensor3DData & AValue)
{
   if (IsPowered())
   {
      SenseHAT::d3 sensedata = HAT->get_accel();
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
