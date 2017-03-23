#pragma once

#include <string>

namespace AudacityRover
{
   class ISensorDataReceiver
   {
   public:
      virtual bool Connect() = 0;
      virtual void Disconnect() = 0;

      virtual void KeepAlive() = 0;
   };
}
