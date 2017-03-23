#pragma once

#include "ProxySensorRelay.h"

namespace AudacityRover
{
   class AccelerometerRelay : public ProxySensorRelay
   {
   public:
      AccelerometerRelay(OpenALRF::sensorid_t AIdentifier) : ProxySensorRelay(AIdentifier)
      {
         UsedUnit = OpenALRF::unitMetersPerSecond;
         Type = OpenALRF::sensorAccelerometer;
      }
   };

   class GyroscopeRelay : public ProxySensorRelay
   {
   public:
      GyroscopeRelay(OpenALRF::sensorid_t AIdentifier) : ProxySensorRelay(AIdentifier)
      {
         UsedUnit = OpenALRF::unitDegrees;
         Type = OpenALRF::sensorGyroscope;
      }
   };

   class MagnometerRelay : public ProxySensorRelay
   {
   public:
      MagnometerRelay(OpenALRF::sensorid_t AIdentifier) : ProxySensorRelay(AIdentifier)
      {
         UsedUnit = OpenALRF::unitDegrees;
         Type = OpenALRF::sensorMagnetometer;
      }
   };

   class TemperatureRelay : public ProxySensorRelay
   {
   public:
      TemperatureRelay(OpenALRF::sensorid_t AIdentifier) : ProxySensorRelay(AIdentifier)
      {
         UsedUnit = OpenALRF::unitCelcius;
         Type = OpenALRF::sensorTemperature;
      }
   };

   class SystemTempRelay : public ProxySensorRelay
   {
   public:
      SystemTempRelay(OpenALRF::sensorid_t AIdentifier) : ProxySensorRelay(AIdentifier)
      {
         UsedUnit = OpenALRF::unitCelcius;
         Type = OpenALRF::sensorTemperature;
      }
   };
}
