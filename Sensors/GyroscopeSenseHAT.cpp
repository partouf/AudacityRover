#include "GyroscopeSenseHAT.h"
#include <OpenALRF/Common/Timing.h>

AudacityRover::GyroscopeSenseHAT::GyroscopeSenseHAT(OpenALRF::sensorid_t AIdentifier) : SenseHATSensor::SenseHATSensor(AIdentifier)
{
}

bool AudacityRover::GyroscopeSenseHAT::NextValue(OpenALRF::Sensor3DData & AValue)
{
   if (IsPowered())
   {
      SenseHAT::d3 sensedata = HAT->get_gyro();
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
