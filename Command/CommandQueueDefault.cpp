#include "CommandQueueDefault.h"

#include "../Pilot/RemotePilotGoPiGo.h"
#include "../Camera/CameraRaspi.h"
#include "../System/SystemAudacity.h"

void AudacityRover::CommandQueue::Init()
{
   System = new AudacityRover::SystemAudacity();
   Pilot = new AudacityRover::RemotePilotGoPiGo();
   Auto = nullptr;// new AutoPilotDefault();
   MainCamera = new AudacityRover::CameraRaspi();
}

void AudacityRover::CommandQueue::DoNextCommand()
{
   if (Queue.size() > 0)
   {
      OpenALRF::Command cmd = Queue.front();
      Queue.erase(Queue.begin());

      DoCommand(cmd);
   }
}

void AudacityRover::CommandQueue::DoCommand(OpenALRF::Command ACmd)
{
   if (ACmd.Module == OpenALRF::modSystem)
   {
      if (ACmd.Action == OpenALRF::actSystemReboot)
      {
         System->RebootNow();
      }
      else if (ACmd.Action == OpenALRF::actSystemRestartNetIF)
      {
         System->RestartNetworkInterface(ACmd.param3);
      }
   }
   else if (ACmd.Module == OpenALRF::modCamera)
   {
      if (ACmd.Action == OpenALRF::actCameraCapture)
      {
         MainCamera->TakePicture();
      }
   }
   else if (ACmd.Module == OpenALRF::modRemotePilot)
   {
      if (ACmd.Action == OpenALRF::actRemotePilotForward)
      {
         Pilot->Forward(static_cast<OpenALRF::distance_t>(ACmd.param1));
      }
   }
}

AudacityRover::CommandQueue::CommandQueue() : ICommandQueue()
{
   Init();
}

void AudacityRover::CommandQueue::Add(OpenALRF::Command ACmd)
{
   Queue.push_back(ACmd);
}

void AudacityRover::CommandQueue::Process()
{
   DoNextCommand();
}

OpenALRF::ISystem * AudacityRover::CommandQueue::GetSystem()
{
   return this->System;
}
