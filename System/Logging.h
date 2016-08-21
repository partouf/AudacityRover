#pragma once

#include <string>

namespace AudacityRover
{
   class ILogging
   {
   public:
      virtual void WriteToLog(const std::string AModule, const std::string AMessage) = 0;
   };

   class Logging : public ILogging
   {
   protected:
      std::string GetTimestamp();
   public:
      void WriteToLog(const std::string AModule, const std::string AMessage) override;
   };
}