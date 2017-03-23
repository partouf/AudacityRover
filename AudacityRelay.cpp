#include "Communication/SensorDataReceiverJumpropes.h"
#include <Jumpropes/Initialize.h>
#include <Jumpropes/LookupCache.h>
#include <Jumpropes/ThreadedServer.h>
#include <OpenALRF/Communication/SensorBusListener.h>
#include <OpenALRF/Sensors/SensorTypes.h>
#include <OpenALRF/Sensors/SensorDictionary.h>
#include <OpenALRF/Sensors/ProxySensorWithHistory.h>
#include <OpenALRF/Communication/SensorBusTypes.h>
#include <Groundfloor/Molecules/BaseCommunicator.h>
#include <Groundfloor/Materials/Functions.h>
#include <Groundfloor/Bookshelfs/BValue.h>

#include "Sensors/RelayedSensors.h"
#include "System/Configuration.h"
#include "RelayConnection.h"

#include <string>
#include <vector>
#include <memory>


int main()
{
   if (Jumpropes::initJumpropes())
   {
      OpenALRF::SensorBus bus;

      RelayListener Relay(&bus);

      AudacityRover::SensorDataReceiver receiver("localhost", &bus);

      if (receiver.Connect())
      {
         // hello

         //revei

         //relay.Start

		  while (true)
		  {
			  GFMillisleep(100);
		  }

      }

      Jumpropes::finiJumpropes();
   }

   system("pause");

   return 0;
}

