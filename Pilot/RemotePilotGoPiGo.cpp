#include "RemotePilotGoPiGo.h"

#ifdef USENULLBOARD
#include <GoPiGo/OS/null.h>
#else
#include <GoPiGo/OS/linux.h>
#endif

void AudacityRover::RemotePilotGoPiGo::ReconnectIfNeeded()
{
   if (!MainBoard->IsConnected())
   {
      MainBoard->Connect();
   }

   if (!WheelEncoders->IsEnabled())
   {
      WheelEncoders->Enable();
   }
}

AudacityRover::RemotePilotGoPiGo::RemotePilotGoPiGo() : OpenALRF::IRemotePilot()
{
#ifdef USENULLBOARD
   MainBoard = new GoPiGo::NullBoard(1);
#else
   MainBoard = new GoPiGo::LinuxBoard(1);
#endif
   
   if (!MainBoard->Connect())
   {
      throw MainBoard->LastKnownError;
   }

   Wheels = new GoPiGo::Wheels(MainBoard);
   WheelEncoders = new GoPiGo::WheelEncoders(MainBoard);
}

AudacityRover::RemotePilotGoPiGo::~RemotePilotGoPiGo()
{
   delete Wheels;
   delete MainBoard;
}

void AudacityRover::RemotePilotGoPiGo::Forward(OpenALRF::distance_t ADistance)
{
   ReconnectIfNeeded();

   WheelEncoders->SetTargeting(true, true, static_cast<GoPiGo::encoderpulses_t>(ADistance * 0.99));
   Wheels->Forward();

   // todo: dont loop forever
   GoPiGo::encstatus_t LastStatus = -1;
   int loopduplicates = 0;

   while (loopduplicates < 40)   // 40*70=2800ms
   {
      auto st = WheelEncoders->GetStatus();
      if (st.EncoderStatus == 0)
      {
         break;
      }

      if (LastStatus == st.EncoderStatus)
      {
         loopduplicates++;
      }
      else
      {
         loopduplicates = 0;
      }

      MainBoard->Sleep(70);
   }

   if (loopduplicates >= 40)
   {
      // todo: notify system of error
   }

   Stop();
}

void AudacityRover::RemotePilotGoPiGo::Backward(OpenALRF::distance_t ADistance)
{
   // todo: implement
}

void AudacityRover::RemotePilotGoPiGo::Left(OpenALRF::degrees_t AAngle)
{
   // todo: implement
}

void AudacityRover::RemotePilotGoPiGo::Right(OpenALRF::degrees_t AAngle)
{
   // todo: implement
}

void AudacityRover::RemotePilotGoPiGo::Stop()
{
   Wheels->Stop();
}
