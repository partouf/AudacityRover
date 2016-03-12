#pragma once

#include <OpenALRF/Command/CommandQueue.h>
#include <OpenALRF/System/System.h>
#include <cstdint>

namespace AudacityRover
{
   class WatchCat
   {
   protected:
      OpenALRF::ICommandQueue *Queue;
      OpenALRF::ISystem *System;
      int64_t TickCount;
      int64_t ActCount;
      int64_t CheckCount;
      int64_t LastCheck;

      void MinorAct();
      void MajorAct();
      void CheckAndAct();

   public:
      WatchCat(OpenALRF::ISystem *ASystem, OpenALRF::ICommandQueue *AQueue);

      void ForceAction();

      void Process();
   };
};
