#include "Modules.h"

#include "../System/SystemAudacity.h"
#include "../Pilot/RemotePilotGoPiGo.h"
#include "../Camera/CameraRaspi.h"
#include "../Command/CommandQueueDefault.h"
#include "../Communication/CommunicationJumpropes.h"
#include "../Communication/SensorDataTransmitter.h"
#include "../Communication/SensorDataReceiver.h"
#include <Jumpropes/Functions.h>
#include <OpenALRF/Sensors/ProxySensor.h>
#include "../Sensors/AccelerometerSenseHAT.h"
#include "../Sensors/MagnometerSenseHAT.h"
#include "../Sensors/TemperatureSenseHAT.h"
#include "../Sensors/GyroscopeSenseHAT.h"
#include "../Sensors/DummySensor.h"
#include "../Sensors/SystemTempSensor.h"
#include <stdexcept>

#ifdef USENULLBOARD
#include <GoPiGo/OS/null.h>
#else
#include <GoPiGo/OS/linux.h>
#endif

std::unique_ptr<AudacityRover::Modules> ModulesInstance = nullptr;

std::string AudacityRover::Modules::GetModuleInfoXML(const std::string AModuleName, const OpenALRF::IModule *AModule) const
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
   Logging = std::make_unique<AudacityRover::Logging>();

   System = std::make_unique<AudacityRover::SystemAudacity>();
   MainCamera = std::make_unique<AudacityRover::CameraRaspi>();
   SensorBus = std::make_unique<OpenALRF::SensorBus>();
   CommandQueue = std::make_unique<AudacityRover::CommandQueue>();
   Comm = std::make_unique<AudacityRover::CommunicationJumpropes>();

   SensorTransmitter = std::make_unique<AudacityRover::SensorDataTransmitter>();
   SensorBus->Subscribe(SensorTransmitter.get());

   Cat = std::make_unique<AudacityRover::WatchCat>();

   Groundfloor::String Computername;
   if (!Jumpropes::TryToGetComputerName(&Computername))
   {
      throw new std::runtime_error("unable to get computername, can't start without it");
   }

   auto Configuration = Configuration::Instance();

   LinkGoPiGo(Computername, Configuration);
   LinkAccelerometer(Computername, Configuration);
   LinkGyroscope(Computername, Configuration);
   LinkMagnometer(Computername, Configuration);
   LinkTemperature(Computername, Configuration);
   LinkSystemTemp1(Computername, Configuration);
   LinkSystemTemp2(Computername, Configuration);
   LinkDummy1(Computername, Configuration);
   LinkDummy2(Computername, Configuration);
}

void AudacityRover::Modules::LinkGoPiGo(Groundfloor::String &Computername, AudacityRover::Configuration * AConfiguration)
{
   if (Computername.match(AConfiguration->GoPiGo))
   {
#ifdef USENULLBOARD
      GoPiGoMainBoard = std::make_unique<GoPiGo::NullBoard>(1);
#else
      GoPiGoMainBoard = std::make_unique<GoPiGo::LinuxBoard>(1);
#endif

      if (!GoPiGoMainBoard->Connect())
      {
         throw new std::runtime_error(GoPiGoMainBoard->LastKnownError.c_str());
      }

      Wheels = std::make_unique<GoPiGo::Wheels>(GoPiGoMainBoard.get());
      Encoders = std::make_unique<GoPiGo::WheelEncodersWithErrorDetection>(GoPiGoMainBoard.get());

      Pilot = std::make_unique<AudacityRover::RemotePilotGoPiGo>();
      Auto = nullptr;// new AutoPilotDefault();

      SensorReceiver = std::make_unique<AudacityRover::SensorDataReceiver>(AConfiguration->RTIMU);
   }
   else
   {
      GoPiGoMainBoard = nullptr;
      Wheels = nullptr;
      Encoders = nullptr;
      Pilot = nullptr;
      Auto = nullptr;

      SensorReceiver = std::make_unique<AudacityRover::SensorDataReceiver>(AConfiguration->GoPiGo);
   }
}

