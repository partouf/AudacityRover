#pragma once

#include <OpenALRF/Pilot/RemotePilot.h>

#include <GoPiGo/gopigo.h>

namespace OpenALRF
{
   class RemotePilotGoPiGo : public IRemotePilot
   {
   private:
      GoPiGo::IBoard *MainBoard;
      GoPiGo::Wheels *Wheels;
      GoPiGo::WheelEncoders *WheelEncoders;

      void ReconnectIfNeeded();
   public:
      RemotePilotGoPiGo();
      ~RemotePilotGoPiGo();

      virtual void Forward(distance_t ADistance) override;

      virtual void Backward(distance_t ADistance) override;

      virtual void Left(degrees_t AAngle) override;

      virtual void Right(degrees_t AAngle) override;

      virtual void Stop() override;
   };
};
