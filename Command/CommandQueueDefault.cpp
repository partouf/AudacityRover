#include "CommandQueueDefault.h"

#include "../System/Modules.h"

void AudacityRover::CommandQueue::Init()
{

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
   auto Pilot = Modules::Instance()->Pilot;
   auto System = Modules::Instance()->System;
   auto MainCamera = Modules::Instance()->MainCamera;

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
      else if (ACmd.Action == OpenALRF::actSystemResume)
      {
         DoResume();
      }
      else if (ACmd.Action == OpenALRF::actSystemWait)
      {
         DoWait();
      }
      else if (ACmd.Action == OpenALRF::actSystemStop)
      {
         DoStop();
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

void AudacityRover::CommandQueue::DoStop()
{
   Modules::Instance()->Pilot->Stop();
   Modules::Instance()->System->ChangeStatus(OpenALRF::statStopped);
}

void AudacityRover::CommandQueue::DoWait()
{
   Modules::Instance()->System->ChangeStatus(OpenALRF::statWaiting);
}

void AudacityRover::CommandQueue::DoResume()
{
   Modules::Instance()->System->ChangeStatus(OpenALRF::statRunning);
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

std::string AudacityRover::CommandQueue::GetStatusInfo()
{
   std::string Data;

   Data += "<system>";
   Data += Modules::Instance()->System->GetStatusInfo();
   Data += "</system>";

   Data += "<camera>";
   Data += Modules::Instance()->MainCamera->GetStatusInfo();
   Data += "</camera>";

   Data += "<remotepilot>";
   Data += Modules::Instance()->Pilot->GetStatusInfo();
   Data += "</remotepilot>";

   Data += "<sensorbus>";
   Data += Modules::Instance()->SensorBus->GetStatusInfo();
   Data += "</sensorbus>";

   return Data;
}
