#include "CameraRaspi.h"

#include <cstdlib>
#include <string>
#include <OpenALRF/Common/Timing.h>

AudacityRover::CameraRaspi::CameraRaspi() : OpenALRF::IMainCamera()
{

}

void AudacityRover::CameraRaspi::TakePicture()
{
   // todo: use library

   auto ts = OpenALRF::GetCurrentTimestamp();
   std::string cmd = "raspistill -vf -o " + std::to_string(ts) + ".png";
   system(cmd.c_str());
}
