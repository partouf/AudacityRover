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
      else if (ACmd.Action == OpenALRF::actSystemInfoReport)
      {
         // todo: redo module system so we can gather info on all modules and use the Comm system here
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
      else if (ACmd.Action == OpenALRF::actRemotePilotBackward)
      {
         Pilot->Backward(static_cast<OpenALRF::distance_t>(ACmd.param1));
      }
      else if (ACmd.Action == OpenALRF::actRemotePilotLeft)
      {
         Pilot->Left(static_cast<OpenALRF::distance_t>(ACmd.param1));
      }
      else if (ACmd.Action == OpenALRF::actRemotePilotRight)
      {
         Pilot->Right(static_cast<OpenALRF::distance_t>(ACmd.param1));
      }
      else if (ACmd.Action == OpenALRF::actRemotePilotStop)
      {
         Pilot->Stop();
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

std::string AudacityRover::CommandQueue::GetStatusInfo()
{
   std::string Data;

   Data += "<system>";
   Data += System->GetStatusInfo();
   Data += "</system>";

   Data += "<camera>";
   Data += MainCamera->GetStatusInfo();
   Data += "</camera>";

   Data += "<remotepilot>";
   Data += Pilot->GetStatusInfo();
   Data += "</remotepilot>";

   return Data;
}
