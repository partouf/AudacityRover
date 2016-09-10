#pragma once

#include <GoPiGo/Common/board.h>
#include <GoPiGo/Devices/encoders.h>
#include <GoPiGo/Devices/wheels.h>

#include <OpenALRF/Pilot/RemotePilot.h>
#include <OpenALRF/Pilot/AutoPilot.h>
#include <OpenALRF/Camera/MainCamera.h>
#include <OpenALRF/System/System.h>
#include <OpenALRF/Communication/SensorBus.h>
#include <OpenALRF/Communication/Communication.h>
#include "../Communication/SensorDataReceiver.h"

#include "../System/Logging.h"
#include "../System/WatchCat.h"

namespace AudacityRover
{
   class Modules
   {
   protected:
      std::vector<OpenALRF::ISensor *> Sensors;

      std::string GetModuleInfoXML(std::string AModuleName, OpenALRF::IModule *AModule);
   public:
      Modules();
      ~Modules();

      static Modules *Instance();

      AudacityRover::ILogging *Logging;

      GoPiGo::IBoard *GoPiGoMainBoard;
      GoPiGo::Wheels *Wheels;
      GoPiGo::WheelEncodersWithErrorDetection *Encoders;

      OpenALRF::IRemotePilot *Pilot;
      OpenALRF::IAutoPilot *Auto;
      OpenALRF::IMainCamera *MainCamera;
      OpenALRF::ISystem *System;
      OpenALRF::SensorBus *SensorBus;

      OpenALRF::ICommandQueue *CommandQueue;

      OpenALRF::ICommunication *Comm;

      AudacityRover::WatchCat *Cat;
      OpenALRF::ISensor3DBusListener *SensorTransmitter;
      AudacityRover::SensorDataReceiver *SensorReceiver;

      OpenALRF::ISensor *Accelerometer1;
      OpenALRF::ISensor *Gyroscope1;
      OpenALRF::ISensor *Magnometer1;
      OpenALRF::ISensor *Temperature1;
      //OpenALRF::ISensor *Barometer1;
      //OpenALRF::ISensor *Humidity1;

      OpenALRF::ISensor *Dummy1;
      OpenALRF::ISensor *Dummy2;

      std::string GetStatusInfo();

      void SensorSweep();
   };
};
