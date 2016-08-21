#include "TemperatureSenseHAT.h"

AudacityRover::TemperatureSenseHAT::TemperatureSenseHAT() : OpenALRF::ISensor("Temperature")
{

}

void AudacityRover::TemperatureSenseHAT::PowerOff()
{
}

void AudacityRover::TemperatureSenseHAT::PowerOn()
{

}

bool AudacityRover::TemperatureSenseHAT::IsPowered()
{
   return false;
}

OpenALRF::Sensor3DData AudacityRover::TemperatureSenseHAT::NextValue()
{
   return OpenALRF::Sensor3DData();
}
