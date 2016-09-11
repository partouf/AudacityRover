#pragma once

#include <OpenALRF/Communication/SensorBusListener.h>
#include <OpenALRF/Communication/SensorBusTypes.h>
#include <Jumpropes/ThreadedServer.h>
#include <Jumpropes/BaseSocket.h>
#include <Groundfloor/Molecules/String.h>
#include <vector>
#include <string>

using namespace std;

namespace AudacityRover
{
   class SensorDataServer : public Jumpropes::ThreadedServer
   {
   protected:
      vector<Jumpropes::BaseSocket *> Clients;

      void CleanupDisconnectedClients();

      Groundfloor::String SensorDataToString(const OpenALRF::SensorBusData3D ABusData);
   public:
      SensorDataServer();

      void newClientConnection(Jumpropes::BaseSocket * aClient) override;

      void BroadcastToClients(const OpenALRF::SensorBusData3D ABusData);
   };

   class SensorDataTransmitter : public OpenALRF::ISensor3DBusListener
   {
   protected:
      SensorDataServer Server;
   public:
      SensorDataTransmitter();

      void NewSensorData(const OpenALRF::SensorBusData3D ABusData) override;

      // Inherited via ISensor3DBusListener
      virtual void PowerOff() override;
      virtual void PowerOn() override;
      virtual bool IsPowered() const override;
      virtual bool NextValue(OpenALRF::Sensor3DData & AValue) override;
   };
};
