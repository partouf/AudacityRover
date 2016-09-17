#pragma once

#include <OpenALRF/Sensors/Sensors.h>

namespace AudacityRover
{
   class SystemTempSensor: public OpenALRF::ISensor
   {
   protected:

   public:
      SystemTempSensor(OpenALRF::sensorid_t AIdentifier);

      // Inherited via ISensor
      virtual void PowerOff() override;

      virtual void PowerOn() override;

      virtual bool IsPowered() const override;

      virtual bool NextValue(OpenALRF::Sensor3DData & AValue) override;

   };
};
