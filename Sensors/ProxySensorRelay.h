#pragma once

#include <OpenALRF/Sensors/ProxySensorWithHistory.h>
#include <vector>

namespace AudacityRover
{
   class ProxySensorRelayListener
   {
   public:
      virtual void NewSensorData(const OpenALRF::SensorBusData3D ABusData) = 0;
   };

   class ProxySensorRelay: public OpenALRF::ProxySensorWithHistory
   {
   protected:
      std::vector<ProxySensorRelayListener *> Listeners;
   public:
      ProxySensorRelay(OpenALRF::sensorid_t AIdentifier);

      virtual void NewSensorData(const OpenALRF::SensorBusData3D ABusData) override;

      void Subscribe(ProxySensorRelayListener *AListener);
      void Unsubscribe(const ProxySensorRelayListener *AListener);
   };
}
