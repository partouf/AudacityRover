#pragma once

#include <OpenALRF/Pilot/RemotePilot.h>
#include <OpenALRF/Pilot/AutoPilot.h>
#include <OpenALRF/Camera/MainCamera.h>
#include <OpenALRF/System/System.h>
#include <OpenALRF/Communication/SensorBus.h>
#include <OpenALRF/Communication/Communication.h>

#include "../System/WatchCat.h"

namespace AudacityRover
{
   class Modules
   {
   public:
      Modules();
      ~Modules();

      static Modules *Instance();

      OpenALRF::IRemotePilot *Pilot;
      OpenALRF::IAutoPilot *Auto;
      OpenALRF::IMainCamera *MainCamera;
      OpenALRF::ISystem *System;
      OpenALRF::SensorBus *SensorBus;

      OpenALRF::ICommandQueue *CommandQueue;

      OpenALRF::ICommunication *Comm;

      AudacityRover::WatchCat *Cat;
      OpenALRF::ISensor3DBusListener *SensorTransmitter;

      OpenALRF::ISensor *Accelerometer1;
      OpenALRF::ISensor *Gyroscope1;
      OpenALRF::ISensor *Magnometer1;
      OpenALRF::ISensor *Temperature1;
      OpenALRF::ISensor *Barometer1;
      OpenALRF::ISensor *Humidity1;

      OpenALRF::ISensor *WheelEncoderLeft;
      OpenALRF::ISensor *WheelEncoderRight;
   };
};
