#include "RemotePilotGoPiGo.h"

#ifdef USENULLBOARD
#include <GoPiGo/OS/null.h>
#else
#include <GoPiGo/OS/linux.h>
#endif

#include <OpenALRF/Common/Timing.h>

#include <iostream>
#include <Groundfloor/Bookshelfs/BValue.h>
#include <chrono>

using namespace std::chrono;

#if defined(__clang__)
namespace std {
   namespace chrono {
      typedef monotonic_clock steady_clock;
   }
}
#define is_steady is_monotonic
#endif

constexpr auto M_PI = 3.14159265358979323846;

void AudacityRover::RemotePilotGoPiGo::ReconnectIfNeeded()
{
   if (!MainBoard->IsConnected())
   {
      MainBoard->Connect();
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
   Encoders = new GoPiGo::WheelEncodersWithErrorDetection(MainBoard);

   LatestMeasuredSpeed1 = 0;
   LatestMeasuredSpeed2 = 0;
   AccumulatedDistanceTraveled1 = 0;
   AccumulatedDistanceTraveled2 = 0;
}

AudacityRover::RemotePilotGoPiGo::~RemotePilotGoPiGo()
{
   delete Wheels;
   delete MainBoard;
}

void AudacityRover::RemotePilotGoPiGo::Forward(OpenALRF::distance_t ADistance)
{
   ReconnectIfNeeded();

   Wheels->SetSpeedBothMotors(255);

   GoPiGo::encoderpulses_t DistanceAsPulseCount = static_cast<GoPiGo::encoderpulses_t>(ADistance * 0.99);

   Encoders->Start(DistanceAsPulseCount, DistanceAsPulseCount);
   Wheels->Forward();

   MovementCheckLoop();

   Stop();
}

void AudacityRover::RemotePilotGoPiGo::MovementCheckLoop()
{
   GoPiGo::encoderpulses_t D1 = -1, D2 = -1;
   int LoopDuplicates = 0;

   auto T0 = steady_clock::now();

   int maxduplicates = 35;
   while (LoopDuplicates < maxduplicates)
   {
      if (Encoders->CheckShouldStop())
      {
         break;
      }

      int IdleCount = 0;
      if (D1 == Encoders->GetLatestDistance1())
      {
         LoopDuplicates++;
         IdleCount++;
      }
      D1 = Encoders->GetLatestDistance1();

      if (D2 == Encoders->GetLatestDistance2())
      {
         LoopDuplicates++;
         IdleCount++;
      }
      D2 = Encoders->GetLatestDistance2();

      if (IdleCount == 0)
      {
         LoopDuplicates = 0;
      }

      MainBoard->Sleep(70);
   }


   auto T9 = steady_clock::now();
   auto TimeDiff = duration_cast<microseconds>(T9 - T0).count();

   LatestMeasuredSpeed1 = (double)Encoders->GetLatestDistance1() / ((double)TimeDiff / 1000000.0);   // centimeters per second?
   LatestMeasuredSpeed2 = (double)Encoders->GetLatestDistance2() / ((double)TimeDiff / 1000000.0);

   AccumulatedDistanceTraveled1 += Encoders->GetLatestDistance1();
   AccumulatedDistanceTraveled2 += Encoders->GetLatestDistance2();

   if (LoopDuplicates >= maxduplicates)
   {
      std::cerr << "Encoder hasn't indicated any movement for a little while (2.8s)" << std::endl;
   }

   std::cout << GetStatusInfo() << std::endl;
}

void AudacityRover::RemotePilotGoPiGo::Backward(OpenALRF::distance_t ADistance)
{
   ReconnectIfNeeded();

   Wheels->SetSpeedBothMotors(255);

   GoPiGo::encoderpulses_t DistanceAsPulseCount = static_cast<GoPiGo::encoderpulses_t>(ADistance * 0.99);

   Encoders->Start(DistanceAsPulseCount, DistanceAsPulseCount);

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
   this->Wheels->SetSpeedMotor2(255);

   // so given the situation, the vehicle's right tire will travel along the edge of a small circle, which can trace back to an angle
   //  so, we know that if 1 rotation of the wheel is 19cm in travel
   //      and the radius of our circle is the distance between the (centers of the) wheels, which is 11.5cm
   //      23*PI would be the the distance to travel the entire circle, we only need AAngle/360th degrees of that

   double calc = ((23.0 * M_PI) / 360.0) * AAngle;
   OpenALRF::distance_t distance = (OpenALRF::distance_t)calc;

   Encoders->Start(0, distance);

   Wheels->Forward();

   MovementCheckLoop();

   Stop();
}

void AudacityRover::RemotePilotGoPiGo::Right(OpenALRF::degrees_t AAngle)
{
   this->ReconnectIfNeeded();

   this->Wheels->SetSpeedMotor1(255);
   this->Wheels->SetSpeedMotor2(0);

   double calc = ((23.0 * M_PI) / 360.0) * AAngle;
   OpenALRF::distance_t distance = (OpenALRF::distance_t)calc;

   Encoders->Start(distance, 0);

   Wheels->Forward();

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

   Val.setInt64(AccumulatedDistanceTraveled1);
   Data += "<accdistance1>";
   Data += Val.asString()->getValue();
   Data += "</accdistance1>";

   Val.setInt64(AccumulatedDistanceTraveled2);
   Data += "<accdistance2>";
   Data += Val.asString()->getValue();
   Data += "</accdistance2>";

   Val.setDouble(LatestMeasuredSpeed1);
   Data += "<latestspeed1>";
   Data += Val.asString()->getValue();
   Data += "</latestspeed1>";

   Val.setDouble(LatestMeasuredSpeed2);
   Data += "<latestspeed2>";
   Data += Val.asString()->getValue();
   Data += "</latestspeed2>";

   return Data;
}
