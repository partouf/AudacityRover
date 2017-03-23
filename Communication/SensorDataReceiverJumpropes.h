#pragma once

#include <OpenALRF/Communication/SensorBusListener.h>
#include <OpenALRF/Communication/SensorBusTypes.h>
#include <Groundfloor/Molecules/String.h>
#include <Jumpropes/ThreadedConnection.h>
#include <Jumpropes/ClientSocket.h>
#include <Jumpropes/BaseSocket.h>

#include <OpenALRF/Common/Types.h>
#include <OpenALRF/Communication/SensorBus.h>

#include <vector>
#include <string>

#include "SensorDataReceiver.h"

using namespace std;

namespace AudacityRover
{
   class SensorDataConnection : public Jumpropes::ThreadedConnection
   {
   protected:
      OpenALRF::SensorBus *SensorBus;
      Groundfloor::String Buffer;
      OpenALRF::timestamp_t LastTimeDataReceived;
      OpenALRF::timestamp_t DeclaredDeadAfter;

      void ProcessBuffer();
      void StillHere();
   public:
      SensorDataConnection(Jumpropes::BaseSocket *aSocket, OpenALRF::SensorBus *aSensorBus);

      void newMessageReceived(const Groundfloor::String * sMessage) override;

      bool StillConsideredAlive() const;
   };

   class SensorDataReceiver: public ISensorDataReceiver
   {
   protected:
      Groundfloor::String Address;
      Jumpropes::ClientSocket Connection;
      OpenALRF::SensorBus *SensorBus;
      SensorDataConnection *Thread;

      bool IsConnected();
   public:
      SensorDataReceiver(const string AIPAddress, OpenALRF::SensorBus *aSensorBus);

      bool Connect();
      void Disconnect();

      void KeepAlive();
   };
};
