#pragma once

#include <OpenALRF/Command/CommandQueue.h>
#include <OpenALRF/Pilot/RemotePilot.h>
#include <OpenALRF/Pilot/AutoPilot.h>
#include <OpenALRF/Camera/MainCamera.h>
#include <OpenALRF/System/System.h>
#include <vector>

namespace AudacityRover
{
   class CommandQueue : public OpenALRF::ICommandQueue
   {
   protected:
      std::vector<OpenALRF::Command> Queue;

      OpenALRF::IRemotePilot *Pilot;
      OpenALRF::IAutoPilot *Auto;
      OpenALRF::IMainCamera *MainCamera;
      OpenALRF::ISystem *System;

      void Init();

      void DoNextCommand();
      void DoCommand(OpenALRF::Command ACmd);
   public:
      CommandQueue();

      void Add(OpenALRF::Command ACmd);

      void Process();

      OpenALRF::ISystem *GetSystem();
   };
};
