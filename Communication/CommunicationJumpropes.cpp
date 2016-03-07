#include "CommunicationJumpropes.h"

OpenALRF::CommunicationJumpropes::CommunicationJumpropes(ICommandQueue * Queue) : ICommunication()
{
   this->CmdQueue = Queue;
}

void OpenALRF::CommunicationJumpropes::LoadFromBackLog()
{
   incomingdata = "";
}

void OpenALRF::CommunicationJumpropes::Process()
{
   // todo
}
