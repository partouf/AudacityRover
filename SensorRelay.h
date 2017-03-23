#pragma once

#include <OpenALRF/Communication/SensorBusListener.h>
#include <OpenALRF/Sensors/SensorTypes.h>
#include <OpenALRF/Sensors/SensorDictionary.h>
#include <OpenALRF/Sensors/ProxySensorWithHistory.h>
#include <OpenALRF/Communication/SensorBusTypes.h>
#include "System/Configuration.h"
#include "Sensors/RelayedSensors.h"
#include <memory>

class SensorRelay: public OpenALRF::ISensor3DBusListener
{
public:
   SensorRelay();

   virtual void PowerOff() override;

   virtual void PowerOn() override;

   virtual bool IsPowered() const override;

   virtual bool NextValue(OpenALRF::Sensor3DData & AValue) override;

   virtual void NewSensorData(const OpenALRF::SensorBusData3D ABusData) override;

   AudacityRover::ProxySensorRelay *GetSensorRelayByID(const long AID);
   AudacityRover::ProxySensorRelay *GetSensorRelayByID(const OpenALRF::sensorid_t AID);

   std::vector<AudacityRover::ProxySensorRelay *> sensors;
};
