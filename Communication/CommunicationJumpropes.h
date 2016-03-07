#pragma once

#include <OpenALRF/Communication/Communication.h>
#include <OpenALRF/Command/CommandQueue.h>

namespace OpenALRF
{
   class CommunicationJumpropes : public ICommunication
   {
   protected:
      ICommandQueue *CmdQueue;
      std::string incomingdata;

      void LoadFromBackLog();
   public:
      CommunicationJumpropes(ICommandQueue *Queue);

      void Process();
   };
};
