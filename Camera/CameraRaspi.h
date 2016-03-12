#pragma once

#include <OpenALRF/Camera/MainCamera.h>

namespace AudacityRover
{
   class CameraRaspi : public OpenALRF::IMainCamera
   {
   public:
      CameraRaspi();

      void TakePicture();
   };
};
