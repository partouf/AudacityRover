#include "Configuration.h"

AudacityRover::Configuration DefaultInstance;

AudacityRover::Configuration::Configuration()
{
   CommandServerPort = 13666;
   SensorServerPort = 13777;

   Accelerometer1       = { 11, "pi1wifi" };
   Gyroscope1           = { 12, "pi1wifi" };
   Magnometer1          = { 13, "pi1wifi" };

   Temperature1         = { 14, "pi1wifi" };
   Barometer1           = { 15, "pi1wifi" };
   Humidity1            = { 16, "pi1wifi" };

   WheelEncoderLeft     = { 21, "pi2wifi" };
   WheelEncoderRight    = { 22, "pi2wifi" };

   CameraFrontLeft = "pi2wifi";
   CameraFrontRight = "pi1wifi";
}

AudacityRover::Configuration * AudacityRover::Configuration::Instance()
{
   return &DefaultInstance;
}