void AudacityRover::Modules::LinkAccelerometer(Groundfloor::String &Computername, AudacityRover::Configuration * AConfiguration)
{
   if (Computername.match(AConfiguration->Accelerometer1.IPAddress))
   {
      Accelerometer1 = std::make_unique<AudacityRover::AccelerometerSenseHAT>(AConfiguration->Accelerometer1.ID);
   }
   else
   {
      Accelerometer1 = std::make_unique<OpenALRF::ProxySensor>(AConfiguration->Accelerometer1.ID);
      SensorBus->Subscribe(reinterpret_cast<OpenALRF::ISensor3DBusListener *>(Accelerometer1.get()));
   }
   Sensors.push_back(Accelerometer1.get());
}

void AudacityRover::Modules::LinkDummy2(Groundfloor::String &Computername, AudacityRover::Configuration * AConfiguration)
{
   if (Computername.match(AConfiguration->Dummy2.IPAddress))
   {
      Dummy2 = std::make_unique<AudacityRover::DummySensor>(AConfiguration->Dummy2.ID);
   }
   else
   {
      Dummy2 = std::make_unique<OpenALRF::ProxySensor>(AConfiguration->Dummy2.ID);
      SensorBus->Subscribe(reinterpret_cast<OpenALRF::ISensor3DBusListener *>(Dummy2.get()));
   }
   Sensors.push_back(Dummy2.get());
}

void AudacityRover::Modules::LinkDummy1(Groundfloor::String &Computername, AudacityRover::Configuration * AConfiguration)
{
   if (Computername.match(AConfiguration->Dummy1.IPAddress))
   {
      Dummy1 = std::make_unique<AudacityRover::DummySensor>(AConfiguration->Dummy1.ID);
   }
   else
   {
      Dummy1 = std::make_unique<OpenALRF::ProxySensor>(AConfiguration->Dummy1.ID);
      SensorBus->Subscribe(reinterpret_cast<OpenALRF::ISensor3DBusListener *>(Dummy1.get()));
   }
   Sensors.push_back(Dummy1.get());
}

void AudacityRover::Modules::LinkSystemTemp2(Groundfloor::String &Computername, AudacityRover::Configuration * AConfiguration)
{
   if (Computername.match(AConfiguration->SystemTemp2.IPAddress))
   {
      SystemTemp2 = std::make_unique<AudacityRover::SystemTempSensor>(AConfiguration->SystemTemp2.ID);
   }
   else
   {
      SystemTemp2 = std::make_unique<OpenALRF::ProxySensor>(AConfiguration->SystemTemp2.ID);
      SensorBus->Subscribe(reinterpret_cast<OpenALRF::ISensor3DBusListener *>(SystemTemp2.get()));
   }
   Sensors.push_back(SystemTemp2.get());
}

void AudacityRover::Modules::LinkSystemTemp1(Groundfloor::String &Computername, AudacityRover::Configuration * AConfiguration)
{
   if (Computername.match(AConfiguration->SystemTemp1.IPAddress))
   {
      SystemTemp1 = std::make_unique<AudacityRover::SystemTempSensor>(AConfiguration->SystemTemp1.ID);
   }
   else
   {
      SystemTemp1 = std::make_unique<OpenALRF::ProxySensor>(AConfiguration->SystemTemp1.ID);
      SensorBus->Subscribe(reinterpret_cast<OpenALRF::ISensor3DBusListener *>(SystemTemp1.get()));
   }
   Sensors.push_back(SystemTemp1.get());
}

void AudacityRover::Modules::LinkTemperature(Groundfloor::String &Computername, AudacityRover::Configuration * AConfiguration)
{
   if (Computername.match(AConfiguration->Temperature1.IPAddress))
   {
      Temperature1 = std::make_unique<AudacityRover::TemperatureSenseHAT>(AConfiguration->Temperature1.ID);
   }
   else
   {
      Temperature1 = std::make_unique<OpenALRF::ProxySensor>(AConfiguration->Temperature1.ID);
      SensorBus->Subscribe(reinterpret_cast<OpenALRF::ISensor3DBusListener *>(Temperature1.get()));
   }
   Sensors.push_back(Temperature1.get());
}

