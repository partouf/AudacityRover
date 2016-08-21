#include "Logging.h"

#include <OpenALRF/Common/Timing.h>

std::string AudacityRover::Logging::GetTimestamp()
{
   auto Timestamp = OpenALRF::GetCurrentTimestamp();

   return std::string();
}
