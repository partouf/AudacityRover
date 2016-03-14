#include "RemotePilotGoPiGo.h"

#ifdef USENULLBOARD
#include <GoPiGo/OS/null.h>
#else
#include <GoPiGo/OS/linux.h>
#endif

#include <iostream>
#include <Groundfloor/Bookshelfs/BValue.h>

constexpr auto M_PI = 3.14159265358979323846;

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

   MovementCheckLoop();

   Stop();
}

void AudacityRover::RemotePilotGoPiGo::MovementCheckLoop()
{
   GoPiGo::encstatus_t LastStatus = -1;
   int loopduplicates = 0;

   int maxduplicates = 40;
   while (loopduplicates < maxduplicates)   // 40*70=2800ms
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

   if (loopduplicates >= maxduplicates)
   {
      std::cerr << "Encoder hasn't indicated any movement for a little while (2.8s)" << std::endl;
   }
}

void AudacityRover::RemotePilotGoPiGo::Backward(OpenALRF::distance_t ADistance)
{
   ReconnectIfNeeded();

   WheelEncoders->SetTargeting(true, true, static_cast<GoPiGo::encoderpulses_t>(ADistance * 0.99));
   Wheels->Backward();

   MovementCheckLoop();

   Stop();
}

void AudacityRover::RemotePilotGoPiGo::Left(OpenALRF::degrees_t AAngle)
{
   // we can really only go left by shutting down the left wheel and drive a little
   //  this doesn't really give any guarantee for an angle, because not only does
   //  the angle change on how much we drive, but also because of not knowing for sure
   //  how much the left wheel will stay in place, nor do we know how much the right
   //  wheel is gonna slip because of the left wheel not helping.
   // but we're gonna try anyway.

   // (this method should be improved by taking before and after pictures and see if key objects have shifted a known amount)

   // to lower chance of slipping, maybe we can lower the speed?

   this->ReconnectIfNeeded();

   this->Wheels->SetSpeedMotor1(0);
   this->Wheels->SetSpeedMotor2(50);

   // so given the situation, the vehicle's right tire will travel along the edge of a small circle, which can trace back to an angle
   //  so, we know that if 1 rotation of the wheel is 19cm in travel
   //      and the radius of our circle is the distance between the (centers of the) wheels, which is 11.5cm
   //      23*PI would be the the distance to travel the entire circle, we only need AAngle/360th degrees of that

   double calc = ((23.0 * M_PI) / 360.0) * AAngle;
   OpenALRF::distance_t distance = (OpenALRF::distance_t)calc;

   this->WheelEncoders->SetTargeting(false, true, distance);

   MovementCheckLoop();

   Stop();
}

void AudacityRover::RemotePilotGoPiGo::Right(OpenALRF::degrees_t AAngle)
{
   this->ReconnectIfNeeded();

   this->Wheels->SetSpeedMotor1(50);
   this->Wheels->SetSpeedMotor2(0);

   double calc = ((23.0 * M_PI) / 360.0) * AAngle;
   OpenALRF::distance_t distance = (OpenALRF::distance_t)calc;

   this->WheelEncoders->SetTargeting(true, false, distance);

   MovementCheckLoop();

   Stop();
}

void AudacityRover::RemotePilotGoPiGo::Stop()
{
   ReconnectIfNeeded();

   Wheels->Stop();
}

std::string AudacityRover::RemotePilotGoPiGo::GetStatusInfo()
{
   std::string Data;

   Groundfloor::BValue Val;

   Val.setInteger(MainBoard->GetCpuSpeed());
   Data += "<cpuspeed>";
   Data += Val.asString()->getValue();
   Data += "</cpuspeed>";

   Val.setInteger(MainBoard->GetBoardVersion());
   Data += "<boardversion>";
   Data += Val.asString()->getValue();
   Data += "</boardversion>";

   Val.setDouble(MainBoard->GetVoltage());
   Data += "<voltage>";
   Data += Val.asString()->getValue();
   Data += "</voltage>";

   if (WheelEncoders->IsEnabled())
   {
      Data += "<encoderstatus>true</encoderstatus>";
   }
   else
   {
      Data += "<encoderstatus>false</encoderstatus>";
   }

   return Data;
}
