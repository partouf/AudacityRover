#include "CameraRaspi.h"

#include <cstdlib>
#include <string>
#include <OpenALRF/Common/Timing.h>

OpenALRF::CameraRaspi::CameraRaspi() : IMainCamera()
{

}

void OpenALRF::CameraRaspi::TakePicture()
{
   // todo: use library

   auto ts = GetCurrentTimestamp();
   std::string cmd = "raspistill -vf -o " + std::to_string(ts) + ".png";
   system(cmd.c_str());
}
