#include "SenseHATSensor.h"

#include <Groundfloor/Atoms/Defines.h>
#include <Groundfloor/Materials/Functions.h>
#include <Groundfloor/Bookshelfs/BValue.h>

#ifdef GF_OS_WIN32
#include <sense-hat-sim.h>
#else
#include <sense-hat-linux.h>
#endif


AudacityRover::SenseHATSensor::SenseHATSensor(OpenALRF::sensorid_t AIdentifier) : OpenALRF::ISensor(AIdentifier)
{
   HAT = nullptr;

   PowerOn();
}

void AudacityRover::SenseHATSensor::PowerOff()
{
   HAT = nullptr;
}

void AudacityRover::SenseHATSensor::PowerOn()
{
#ifdef GF_OS_WIN32
   HAT = SenseHAT::SenseHATSim::Instance();
#else
   HAT = SenseHAT::SenseHATLinux::Instance();
#endif
}

bool AudacityRover::SenseHATSensor::IsPowered() const
{
   return (HAT != nullptr);
}

std::string AudacityRover::SenseHATSensor::GetStatusInfo() const
{
   std::string info = ISensor::GetStatusInfo();

   if (IsPowered())
   {
      info += "<Power>On</Power>";
   }
   else
   {
      info += "<Power>Off</Power>";
   }

   return info;
}
