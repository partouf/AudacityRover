#include "LoggingStdout.h"

#include <OpenALRF/Common/Timing.h>
#include <iostream>

std::string AudacityRover::Logging::GetTimestamp()
{
   auto Timestamp = OpenALRF::GetCurrentTimestamp();

   return OpenALRF::GetTimestampAsString(Timestamp);
}

void AudacityRover::Logging::WriteToLog(const std::string AModule, const long ALineNumber, const std::string AMessage)
{
   std::cout << GetTimestamp() << " - " << "[" << AModule << " (" << std::to_string(ALineNumber) << ")] " << AMessage << std::endl;
}
