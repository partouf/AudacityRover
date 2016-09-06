#pragma once

#include <OpenALRF/Communication/SensorBusListener.h>
#include <OpenALRF/Communication/SensorBusTypes.h>
#include <Groundfloor/Molecules/String.h>
#include <Jumpropes/ThreadedConnection.h>
#include <Jumpropes/ClientSocket.h>
#include <Jumpropes/BaseSocket.h>

#include <vector>
#include <string>

using namespace std;

namespace AudacityRover
{
   class SensorDataConnection : public Jumpropes::ThreadedConnection
   {
   protected:
   public:
      SensorDataConnection(Jumpropes::BaseSocket *aSocket);

      void newMessageReceived(const Groundfloor::String * sMessage) override;
   };

   class SensorDataReceiver
   {
   protected:
      Groundfloor::String Address;
      Jumpropes::ClientSocket Connection;
      SensorDataConnection *Thread;
   public:
      SensorDataReceiver(const string AIPAddress);

      bool Connect();
      void Disconnect();

      bool IsConnected();
   };
};
