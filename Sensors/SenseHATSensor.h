#pragma once

#include <sense-hat.h>
#include <OpenALRF/Sensors/Sensors.h>
#include <OpenALRF/Sensors/SensorTypes.h>

namespace AudacityRover {
   class SenseHATSensor : public OpenALRF::ISensor
   {
   protected:
      SenseHAT::ISenseHAT *HAT;
   public:
      SenseHATSensor(OpenALRF::sensorid_t AIdentifier);

      virtual void PowerOff() override;
      virtual void PowerOn() override;
      virtual bool IsPowered() const override;

      virtual std::string GetStatusInfo() override;
   };
};
