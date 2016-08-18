#include "CameraRaspi.h"

#include <cstdlib>
#include <string>
#include <OpenALRF/Common/Timing.h>

#include <Groundfloor/Atoms/Defines.h>

#ifdef GF_OS_WIN32
#include <raspicam/src/raspicam_still_cvcam.h>
#else
#include <raspicam/src/raspicam_still.h>
#endif

#include <OpenALRF/ImageProcessing/Software.h>

AudacityRover::CameraRaspi::CameraRaspi() : OpenALRF::IMainCamera()
{
#ifdef GF_OS_WIN32
   Cam = new raspicam::RaspiCam_Still_CVCam();
#else
   Cam = new raspicam::RaspiCam_Still();
#endif

   Cam->open();

   InitializeImage();
}

void AudacityRover::CameraRaspi::InitializeImage()
{
   // todo: how to find out camera/output resolution
   LatestImage.Width = 640;
   LatestImage.Height = 480;
   LatestImage.Channels = 3;

   long BufferLen = LatestImage.Width * LatestImage.Height * LatestImage.Channels;

   LatestImage.Buffer = new unsigned char[BufferLen];
   LatestImage.BufferLength = BufferLen;
}

AudacityRover::CameraRaspi::~CameraRaspi()
{
   delete Cam;
   delete LatestImage.Buffer;
}

void AudacityRover::CameraRaspi::TakePicture()
{
   Cam->grab_retrieve(LatestImage.Buffer, LatestImage.BufferLength);

   OpenALRF::ImageProcessingSoftware Stuff;
   Stuff.Initialize(LatestImage);
   Stuff.MakeObjectMask();

/*
   auto ts = OpenALRF::GetCurrentTimestamp();
   std::string cmd = "raspistill -vf -o " + std::to_string(ts) + ".png";
   system(cmd.c_str());
*/
}

OpenALRF::CapturedImage AudacityRover::CameraRaspi::GetLatestImage()
{
   return LatestImage;
}

std::string AudacityRover::CameraRaspi::GetStatusInfo()
{
   return "";
}
