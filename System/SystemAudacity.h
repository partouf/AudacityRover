#pragma once

#include <OpenALRF/System/SystemLinux.h>

namespace AudacityRover
{
   class SystemAudacity : public OpenALRF::SystemLinux
   {
   public:
      SystemAudacity();

      bool HasValidActiveNetwork() override;
   };
}