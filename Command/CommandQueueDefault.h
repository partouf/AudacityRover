#pragma once

#include <OpenALRF/Command/CommandQueue.h>
#include <OpenALRF/Pilot/RemotePilot.h>
#include <OpenALRF/Pilot/AutoPilot.h>
#include <OpenALRF/Camera/MainCamera.h>
#include <OpenALRF/System/System.h>
#include <OpenALRF/Communication/SensorBus.h>
#include <vector>

namespace AudacityRover
{
   class CommandQueue : public OpenALRF::ICommandQueue
   {
   protected:
      std::vector<OpenALRF::Command> Queue;

      void Init();

      void DoNextCommand();
      void DoCommand(OpenALRF::Command ACmd);

      void DoStop();
      void DoWait();
      void DoResume();
   public:
      CommandQueue();

      void Add(OpenALRF::Command ACmd);

      void Process();

      // Inherited via ICommandQueue
      std::string GetStatusInfo() const override;
   };
};
