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

AudacityRover::Modules *ModulesInstance = nullptr;

AudacityRover::Modules::Modules()
{
   System = new AudacityRover::SystemAudacity();
   Pilot = new AudacityRover::RemotePilotGoPiGo();
   Auto = nullptr;// new AutoPilotDefault();
   MainCamera = new AudacityRover::CameraRaspi();
   SensorBus = new OpenALRF::SensorBus();
   CommandQueue = new AudacityRover::CommandQueue();
   Comm = new AudacityRover::CommunicationJumpropes();
   SensorTransmitter = new AudacityRover::SensorDataTransmitter();
   Cat = new AudacityRover::WatchCat();

   Groundfloor::String Computername;
   if (!Jumpropes::TryToGetComputerName(&Computername))
   {
      throw "unable to get computername, can't start without it";
   }

   auto Configuration = Configuration::Instance();

   if (Computername.match(Configuration->Accelerometer1.IPAddress))
   {
      //Accelerometer1 = new 
   }
   else
   {
      Accelerometer1 = new OpenALRF::ProxySensor("Accelerometer1", Configuration->Accelerometer1.ID);
   }

   if (Computername.match(Configuration->Gyroscope1.IPAddress))
   {
      //Gyroscope1 = new 
   }
   else
   {
      Gyroscope1 = new OpenALRF::ProxySensor("Gyroscope1", Configuration->Gyroscope1.ID);
   }

   if (Computername.match(Configuration->Magnometer1.IPAddress))
   {
      //Magnometer1 = new 
   }
   else
   {
      Magnometer1 = new OpenALRF::ProxySensor("Magnometer1", Configuration->Magnometer1.ID);
   }

   if (Computername.match(Configuration->Temperature1.IPAddress))
   {
      //Temperature1 = new 
   }
   else
   {
      Temperature1 = new OpenALRF::ProxySensor("Temperature1", Configuration->Temperature1.ID);
   }

   if (Computername.match(Configuration->Barometer1.IPAddress))
   {
      //Barometer1 = new 
   }
   else
   {
      Barometer1 = new OpenALRF::ProxySensor("Barometer1", Configuration->Barometer1.ID);
   }

   if (Computername.match(Configuration->Humidity1.IPAddress))
   {
      //Humidity1 = new 
   }
   else
   {
      Humidity1 = new OpenALRF::ProxySensor("Humidity1", Configuration->Humidity1.ID);
   }

   if (Computername.match(Configuration->WheelEncoderLeft.IPAddress))
   {
      //WheelEncoderLeft = new 
   }
   else
   {
      WheelEncoderLeft = new OpenALRF::ProxySensor("WheelEncoderLeft", Configuration->WheelEncoderLeft.ID);
   }

   if (Computername.match(Configuration->WheelEncoderRight.IPAddress))
   {
      //WheelEncoderRight = new 
   }
   else
   {
      WheelEncoderRight = new OpenALRF::ProxySensor("WheelEncoderRight", Configuration->WheelEncoderRight.ID);
   }
}

AudacityRover::Modules::~Modules()
{
   delete Cat;
   delete CommandQueue;
   //delete Auto;
   delete Pilot;
   delete MainCamera;
   delete SensorBus;
   delete System;
}

AudacityRover::Modules * AudacityRover::Modules::Instance()
{
   if (ModulesInstance == nullptr)
   {
      ModulesInstance = new Modules();
   }

   return ModulesInstance;
}
