#pragma once

#include <OpenALRF/Sensors/Sensors.h>

namespace AudacityRover
{

   class DummySensor : public OpenALRF::ISensor
   {
   protected:
      bool Powered;
   public:
      DummySensor(OpenALRF::sensorid_t AIdentifier);

      virtual void PowerOff() override;

      virtual void PowerOn() override;

      virtual bool IsPowered() override;

      virtual bool NextValue(OpenALRF::Sensor3DData & AValue) override;
   };
};
