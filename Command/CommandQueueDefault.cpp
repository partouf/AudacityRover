#include "CommandQueueDefault.h"

#include "../System/Modules.h"
#include <Groundfloor/Bookshelfs/BValue.h>

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
   auto Pilot = Modules::Instance()->Pilot.get();
   auto System = Modules::Instance()->System.get();
   auto MainCamera = Modules::Instance()->MainCamera.get();

   if (ACmd.Module == OpenALRF::Module::System)
   {
      if (ACmd.Action == OpenALRF::Action::SystemReboot)
      {
         System->RebootNow();
      }
      else if (ACmd.Action == OpenALRF::Action::SystemRestartNetIF)
      {
         System->RestartNetworkInterface(ACmd.param3);
      }
      else if (ACmd.Action == OpenALRF::Action::SystemInfoReport)
      {
         // todo: redo module system so we can gather info on all modules and use the Comm system here
      }
      else if (ACmd.Action == OpenALRF::Action::SystemResume)
      {
         DoResume();
      }
      else if (ACmd.Action == OpenALRF::Action::SystemWait)
      {
         DoWait();
      }
      else if (ACmd.Action == OpenALRF::Action::SystemStop)
      {
         DoStop();
      }
   }
   else if (ACmd.Module == OpenALRF::Module::Camera)
   {
      if (ACmd.Action == OpenALRF::Action::CameraCapture)
      {
         MainCamera->TakePicture();
      }
   }
   else if (ACmd.Module == OpenALRF::Module::RemotePilot)
   {
      if (Pilot != nullptr)
      {
         if (ACmd.Action == OpenALRF::Action::RemotePilotForward)
         {
            Pilot->Forward(static_cast<OpenALRF::distance_t>(ACmd.param1));
         }
         else if (ACmd.Action == OpenALRF::Action::RemotePilotBackward)
         {
            Pilot->Backward(static_cast<OpenALRF::distance_t>(ACmd.param1));
         }
         else if (ACmd.Action == OpenALRF::Action::RemotePilotLeft)
         {
            Pilot->Left(static_cast<OpenALRF::distance_t>(ACmd.param1));
         }
         else if (ACmd.Action == OpenALRF::Action::RemotePilotRight)
         {
            Pilot->Right(static_cast<OpenALRF::distance_t>(ACmd.param1));
         }
         else if (ACmd.Action == OpenALRF::Action::RemotePilotStop)
         {
            Pilot->Stop();
         }
      }
      // todo: else -> relay to other PI
   }
}

void AudacityRover::CommandQueue::DoStop()
{
   Modules::Instance()->Pilot->Stop();
   Modules::Instance()->System->ChangeStatus(OpenALRF::Status::Stopped);
}

void AudacityRover::CommandQueue::DoWait()
{
   Modules::Instance()->System->ChangeStatus(OpenALRF::Status::Waiting);
}

void AudacityRover::CommandQueue::DoResume()
{
   Modules::Instance()->System->ChangeStatus(OpenALRF::Status::Running);
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

std::string AudacityRover::CommandQueue::GetStatusInfo() const
{
   std::string Data;

   Data += "<queuesize>";
   Data += std::to_string(Queue.size());
   Data += "</queuesize>";

   return Data;
}
