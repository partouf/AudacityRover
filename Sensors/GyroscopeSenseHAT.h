#pragma once

#include "SenseHATSensor.h"

namespace AudacityRover {
   class GyroscopeSenseHAT : public AudacityRover::SenseHATSensor
   {
   public:
      GyroscopeSenseHAT(OpenALRF::sensorid_t AIdentifier);

      bool NextValue(OpenALRF::Sensor3DData &AValue) override;
   };
};