void AudacityRover::Modules::LinkMagnometer(Groundfloor::String &Computername, AudacityRover::Configuration * AConfiguration)
{
   if (Computername.match(AConfiguration->Magnometer1.IPAddress))
   {
      Magnometer1 = std::make_unique<AudacityRover::MagnometerSenseHAT>(AConfiguration->Magnometer1.ID);
   }
   else
   {
      Magnometer1 = std::make_unique<OpenALRF::ProxySensor>(AConfiguration->Magnometer1.ID);
      SensorBus->Subscribe(reinterpret_cast<OpenALRF::ISensor3DBusListener *>(Magnometer1.get()));
   }
   Sensors.push_back(Magnometer1.get());
}

void AudacityRover::Modules::LinkGyroscope(Groundfloor::String &Computername, AudacityRover::Configuration * AConfiguration)
{
   if (Computername.match(AConfiguration->Gyroscope1.IPAddress))
   {
      Gyroscope1 = std::make_unique<AudacityRover::GyroscopeSenseHAT>(AConfiguration->Gyroscope1.ID);
   }
   else
   {
      Gyroscope1 = std::make_unique<OpenALRF::ProxySensor>(AConfiguration->Gyroscope1.ID);
      SensorBus->Subscribe(reinterpret_cast<OpenALRF::ISensor3DBusListener *>(Gyroscope1.get()));
   }
   Sensors.push_back(Gyroscope1.get());
}

AudacityRover::Modules::~Modules()
{
   LOGCUSTOM("PANIC!");

   Sensors.clear();
   SensorBus->ClearSubscribers();
   SensorReceiver->Disconnect();
}

AudacityRover::Modules * AudacityRover::Modules::Instance()
{
   if (ModulesInstance == nullptr)
   {
      ModulesInstance = std::make_unique<Modules>();
   }

   return ModulesInstance.get();
}

std::string AudacityRover::Modules::GetStatusInfo() const
{
   std::string info;
   info += GetModuleInfoXML("System", System.get());
   info += GetModuleInfoXML("Pilot", Pilot.get());
   info += GetModuleInfoXML("Auto", Auto.get());
   info += GetModuleInfoXML("MainCamera", MainCamera.get());
   info += GetModuleInfoXML("SensorBus", SensorBus.get());
   info += GetModuleInfoXML("CommandQueue", CommandQueue.get());
   info += GetModuleInfoXML("Comm", Comm.get());
   info += GetModuleInfoXML("Accelerometer1", Accelerometer1.get());
   info += GetModuleInfoXML("Gyroscope1", Gyroscope1.get());
   info += GetModuleInfoXML("Magnometer1", Magnometer1.get());
   info += GetModuleInfoXML("Temperature1", Temperature1.get());
//   info += GetModuleInfoXML("Barometer1", Barometer1);
//   info += GetModuleInfoXML("Humidity1", Humidity1);
   info += GetModuleInfoXML("SystemTemp1", SystemTemp1.get());
   info += GetModuleInfoXML("SystemTemp2", SystemTemp2.get());
   info += GetModuleInfoXML("Dummy1", Dummy1.get());
   info += GetModuleInfoXML("Dummy2", Dummy2.get());

   return info;
}

void AudacityRover::Modules::SensorSweep()
{
   LOGFUNCTION();

   SensorReceiver->KeepAlive();

   for (auto sensor : Sensors) 
   {
      LOGCUSTOM("Sensing");
      if (sensor == nullptr)
      {
         LOGCUSTOM("nullptr");
         continue;
      }

      if (sensor->GetOrigin() == OpenALRF::sensoriLocal)
      {
         LOGCUSTOM("Local");
         OpenALRF::Sensor3DData data;
         if (sensor->NextValue(data))
         {
            SensorBus->Broadcast(sensor->GetIdentifier(), sensor->GetSensorType(), sensor->GetUsedUnit(), data);
         }
      }
   }
}
