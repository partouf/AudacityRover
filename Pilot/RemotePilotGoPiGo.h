#pragma once

#include <OpenALRF/Pilot/RemotePilot.h>

#include <GoPiGo/gopigo.h>

namespace AudacityRover
{
   class RemotePilotGoPiGo : public OpenALRF::IRemotePilot
   {
   private:
      GoPiGo::IBoard *MainBoard;
      GoPiGo::Wheels *Wheels;
      GoPiGo::WheelEncoders *WheelEncoders;

      void ReconnectIfNeeded();
   public:
      RemotePilotGoPiGo();
      ~RemotePilotGoPiGo();

      virtual void Forward(OpenALRF::distance_t ADistance) override;

      virtual void Backward(OpenALRF::distance_t ADistance) override;

      virtual void Left(OpenALRF::degrees_t AAngle) override;

      virtual void Right(OpenALRF::degrees_t AAngle) override;

      virtual void Stop() override;
   };
};
