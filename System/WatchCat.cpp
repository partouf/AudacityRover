#include "WatchCat.h"

#include "../System/Modules.h"
#include <iostream>

void AudacityRover::WatchCat::MinorAct()
{
   auto Queue = Modules::Instance()->CommandQueue.get();

   Queue->Add({ OpenALRF::modSystem, OpenALRF::actSystemRestartNetIF, 0, 0, "eth0" });
   Queue->Add({ OpenALRF::modSystem, OpenALRF::actSystemRestartNetIF, 0, 0, "wlan0" });
   Queue->Add({ OpenALRF::modCamera, OpenALRF::actCameraCapture, 0, 0, "" });

   ActCount++;
}

void AudacityRover::WatchCat::MajorAct()
{
   ActCount++;

   Modules::Instance()->CommandQueue->Add({ OpenALRF::modSystem, OpenALRF::actSystemReboot, 0, 0, "" });
}

void AudacityRover::WatchCat::CheckAndAct()
{
   std::cout << "Meow?" << std::endl;

   if (!Modules::Instance()->System->HasValidActiveNetwork())
   {
      CheckCount++;

      if ((CheckCount < 6) && (ActCount < 2))
      {
         MinorAct();
      }
      else
      {
         MajorAct();
      }
   }
   else
   {
      ActCount = 0;
      CheckCount = 0;
   }

   LastCheck = 0;
   TickCount = 0;
}

AudacityRover::WatchCat::WatchCat()
{
   TickCount = 0;
   LastCheck = 0;
   ActCount = 0;
   CheckCount = 0;
}

void AudacityRover::WatchCat::ForceAction()
{
   CheckAndAct();
}

void AudacityRover::WatchCat::Process()
{
   TickCount++;

   if (TickCount - LastCheck > 6000)
   {
      CheckAndAct();
   }
}
