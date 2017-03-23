#include "ProxySensorRelay.h"
#include <algorithm>

AudacityRover::ProxySensorRelay::ProxySensorRelay(OpenALRF::sensorid_t AIdentifier) : OpenALRF::ProxySensorWithHistory(AIdentifier)
{
}

void AudacityRover::ProxySensorRelay::NewSensorData(const OpenALRF::SensorBusData3D ABusData)
{
   OpenALRF::ProxySensorWithHistory::NewSensorData(ABusData);

   for (auto listener : Listeners)
   {
      listener->NewSensorData(ABusData);
   }
}

void AudacityRover::ProxySensorRelay::Subscribe(ProxySensorRelayListener * AListener)
{
   Listeners.push_back(AListener);
}

void AudacityRover::ProxySensorRelay::Unsubscribe(const ProxySensorRelayListener * AListener)
{
   auto Found = std::find(Listeners.begin(), Listeners.end(), AListener);
   if (Found != Listeners.end())
   {
      Listeners.erase(Found);
   }
}
