#pragma once

#include <OpenALRF/Camera/MainCamera.h>
#include <OpenALRF/Common/CapturedImage.h>
#include <raspicam/src/raspicam_still_interface.h>

namespace AudacityRover
{
   class CameraRaspi : public OpenALRF::IMainCamera
   {
   protected:
      raspicam::RaspiCam_Still_Interface *Cam;
      OpenALRF::CapturedImage LatestImage;

      void InitializeImage();
   public:
      CameraRaspi();
      ~CameraRaspi();

      virtual void TakePicture() override;

      virtual OpenALRF::CapturedImage GetLatestImage() override;

      virtual std::string GetStatusInfo() override;
   };
};
