
#include <iostream>

#include "Command/CommandQueueDefault.h"
#include "Communication/CommunicationJumpropes.h"

OpenALRF::CommandQueue Queue;
OpenALRF::CommunicationJumpropes Comm(&Queue);

int main()
{
   std::cout << "Hello, World!" << std::endl;

   OpenALRF::Command Hello{ OpenALRF::modRemotePilot, OpenALRF::actRemotePilotForward, 2, 0 };
   Queue.Add(Hello);

   OpenALRF::Command World{ OpenALRF::modCamera, OpenALRF::actCameraCapture, 0, 0 };
   Queue.Add(World);

   return 1;
}
