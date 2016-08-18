
#include "SystemAudacity.h"
#include <Jumpropes/Functions.h>

AudacityRover::SystemAudacity::SystemAudacity() : OpenALRF::SystemLinux()
{
}

bool AudacityRover::SystemAudacity::HasValidActiveNetwork()
{
   bool HasNetwork = false;

   // note for RaspberryPi; you need to remove the 127.0.1.1 line in the /etc/hosts file as it will confuse this into thinking that's the only IP address we have
   auto list = Jumpropes::ListNetworkInterfaces();
   for (auto netif : list)
   {
      if (!netif.IP.ip.startsWith_ansi("127.") && !netif.IP.ip.startsWith_ansi("0.") && !netif.IP.ip.startsWith_ansi("169.254."))
      {
         HasNetwork = true;
         break;
      }
   }

   return HasNetwork;
}

std::string AudacityRover::SystemAudacity::GetStatusInfo()
{
   std::string suggeststatusstr = "";

   switch (CurrentStatus()) {
   case OpenALRF::statStopped:
      suggeststatusstr = "stopped";
      break;
   case OpenALRF::statRunning:
      suggeststatusstr = "running";
      break;
   case OpenALRF::statWaiting:
      suggeststatusstr = "waiting";
      break;
   }

   return suggeststatusstr;
}
