#pragma once

#include <OpenALRF/Sensors/Sensors.h>

namespace AudacityRover {
   class TemperatureSenseHAT : public OpenALRF::ISensor
   {
      TemperatureSenseHAT();

      void PowerOff() override;
      void PowerOn() override;
      bool IsPowered() override;

      OpenALRF::Sensor3DData NextValue() override;
   };
};
