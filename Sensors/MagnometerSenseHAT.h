#pragma once

#include "SenseHATSensor.h"

namespace AudacityRover {
   class MagnometerSenseHAT : public AudacityRover::SenseHATSensor
   {
   public:
      MagnometerSenseHAT(OpenALRF::sensorid_t AIdentifier);

      bool NextValue(OpenALRF::Sensor3DData &AValue) override;
   };
};
