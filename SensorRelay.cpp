#include "SensorRelay.h"

SensorRelay::SensorRelay() : ISensor3DBusListener(OpenALRF::sensorUnknown) {
}

void SensorRelay::PowerOff()
{
}

void SensorRelay::PowerOn()
{
}

bool SensorRelay::IsPowered() const
{
   return true;
}

bool SensorRelay::NextValue(OpenALRF::Sensor3DData & AValue)
{
   return false;
}

void SensorRelay::NewSensorData(const OpenALRF::SensorBusData3D ABusData)
{
	AudacityRover::ProxySensorRelay *relay = GetSensorRelayByID(ABusData.ID);
	if (relay == nullptr)
	{
		relay = new AudacityRover::ProxySensorRelay(ABusData.ID);
		sensors.push_back(relay);
	}

	relay->NewSensorData(ABusData);
}

AudacityRover::ProxySensorRelay *SensorRelay::GetSensorRelayByID(const long AID)
{
	return GetSensorRelayByID(static_cast<OpenALRF::sensorid_t>(AID));
}

AudacityRover::ProxySensorRelay *SensorRelay::GetSensorRelayByID(const OpenALRF::sensorid_t AID)
{
	for (auto sensor : sensors)
	{
		if (sensor->GetIdentifier() == AID)
		{
			return sensor;
		}
	}

	return nullptr;
}
