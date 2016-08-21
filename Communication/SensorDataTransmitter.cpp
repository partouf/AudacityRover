#include "SensorDataTransmitter.h"

#include "../System/Configuration.h"
#include <Groundfloor/Molecules/String.h>

void AudacityRover::SensorDataServer::CleanupDisconnectedClients()
{
   auto ClientIter = Clients.begin();
   while (ClientIter != Clients.end())
   {
      auto Client = *ClientIter;
      if (!Client->isConnected())
      {
         ClientIter = Clients.erase(ClientIter);
         delete Client;
      }
      else
      {
         ++ClientIter;
      }
   }
}

Groundfloor::String AudacityRover::SensorDataServer::SensorDataToString(const OpenALRF::SensorBusData3D ABusData)
{
   Groundfloor::String Data;

   // SOH
   Data.append(1);

   Data.append(ABusData.ID);
   Data.append(ABusData.Type & 0xff);
   Data.append(ABusData.UnitUsed & 0xff);

   // note; as you can see by the memcpy, the architectures of server/client needs to be the same
   //  should probably rewritten to maskshift byte writing

   char data64bits[8];

   memcpy(data64bits, &ABusData.Data.Timestamp, 8);
   Data.append(data64bits, 8);

   memcpy(data64bits, &ABusData.Data.Data1, 8);
   Data.append(data64bits, 8);

   memcpy(data64bits, &ABusData.Data.Data2, 8);
   Data.append(data64bits, 8);

   memcpy(data64bits, &ABusData.Data.Data3, 8);
   Data.append(data64bits, 8);

   // ETB
   Data.append(23);

   return Data;
}

AudacityRover::SensorDataServer::SensorDataServer() : Jumpropes::ThreadedServer()
{
   startListening(Configuration::Instance()->SensorServerPort);
}

void AudacityRover::SensorDataServer::newClientConnection(Jumpropes::BaseSocket * aClient)
{
   Clients.push_back(aClient);
}

void AudacityRover::SensorDataServer::BroadcastToClients(const OpenALRF::SensorBusData3D ABusData)
{
   Groundfloor::String Data = SensorDataToString(ABusData);
   bool ShouldCleanUpClients = false;

   CommReturnData err;
   for (auto Client : Clients)
   {
      if ((Client != nullptr) && Client->isConnected())
      {
         Client->send(&Data, &err);
         if (err.error)
         {
            ShouldCleanUpClients = true;

            Client->disconnect();
         }
      }
      else
      {
         ShouldCleanUpClients = true;
      }
   }

   if (ShouldCleanUpClients)
   {
      CleanupDisconnectedClients();
   }
}

AudacityRover::SensorDataTransmitter::SensorDataTransmitter() : OpenALRF::ISensor3DBusListener(ALLSENSORS)
{
}

void AudacityRover::SensorDataTransmitter::NewSensorData(const OpenALRF::SensorBusData3D ABusData)
{
   if (ABusData.Origin == OpenALRF::sensoriLocal)
   {
      Server.BroadcastToClients(ABusData);
   }
}
