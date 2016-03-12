#pragma once

#include <OpenALRF/Command/CommandQueue.h>
#include <OpenALRF/System/System.h>

namespace AudacityRover
{
   class WatchCat
   {
   protected:
      OpenALRF::ICommandQueue *Queue;
      OpenALRF::ISystem *System;
      __int64 TickCount;
      __int64 ActCount;
      __int64 CheckCount;
      __int64 LastCheck;

      void MinorAct();
      void MajorAct();
      void CheckAndAct();

   public:
      WatchCat(OpenALRF::ISystem *ASystem, OpenALRF::ICommandQueue *AQueue);

      void ForceAction();

      void Process();
   };
};
