#include "RemotePilotGoPiGo.h"

#ifdef USENULLBOARD
#include <GoPiGo/OS/null.h>
#else
#include <GoPiGo/OS/linux.h>
#endif

void OpenALRF::RemotePilotGoPiGo::ReconnectIfNeeded()
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

OpenALRF::RemotePilotGoPiGo::RemotePilotGoPiGo() : IRemotePilot()
{
#ifdef USENULLBOARD
   MainBoard = new GoPiGo::NullBoard(1);
#else
   MainBoard = new GoPiGo::LinuxBoard(1);
#endif

   Wheels = new GoPiGo::Wheels(MainBoard);
   WheelEncoders = new GoPiGo::WheelEncoders(MainBoard);
}

OpenALRF::RemotePilotGoPiGo::~RemotePilotGoPiGo()
{
   delete Wheels;
   delete MainBoard;
}

void OpenALRF::RemotePilotGoPiGo::Forward(distance_t ADistance)
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

void OpenALRF::RemotePilotGoPiGo::Backward(distance_t ADistance)
{
   // todo: implement
}

void OpenALRF::RemotePilotGoPiGo::Left(degrees_t AAngle)
{
   // todo: implement
}

void OpenALRF::RemotePilotGoPiGo::Right(degrees_t AAngle)
{
   // todo: implement
}

void OpenALRF::RemotePilotGoPiGo::Stop()
{
   Wheels->Stop();
}
