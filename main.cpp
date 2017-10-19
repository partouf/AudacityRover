
#include <iostream>

#include <Groundfloor/Atoms/Defines.h>
#include <Groundfloor/Atoms/Initialize.h>

#include "Command/CommandQueueDefault.h"
#include "Communication/CommunicationJumpropes.h"
#include "Communication/SensorDataTransmitter.h"

#include <Jumpropes/Initialize.h>
#include <Jumpropes/Functions.h>

#include "System/Modules.h"

#include <OpenALRF/World/Map.h>
#include <OpenALRF/World/AStarMapNode.h>


OpenALRF::degrees_t CurrentAngle = 0;

void RebootSystem()
{
   auto Queue = AudacityRover::Modules::Instance()->CommandQueue.get();

   Queue->Add({ OpenALRF::Module::System, OpenALRF::Action::SystemReboot });
   Queue->Process();
}

int main()
{
   std::cout << "Hello" << std::endl;

   bool MapDone = false;


   if (Groundfloor::initGroundfloor())
   {
      std::cout << "World!" << std::endl;

      if (Jumpropes::initJumpropes())
      {
         std::cout << "And Universe!" << std::endl;

         try
         {
            auto Modules = AudacityRover::Modules::Instance();
            Modules->System->ChangeStatus(OpenALRF::Status::Running);
            while (!Modules->System->ShouldQuit())
            {
               Modules->Comm->Process();
               Modules->CommandQueue->Process();

               if (Modules->System->CurrentStatus() == OpenALRF::Status::Running)
               {
                  Modules->SensorSweep();
                  Modules->Cat->Process();

                  if (!MapDone)
                  {
                     //findSolutionForMap("maptest.txt", "mapout.txt");
                     MapDone = true;
                  }
               }

               GFMillisleep(5000);
            }

            if (Modules->System->ShouldQuit())
            {
               std::cout << "Someone made me Quit" << std::endl;
            }
         }
         catch (std::exception &e)
         {
            std::cout << "An exception was thrown: " << e.what() << std::endl;
         }

         std::cout << "Goodbye!" << std::endl;

         /*
         OpenALRF::Command Hello{ OpenALRF::Module::RemotePilot, OpenALRF::Action::RemotePilotForward, 2, 0 };
         Queue.Add(Hello);

         OpenALRF::Command World{ OpenALRF::Module::Camera, OpenALRF::Action::CameraCapture, 0, 0 };
         Queue.Add(World);
         */

         Jumpropes::finiJumpropes();
      }

      Groundfloor::initGroundfloor();
   }

   return 1;
}
