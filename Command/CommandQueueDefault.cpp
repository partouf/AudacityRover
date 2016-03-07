#include "CommandQueueDefault.h"

#include "../Pilot/RemotePilotGoPiGo.h"
#include "../Camera/CameraRaspi.h"
#include <OpenALRF/System/SystemLinux.h>

void OpenALRF::CommandQueue::Init()
{
   System = new SystemLinux();
   Pilot = new RemotePilotGoPiGo();
   Auto = nullptr;// new AutoPilotDefault();
   MainCamera = new CameraRaspi();
}

void OpenALRF::CommandQueue::DoNextCommand()
{
   if (Queue.size() > 0)
   {
      Command cmd = Queue.front();
      Queue.erase(Queue.begin());

      DoCommand(cmd);
   }
}

void OpenALRF::CommandQueue::DoCommand(Command ACmd)
{
   if (ACmd.Module == modCamera)
   {
      if (ACmd.Action == actCameraCapture)
      {
         MainCamera->TakePicture();
      }
   }
   else if (ACmd.Module == modRemotePilot)
   {
      if (ACmd.Action == actRemotePilotForward)
      {
         Pilot->Forward(static_cast<OpenALRF::distance_t>(ACmd.param1));
      }
   }
}

OpenALRF::CommandQueue::CommandQueue() : ICommandQueue()
{
   Init();
}

void OpenALRF::CommandQueue::Add(Command ACmd)
{
   Queue.push_back(ACmd);
}

void OpenALRF::CommandQueue::Process()
{
   DoNextCommand();
}
