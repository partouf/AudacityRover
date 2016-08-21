#pragma once

#include <cstdint>
#include <OpenALRF/Communication/SensorBusTypes.h>

namespace AudacityRover
{
   class Configuration
   {
   public:
      Configuration();

      int16_t SensorServerPort;
      int16_t CommandServerPort;

      OpenALRF::SensorConfig Accelerometer1;
      OpenALRF::SensorConfig Gyroscope1;
      OpenALRF::SensorConfig Magnometer1;
      OpenALRF::SensorConfig Temperature1;
      OpenALRF::SensorConfig Barometer1;
      OpenALRF::SensorConfig Humidity1;
      OpenALRF::SensorConfig WheelEncoderLeft;
      OpenALRF::SensorConfig WheelEncoderRight;

      std::string CameraFrontLeft;
      std::string CameraFrontRight;

      static Configuration *Instance();
   };
};
