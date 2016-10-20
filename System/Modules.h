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
#include "../System/Configuration.h"
#include "../System/Logging.h"
#include "../System/WatchCat.h"

namespace AudacityRover
{
   class Modules
   {
   protected:
      std::vector<OpenALRF::ISensor *> Sensors;

      void LinkGoPiGo(Groundfloor::String &Computername, AudacityRover::Configuration * AConfiguration);
      void LinkAccelerometer(Groundfloor::String &Computername, AudacityRover::Configuration * AConfiguration);
      void LinkDummy2(Groundfloor::String &Computername, AudacityRover::Configuration * AConfiguration);
      void LinkDummy1(Groundfloor::String &Computername, AudacityRover::Configuration * AConfiguration);
      void LinkSystemTemp2(Groundfloor::String &Computername, AudacityRover::Configuration * AConfiguration);
      void LinkSystemTemp1(Groundfloor::String &Computername, AudacityRover::Configuration * AConfiguration);
      void LinkTemperature(Groundfloor::String &Computername, AudacityRover::Configuration * AConfiguration);
      void LinkMagnometer(Groundfloor::String &Computername, AudacityRover::Configuration * AConfiguration);
      void LinkGyroscope(Groundfloor::String &Computername, AudacityRover::Configuration * AConfiguration);

      std::string GetModuleInfoXML(const std::string AModuleName, const OpenALRF::IModule *AModule) const;
   public:
      Modules();
      ~Modules();

      static Modules *Instance();

      std::unique_ptr<AudacityRover::ILogging> Logging;

      std::unique_ptr<GoPiGo::IBoard> GoPiGoMainBoard;
      std::unique_ptr<GoPiGo::Wheels> Wheels;
      std::unique_ptr<GoPiGo::WheelEncodersWithErrorDetection> Encoders;

      std::unique_ptr<OpenALRF::IRemotePilot> Pilot;
      std::unique_ptr<OpenALRF::IAutoPilot> Auto;
      std::unique_ptr<OpenALRF::IMainCamera> MainCamera;
      std::unique_ptr<OpenALRF::ISystem> System;
      std::unique_ptr<OpenALRF::SensorBus> SensorBus;

      std::unique_ptr<OpenALRF::ICommandQueue> CommandQueue;

      std::unique_ptr<OpenALRF::ICommunication> Comm;

      std::unique_ptr<AudacityRover::WatchCat> Cat;
      std::unique_ptr<OpenALRF::ISensor3DBusListener> SensorTransmitter;
      std::unique_ptr<AudacityRover::SensorDataReceiver> SensorReceiver;

      std::unique_ptr<OpenALRF::ISensor> Accelerometer1;
      std::unique_ptr<OpenALRF::ISensor> Gyroscope1;
      std::unique_ptr<OpenALRF::ISensor> Magnometer1;
      std::unique_ptr<OpenALRF::ISensor> Temperature1;
      //OpenALRF::ISensor *Barometer1;
      //OpenALRF::ISensor *Humidity1;

      std::unique_ptr<OpenALRF::ISensor> SystemTemp1;
      std::unique_ptr<OpenALRF::ISensor> SystemTemp2;

      std::unique_ptr<OpenALRF::ISensor> Dummy1;
      std::unique_ptr<OpenALRF::ISensor> Dummy2;

      std::string GetStatusInfo() const;

      void SensorSweep();
   };
};
