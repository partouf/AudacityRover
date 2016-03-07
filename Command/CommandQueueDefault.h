#pragma once

#include <OpenALRF/Command/CommandQueue.h>
#include <OpenALRF/Pilot/RemotePilot.h>
#include <OpenALRF/Pilot/AutoPilot.h>
#include <OpenALRF/Camera/MainCamera.h>
#include <OpenALRF/System/System.h>
#include <vector>

namespace OpenALRF
{
   class CommandQueue : public ICommandQueue
   {
   protected:
      std::vector<Command> Queue;

      IRemotePilot *Pilot;
      IAutoPilot *Auto;
      IMainCamera *MainCamera;
      ISystem *System;

      void Init();

      void DoNextCommand();
      void DoCommand(Command ACmd);
   public:
      CommandQueue();

      void Add(Command ACmd);

      void Process();
   };
};
