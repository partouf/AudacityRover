#include "Modules.h"

#include "../System/SystemAudacity.h"
#include "../Pilot/RemotePilotGoPiGo.h"
#include "../Camera/CameraRaspi.h"
#include "../Command/CommandQueueDefault.h"
#include "../Communication/CommunicationJumpropes.h"
#include "../Communication/SensorDataTransmitter.h"
#include "../Communication/SensorDataReceiver.h"
#include <Jumpropes/Functions.h>
#include "../System/Configuration.h"
#include <OpenALRF/Sensors/ProxySensor.h>
#include "../Sensors/AccelerometerSenseHAT.h"
#include "../Sensors/MagnometerSenseHAT.h"
#include "../Sensors/TemperatureSenseHAT.h"
#include "../Sensors/GyroscopeSenseHAT.h"
#include "../Sensors/DummySensor.h"
#include <stdexcept>

#ifdef USENULLBOARD
#include <GoPiGo/OS/null.h>
#else
#include <GoPiGo/OS/linux.h>
#endif

AudacityRover::Modules *ModulesInstance = nullptr;

std::string AudacityRover::Modules::GetModuleInfoXML(std::string AModuleName, OpenALRF::IModule * AModule)
{
   std::string info;
   info += "<";
   info += AModuleName;

   if (AModule != nullptr)
   {
      info += ">";
      info += AModule->GetStatusInfo();
      info += "</";
      info += AModuleName;
   }
   else
   {
      info += " /";
   }
   info += ">";

   return info;
}

AudacityRover::Modules::Modules()
{
   System = new AudacityRover::SystemAudacity();
   MainCamera = new AudacityRover::CameraRaspi();
   SensorBus = new OpenALRF::SensorBus();
   CommandQueue = new AudacityRover::CommandQueue();
   Comm = new AudacityRover::CommunicationJumpropes();
   SensorTransmitter = new AudacityRover::SensorDataTransmitter();
   Cat = new AudacityRover::WatchCat();

   Groundfloor::String Computername;
   if (!Jumpropes::TryToGetComputerName(&Computername))
   {
      throw new std::runtime_error("unable to get computername, can't start without it");
   }

   auto Configuration = Configuration::Instance();

   if (Computername.match(Configuration->GoPiGo))
   {
      #ifdef USENULLBOARD
      GoPiGoMainBoard = new GoPiGo::NullBoard(1);
      #else
      GoPiGoMainBoard = new GoPiGo::LinuxBoard(1);
      #endif

      if (!GoPiGoMainBoard->Connect())
      {
         throw new std::runtime_error(GoPiGoMainBoard->LastKnownError.c_str());
      }

      Wheels = new GoPiGo::Wheels(GoPiGoMainBoard);
      Encoders = new GoPiGo::WheelEncodersWithErrorDetection(GoPiGoMainBoard);

      Pilot = new AudacityRover::RemotePilotGoPiGo();
      Auto = nullptr;// new AutoPilotDefault();
   }

   if (Computername.match(Configuration->Accelerometer1.IPAddress))
   {
      Accelerometer1 = new AudacityRover::AccelerometerSenseHAT(Configuration->Accelerometer1.ID);
   }
   else
   {
      Accelerometer1 = new OpenALRF::ProxySensor(Configuration->Accelerometer1.ID);
   }
   Sensors.push_back(Accelerometer1);

   if (Computername.match(Configuration->Gyroscope1.IPAddress))
   {
      Gyroscope1 = new AudacityRover::GyroscopeSenseHAT(Configuration->Gyroscope1.ID);
   }
   else
   {
      Gyroscope1 = new OpenALRF::ProxySensor(Configuration->Gyroscope1.ID);
   }
   Sensors.push_back(Gyroscope1);

   if (Computername.match(Configuration->Magnometer1.IPAddress))
   {
      Magnometer1 = new AudacityRover::MagnometerSenseHAT(Configuration->Magnometer1.ID);
   }
   else
   {
      Magnometer1 = new OpenALRF::ProxySensor(Configuration->Magnometer1.ID);
   }
   Sensors.push_back(Magnometer1);

   if (Computername.match(Configuration->Temperature1.IPAddress))
   {
      Temperature1 = new AudacityRover::TemperatureSenseHAT(Configuration->Temperature1.ID);
   }
   else
   {
      Temperature1 = new OpenALRF::ProxySensor(Configuration->Temperature1.ID);
   }
   Sensors.push_back(Temperature1);

   if (Computername.match(Configuration->Barometer1.IPAddress))
   {
      //Barometer1 = new 
   }
   else
   {
      Barometer1 = new OpenALRF::ProxySensor(Configuration->Barometer1.ID);
   }

   if (Computername.match(Configuration->Humidity1.IPAddress))
   {
      //Humidity1 = new 
   }
   else
   {
      Humidity1 = new OpenALRF::ProxySensor(Configuration->Humidity1.ID);
   }

   if (Computername.match(Configuration->Dummy1.IPAddress))
   {
      Dummy1 = new DummySensor(Configuration->Dummy1.ID);
   }
   else
   {
      Dummy1 = new OpenALRF::ProxySensor(Configuration->Dummy1.ID);
   }
   Sensors.push_back(Dummy1);

   if (Computername.match(Configuration->Dummy2.IPAddress))
   {
      Dummy2 = new DummySensor(Configuration->Dummy2.ID);
   }
   else
   {
      Dummy2 = new OpenALRF::ProxySensor(Configuration->Dummy2.ID);
   }
   Sensors.push_back(Dummy2);
}

AudacityRover::Modules::~Modules()
{
   Sensors.clear();

   delete Dummy1;
   delete Dummy2;

   delete Humidity1;
   delete Barometer1;
   delete Temperature1;
   delete Magnometer1;
   delete Gyroscope1;
   delete Accelerometer1;

   delete Cat;
   delete CommandQueue;
   //delete Auto;
   delete Pilot;
   delete MainCamera;
   delete SensorBus;
   delete System;
   delete GoPiGoMainBoard;
}

AudacityRover::Modules * AudacityRover::Modules::Instance()
{
   if (ModulesInstance == nullptr)
   {
      ModulesInstance = new Modules();
   }

   return ModulesInstance;
}

std::string AudacityRover::Modules::GetStatusInfo()
{
   std::string info;
   info += GetModuleInfoXML("System", System);
   info += GetModuleInfoXML("Pilot", Pilot);
   info += GetModuleInfoXML("Auto", Auto);
   info += GetModuleInfoXML("MainCamera", MainCamera);
   info += GetModuleInfoXML("SensorBus", SensorBus);
   info += GetModuleInfoXML("CommandQueue", CommandQueue);
   info += GetModuleInfoXML("Comm", Comm);
   info += GetModuleInfoXML("Accelerometer1", Accelerometer1);
   info += GetModuleInfoXML("Gyroscope1", Gyroscope1);
   info += GetModuleInfoXML("Magnometer1", Magnometer1);
   info += GetModuleInfoXML("Temperature1", Temperature1);
   info += GetModuleInfoXML("Barometer1", Barometer1);
   info += GetModuleInfoXML("Humidity1", Humidity1);
   info += GetModuleInfoXML("Dummy1", Dummy1);
   info += GetModuleInfoXML("Dummy2", Dummy2);

   return info;
}

void AudacityRover::Modules::SensorSweep()
{
   for (auto sensor : Sensors) 
   {
      OpenALRF::Sensor3DData data;
      if (sensor->NextValue(data))
      {
         SensorBus->Broadcast(sensor->GetIdentifier(), sensor->GetSensorType(), sensor->GetUsedUnit(), data);
      }
   }
}
