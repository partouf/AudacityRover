#include "MagnometerSenseHAT.h"

#include <OpenALRF/Common/Timing.h>

#define	RTMATH_PI					3.1415926535
#define	RTMATH_DEGREE_TO_RAD		(RTMATH_PI / 180.0)
#define	RTMATH_RAD_TO_DEGREE		(180.0 / RTMATH_PI)

AudacityRover::MagnometerSenseHAT::MagnometerSenseHAT(OpenALRF::sensorid_t AIdentifier) : AudacityRover::SenseHATSensor(AIdentifier)
{
   Type = OpenALRF::sensorMagnetometer;
   UsedUnit = OpenALRF::unitDegrees;
}

bool AudacityRover::MagnometerSenseHAT::NextValue(OpenALRF::Sensor3DData &AValue)
{
   if (IsPowered())
   {
      SenseHAT::d3 sensedata = HAT->get_magno();
      if (sensedata.valid)
      {
         AValue.Timestamp = OpenALRF::GetCurrentTimestamp();

         AValue.Data1 = sensedata.x * RTMATH_RAD_TO_DEGREE;
         AValue.Data2 = sensedata.y * RTMATH_RAD_TO_DEGREE;
         AValue.Data3 = sensedata.z * RTMATH_RAD_TO_DEGREE;

         LatestSensorData = AValue;

         return true;
      }
   }

   return false;
}
