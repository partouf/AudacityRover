#pragma once

#include <string>

namespace AudacityRover
{
   class ILogging
   {
   public:
      virtual void WriteToLog(const std::string AModule, const long ALineNumber, const std::string AMessage) = 0;
   };
}

#ifndef NOLOGGING
#include "../System/Modules.h"
#endif

#ifdef NOLOGGING
#define LOGFUNCTION() void()
#define LOGCUSTOM(x) void(x)
#else
#define LOGFUNCTION() Modules::Instance()->Logging->WriteToLog(__FILE__, __LINE__, __FUNCTION__)
#define LOGCUSTOM(x) Modules::Instance()->Logging->WriteToLog(__FILE__, __LINE__, x)
#endif
