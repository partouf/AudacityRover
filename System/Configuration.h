#pragma once

#include <cstdint>
#include <OpenALRF/Communication/SensorBusTypes.h>
#include <OpenALRF/Common/Types.h>

namespace AudacityRover
{
   class Configuration
   {
   public:
      Configuration();

      int16_t SensorServerPort;
      int16_t CommandServerPort;

      std::string GoPiGo;
      std::string RTIMU;

      OpenALRF::SensorConfig Accelerometer1;
      OpenALRF::SensorConfig Gyroscope1;
      OpenALRF::SensorConfig Magnometer1;
      OpenALRF::SensorConfig Temperature1;
      OpenALRF::SensorConfig Barometer1;
      OpenALRF::SensorConfig Humidity1;
      OpenALRF::SensorConfig Compass1;

      OpenALRF::SensorConfig Dummy1;
      OpenALRF::SensorConfig Dummy2;

      OpenALRF::SensorConfig SystemTemp1;
      OpenALRF::SensorConfig SystemTemp2;

      std::string CameraFrontLeft;
      std::string CameraFrontRight;

      int32_t SecondsWhenToDeclareConnectionDead;

      static Configuration *Instance();
   };
};
