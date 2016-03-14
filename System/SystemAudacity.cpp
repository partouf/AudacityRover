
#include "SystemAudacity.h"
#include <Jumpropes/Functions.h>

AudacityRover::SystemAudacity::SystemAudacity() : OpenALRF::SystemLinux()
{
}

bool AudacityRover::SystemAudacity::HasValidActiveNetwork()
{
   bool HasNetwork = false;

   auto list = Jumpropes::ListNetworkInterfaces();
   for (auto netif : list)
   {
      std::cout << netif.IP.ip.getValue() << std::endl;

      if (!netif.IP.ip.startsWith_ansi("127.") && !netif.IP.ip.startsWith_ansi("0.") && !netif.IP.ip.startsWith_ansi("169.254."))
      {
         HasNetwork = true;
         break;
      }
   }
   std::cout << "-----" << std::endl;

   return HasNetwork;
}

std::string AudacityRover::SystemAudacity::GetStatusInfo()
{
   return std::string();
}
