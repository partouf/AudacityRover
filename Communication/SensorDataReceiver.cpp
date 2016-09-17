#include "SensorDataReceiver.h"

#include <OpenALRF/Communication/SensorBusTypes.h>
#include <Jumpropes/Functions.h>
#include "../System/Configuration.h"
#include "../System/Modules.h"
#include <iostream>
#include <OpenALRF/Common/Timing.h>

AudacityRover::SensorDataReceiver::SensorDataReceiver(const string AIPAddress)
{
   if (AIPAddress.find('.') != -1)
   {
      Address.setValue(AIPAddress);
   }
   else
   {
      Address.setValue(AIPAddress + ".local");
   }
}

bool AudacityRover::SensorDataReceiver::Connect()
{
   Disconnect();

   Jumpropes::LookupObject Lookup;
   if (Jumpropes::JRresolveIP(&Address, &Lookup))
   {
      Connection.getRemoteAddress()->setValue(Lookup.getAddress(0));
   }
   else
   {
      std::cerr << "Cannot resolve " << Address.getValue() << std::endl;
      return false;
   }

   Connection.remotePort.set(Configuration::Instance()->SensorServerPort);

   if (Connection.connect())
   {
      Thread = new SensorDataConnection(&Connection);
      Thread->start();

      return true;
   }
   else
   {
      std::cerr << "Cannot connect to sensordata port on " << Address.getValue() << std::endl;
      return false;
   }
}

void AudacityRover::SensorDataReceiver::Disconnect()
{
   if (Thread != nullptr)
   {
      LOGFUNCTION();
      Thread->setSocket(nullptr);

      Connection.disconnect();
      delete Thread;
      Thread = nullptr;
   }
}

bool AudacityRover::SensorDataReceiver::IsConnected()
{
   return ((Thread != nullptr) && Connection.isConnected());
}

void AudacityRover::SensorDataReceiver::KeepAlive()
{
   if (!IsConnected())
   {
      Connect();
   }
   else if (!Thread->StillConsideredAlive())
   {
      Connect();
   }
}

void AudacityRover::SensorDataConnection::ProcessBuffer()
{
   while (Buffer.getLength() >= 37)
   {
      char *Data = Buffer.getValue();
      if ((Data[0] == 1) && (Data[36] == 23))
      {
         // only allow valid sensor packets
         OpenALRF::SensorBusData3D SensorData;
         SensorData.Origin = OpenALRF::sensoriRemote;
         SensorData.ID = Data[1];
         SensorData.Type = static_cast<OpenALRF::sensor_t>(Data[2]);
         SensorData.UnitUsed = static_cast<OpenALRF::sensorunit_t>(Data[3]);

         memcpy(&SensorData.Data.Timestamp, Data + 4, 8);
         memcpy(&SensorData.Data.Data1, Data + 12, 8);
         memcpy(&SensorData.Data.Data2, Data + 20, 8);
         memcpy(&SensorData.Data.Data3, Data + 28, 8);

         Buffer.remove(0, 36);

         Modules::Instance()->SensorBus->Broadcast(SensorData);
      }
   }
}

void AudacityRover::SensorDataConnection::StillHere()
{
   LastTimeDataReceived = OpenALRF::GetCurrentTimestamp();
}

AudacityRover::SensorDataConnection::SensorDataConnection(Jumpropes::BaseSocket * aSocket) : Jumpropes::ThreadedConnection(aSocket)
{
   LastTimeDataReceived = OpenALRF::GetCurrentTimestamp();;
   DeclaredDeadAfter = Configuration::Instance()->SecondsWhenToDeclareConnectionDead;
}

void AudacityRover::SensorDataConnection::newMessageReceived(const String * sMessage)
{
   StillHere();
   Buffer.append(sMessage);

   ProcessBuffer();
}

bool AudacityRover::SensorDataConnection::StillConsideredAlive() const
{
   auto diff = OpenALRF::GetCurrentTimestamp() - LastTimeDataReceived;

   return (diff > DeclaredDeadAfter);
}
