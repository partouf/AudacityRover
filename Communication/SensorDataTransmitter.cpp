#include "SensorDataTransmitter.h"

#include "../System/Configuration.h"
#include <Groundfloor/Molecules/String.h>
#include <stdexcept>
#include <sstream>

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
   std::ostringstream buffer;
   buffer << ABusData;

   return buffer.str();
}

AudacityRover::SensorDataServer::SensorDataServer() : Jumpropes::ThreadedServer()
{
   if (!startListening(Configuration::Instance()->SensorServerPort))
   {
      throw std::runtime_error("Unable to startListening");
   }
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

void AudacityRover::SensorDataTransmitter::PowerOff()
{
}

void AudacityRover::SensorDataTransmitter::PowerOn()
{
}

bool AudacityRover::SensorDataTransmitter::IsPowered() const
{
   return false;
}

bool AudacityRover::SensorDataTransmitter::NextValue(OpenALRF::Sensor3DData & AValue)
{
   return false;
}
