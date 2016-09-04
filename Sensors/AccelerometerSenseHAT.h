#pragma once

#include "SenseHATSensor.h"

namespace AudacityRover {
   class AccelerometerSenseHAT : public AudacityRover::SenseHATSensor
   {
   public:
      AccelerometerSenseHAT(OpenALRF::sensorid_t AIdentifier);

      bool NextValue(OpenALRF::Sensor3DData &AValue) override;
   };
};
