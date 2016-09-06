#include "SensorDataReceiver.h"

#include <OpenALRF/Communication/SensorBusTypes.h>
#include <Jumpropes/Functions.h>
#include "../System/Configuration.h"

AudacityRover::SensorDataReceiver::SensorDataReceiver(const string AIPAddress)
{
   Address.setValue(AIPAddress);
}

bool AudacityRover::SensorDataReceiver::Connect()
{
   Disconnect();

   Jumpropes::LookupObject Lookup;
   Jumpropes::JRresolveIP(&Address, &Lookup);

   Connection.getRemoteAddress()->setValue(Lookup.getAddress(0));
   Connection.remotePort.set(Configuration::Instance()->SensorServerPort);

   if (Connection.connect())
   {
      Thread = new SensorDataConnection(&Connection);
      Thread->start();

      return true;
   }
   else
   {
      return false;
   }
}

void AudacityRover::SensorDataReceiver::Disconnect()
{
   if (Thread != nullptr)
   {
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

AudacityRover::SensorDataConnection::SensorDataConnection(Jumpropes::BaseSocket * aSocket) : Jumpropes::ThreadedConnection(aSocket)
{
}

void AudacityRover::SensorDataConnection::newMessageReceived(const String * sMessage)
{
   if (sMessage->getLength() == 37)
   {
      char *Data = sMessage->getValue();
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
      }
   }
}
