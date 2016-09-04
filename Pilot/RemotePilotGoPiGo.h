#pragma once

#include <OpenALRF/Pilot/RemotePilot.h>

#include <GoPiGo/gopigo.h>

namespace AudacityRover
{
   class RemotePilotGoPiGo : public OpenALRF::IRemotePilot
   {
   private:
      int64_t AccumulatedDistanceTraveled1;
      int64_t AccumulatedDistanceTraveled2;

      double LatestMeasuredSpeed1;
      double LatestMeasuredSpeed2;

      void ReconnectIfNeeded();

      void MovementCheckLoop();
   public:
      RemotePilotGoPiGo();
      ~RemotePilotGoPiGo();

      virtual void Forward(OpenALRF::distance_t ADistance) override;

      virtual void Backward(OpenALRF::distance_t ADistance) override;

      virtual void Left(OpenALRF::degrees_t AAngle) override;

      virtual void Right(OpenALRF::degrees_t AAngle) override;

      virtual void Stop() override;

      // Inherited via IRemotePilot
      virtual std::string GetStatusInfo() override;
   };
};
