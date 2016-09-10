#pragma once

#include <string>

namespace AudacityRover
{
   class ILogging
   {
   public:
      virtual void WriteToLog(const std::string AModule, const long ALineNumber, const std::string AMessage) = 0;
   };

   class Logging : public ILogging
   {
   protected:
      std::string GetTimestamp();
   public:
      void WriteToLog(const std::string AModule, const long ALineNumber, const std::string AMessage) override;
   };
}

#define LOGFUNCTION() Modules::Instance()->Logging->WriteToLog(__FILE__, __LINE__, __FUNCTION__)
#define LOGCUSTOM(x) Modules::Instance()->Logging->WriteToLog(__FILE__, __LINE__, x)

