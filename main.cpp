
#include <iostream>

#include <Groundfloor/Atoms/Defines.h>
#include <Groundfloor/Atoms/Initialize.h>

#include "Command/CommandQueueDefault.h"
#include "Communication/CommunicationJumpropes.h"

#include <Jumpropes/Initialize.h>
#include <Jumpropes/Functions.h>

#include "System/WatchCat.h"

AudacityRover::CommandQueue Queue;
OpenALRF::ISystem *System = Queue.GetSystem();

void RebootSystem()
{
   Queue.Add({ OpenALRF::modSystem, OpenALRF::actSystemReboot });
   Queue.Process();
}

int main()
{
   std::cout << "Hello" << std::endl;

   if (Groundfloor::initGroundfloor())
   {
      std::cout << "World!" << std::endl;

      if (Jumpropes::initJumpropes())
      {
         std::cout << "And Universe!" << std::endl;

         AudacityRover::WatchCat Cat(System, &Queue);
         AudacityRover::CommunicationJumpropes Comm(&Queue);
         try
         {
            while (!System->ShouldQuit())
            {
               Comm.Process();
               Queue.Process();
               Cat.Process();

               GFMillisleep(10);
            }

         }
         catch (std::exception &e)
         {
            std::cout << e.what() << std::endl;
         }

         std::cout << "Goodbye!" << std::endl;

         /*
         OpenALRF::Command Hello{ OpenALRF::modRemotePilot, OpenALRF::actRemotePilotForward, 2, 0 };
         Queue.Add(Hello);

         OpenALRF::Command World{ OpenALRF::modCamera, OpenALRF::actCameraCapture, 0, 0 };
         Queue.Add(World);
         */

         Jumpropes::finiJumpropes();
      }

      Groundfloor::initGroundfloor();
   }

   return 1;
}
