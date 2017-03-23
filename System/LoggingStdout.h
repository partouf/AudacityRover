#pragma once

#include <string>
#include "Logging.h"

namespace AudacityRover
{
   class Logging : public ILogging
   {
   protected:
      std::string GetTimestamp();
   public:
      void WriteToLog(const std::string AModule, const long ALineNumber, const std::string AMessage) override;
   };
}

