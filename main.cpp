
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
#include "main.h"

OpenALRF::degrees_t CurrentAngle = 0;

void RebootSystem()
{
   auto Queue = AudacityRover::Modules::Instance()->CommandQueue;

   Queue->Add({ OpenALRF::modSystem, OpenALRF::actSystemReboot });
   Queue->Process();
}


int GetAngleBetweenMapSearchNodes(const OpenALRF::MapSearchNode *ANodeCurrent, const OpenALRF::MapSearchNode *ANodeNext)
{
   int dx = ANodeNext->GetX() - ANodeCurrent->GetX();
   int dy = ANodeNext->GetY() - ANodeCurrent->GetY();

   if ((dy == 0) && (dx < 0))
   {
      return 270;
   }
   else if ((dy == 0) && (dx > 0))
   {
      return 90;
   }
   else if ((dy < 0) && (dx == 0))
   {
      return 0;
   }
   else if ((dy > 0) && (dx == 0))
   {
      return 180;
   }
   else if ((dy != 0) && (dx != 0))
   {
      return static_cast<int>(atan(dy/dx));
   }

   return -1;
}

double GetDistanceBetweenMapSearchNodes(const OpenALRF::MapSearchNode *ANodeCurrent, const OpenALRF::MapSearchNode *ANodeNext)
{
   double dx = fabs(ANodeCurrent->GetX() - ANodeNext->GetX());
   double dy = fabs(ANodeCurrent->GetY() - ANodeNext->GetY());

   return sqrt(dx*dx + dy*dy);
}

OpenALRF::degrees_t MakeCommandsFromMapSearchDifference(const OpenALRF::MapSearchNode *ANodeCurrent, const OpenALRF::MapSearchNode *ANodeNext, const OpenALRF::degrees_t ACurrentAngle)
{
   auto Queue = AudacityRover::Modules::Instance()->CommandQueue;

   int16_t distance = static_cast<int16_t>(GetDistanceBetweenMapSearchNodes(ANodeCurrent, ANodeNext) * 60);
   int angle = GetAngleBetweenMapSearchNodes(ANodeCurrent, ANodeNext);

   OpenALRF::degrees_t NewAngle = ACurrentAngle;

   if (angle == -1)
   {
      // do nothing
   }
   else if (angle == 180)
   {
      // best to just go backwards instead of turning
      Queue->Add({ OpenALRF::modRemotePilot, OpenALRF::actRemotePilotBackward, distance });
      NewAngle = 0;
   }
   else if (angle == 0)
   {
      Queue->Add({ OpenALRF::modRemotePilot, OpenALRF::actRemotePilotForward, distance });
      NewAngle = 0;
   }
   else
   {
      NewAngle -= ACurrentAngle;

      if (NewAngle > 180)
      {
         Queue->Add({ OpenALRF::modRemotePilot, OpenALRF::actRemotePilotLeft, 180 - NewAngle });
      }
      else if (NewAngle > 0)
      {
         Queue->Add({ OpenALRF::modRemotePilot, OpenALRF::actRemotePilotRight, NewAngle });
      }

      Queue->Add({ OpenALRF::modRemotePilot, OpenALRF::actRemotePilotForward, distance });
   }

   return NewAngle;
}

void findSolutionForMap(const std::string AInMapFile, const std::string AOutMapFile)
{
   OpenALRF::Map20x20m map(AInMapFile);
   OpenALRF::SetCurrentSearchMap(&map);

   AStarSearch<OpenALRF::MapSearchNode> astarsearch;

   OpenALRF::MapSearchNode StartNode(10, 20);
   OpenALRF::MapSearchNode EndNode(10, 0);
   astarsearch.SetStartAndGoalStates(StartNode, EndNode);

   int SearchState;
   do
   {
      SearchState = astarsearch.SearchStep();
   } while (SearchState == STLAStar::SEARCH_STATE_SEARCHING);


   if (SearchState == STLAStar::SEARCH_STATE_SUCCEEDED)
   {
      OpenALRF::MapSearchNode *node = astarsearch.GetSolutionStart();
      OpenALRF::MapSearchNode *previousnode = node;

      int steps = 0;
      for (;; )
      {
         node = astarsearch.GetSolutionNext();
         if (!node) break;

         map.SetMapValue(node->GetX(), node->GetY(), OpenALRF::MapValuePath);

         CurrentAngle = MakeCommandsFromMapSearchDifference(previousnode, node, CurrentAngle);
         previousnode = node;

         steps++;
      };

      map.SaveToFile(AOutMapFile);

      // Once you're done with the solution you can free the nodes up
      astarsearch.FreeSolutionNodes();
   }
   else if (SearchState == STLAStar::SEARCH_STATE_FAILED)
   {
      cout << "Search terminated. Did not find goal state\n";
   }

   astarsearch.EnsureMemoryFreed();
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
            while (!Modules->System->ShouldQuit())
            {
               Modules->Comm->Process();
               Modules->CommandQueue->Process();
               Modules->Cat->Process();

               if (!MapDone)
               {
                  findSolutionForMap("maptest.txt", "mapout.txt");
                  MapDone = true;
               }

               GFMillisleep(100);
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
