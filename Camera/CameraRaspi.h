#pragma once

#include <OpenALRF/Camera/MainCamera.h>

namespace OpenALRF
{
   class CameraRaspi : public IMainCamera
   {
   public:
      CameraRaspi();

      void TakePicture();
   };
};
