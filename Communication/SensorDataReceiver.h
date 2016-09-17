#pragma once

#include <OpenALRF/Communication/SensorBusListener.h>
#include <OpenALRF/Communication/SensorBusTypes.h>
#include <Groundfloor/Molecules/String.h>
#include <Jumpropes/ThreadedConnection.h>
#include <Jumpropes/ClientSocket.h>
#include <Jumpropes/BaseSocket.h>

#include <OpenALRF/Common/Types.h>

#include <vector>
#include <string>

using namespace std;

namespace AudacityRover
{
   class SensorDataConnection : public Jumpropes::ThreadedConnection
   {
   protected:
      Groundfloor::String Buffer;
      OpenALRF::timestamp_t LastTimeDataReceived;
      OpenALRF::timestamp_t DeclaredDeadAfter;

      void ProcessBuffer();
      void StillHere();
   public:
      SensorDataConnection(Jumpropes::BaseSocket *aSocket);

      void newMessageReceived(const Groundfloor::String * sMessage) override;

      bool StillConsideredAlive() const;
   };

   class SensorDataReceiver
   {
   protected:
      Groundfloor::String Address;
      Jumpropes::ClientSocket Connection;
      SensorDataConnection *Thread;

      bool IsConnected();
   public:
      SensorDataReceiver(const string AIPAddress);

      bool Connect();
      void Disconnect();

      void KeepAlive();
   };
};
