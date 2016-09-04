#include "DummySensor.h"
#include <OpenALRF/Common/Timing.h>

AudacityRover::DummySensor::DummySensor(OpenALRF::sensorid_t AIdentifier) : OpenALRF::ISensor(AIdentifier)
{
   Type = OpenALRF::sensorUnknown;
   UsedUnit = OpenALRF::unitUnknown;

   PowerOn();
}

void AudacityRover::DummySensor::PowerOff()
{
   Powered = false;
}

void AudacityRover::DummySensor::PowerOn()
{
   Powered = true;
}

bool AudacityRover::DummySensor::IsPowered()
{
   return Powered;
}

bool AudacityRover::DummySensor::NextValue(OpenALRF::Sensor3DData & AValue)
{
   AValue.Timestamp = OpenALRF::GetCurrentTimestamp();
   AValue.Data1 = 1;
   AValue.Data2 = 2;
   AValue.Data3 = 3;

   LatestSensorData = AValue;

   return true;
}
