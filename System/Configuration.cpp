#include "Configuration.h"

#include <Groundfloor/Atoms/Defines.h>

#define pi1hostname "pi1wifi"

#ifdef GF_OS_WIN32
#define pi2hostname "testpc"
#else
#define pi2hostname "pi2wifi"
#endif

AudacityRover::Configuration DefaultInstance;

AudacityRover::Configuration::Configuration()
{
   CommandServerPort = 13666;
   SensorServerPort = 13777;

   GoPiGo               = pi2hostname;

   Accelerometer1       = { 11, pi1hostname };
   Gyroscope1           = { 12, pi1hostname };
   Magnometer1          = { 13, pi1hostname };

   Temperature1         = { 14, pi1hostname };
   Barometer1           = { 15, pi1hostname };
   Humidity1            = { 16, pi1hostname };

   Dummy1               = { 21, pi1hostname };
   Dummy2               = { 22, pi2hostname };

   CameraFrontLeft      = pi2hostname;
   CameraFrontRight     = pi1hostname;
}

AudacityRover::Configuration * AudacityRover::Configuration::Instance()
{
   return &DefaultInstance;
}
