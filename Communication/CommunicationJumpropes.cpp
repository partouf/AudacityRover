#include "CommunicationJumpropes.h"

#include <Jumpropes/ThreadedConnection.h>
#include <Groundfloor/Materials/Functions.h>
#include <Groundfloor/Bookshelfs/BValue.h>
#include <Jumpropes/ClientSocket.h>

AudacityRover::CommunicationJumpropes::CommunicationJumpropes(OpenALRF::ICommandQueue * Queue) : OpenALRF::ICommunication()
{
   this->CmdQueue = Queue;
   this->Server.SetComm(this);
   this->Server.startListening(13666);
}

void AudacityRover::CommunicationJumpropes::LoadFromBackLog()
{
   incomingdata = "";
}

void AudacityRover::CommunicationJumpropes::Process()
{
   // todo
}

void AudacityRover::CommunicationJumpropes::SendToStation(const std::string AMessage)
{
   Jumpropes::ClientSocket Client;
   Client.remotePort.set(14666);
   Client.getRemoteAddress()->ip = this->Server.LastSender;
   Client.connect();

   Groundfloor::String Data = AMessage;
   Groundfloor::CommReturnData ErrorInfo;

   if (!Client.send(&Data, &ErrorInfo))
   {
      std::cerr << "Failed to send data to " << this->Server.LastSender << " (" << ErrorInfo.errorcode << ")" << std::endl;
   }

   Client.disconnect();
}

OpenALRF::ICommandQueue * AudacityRover::CommunicationJumpropes::GetCmdQueue()
{
   return this->CmdQueue;
}

std::string AudacityRover::CommunicationJumpropes::GetStatusInfo()
{
   return "";
}

void AudacityRover::Receiver::newClientConnection(Jumpropes::BaseSocket * aClient)
{
   LastSender = aClient->getRemoteAddress()->ip.getValue();

   Connection *Conn = new Connection(aClient, this);
   Clients.addElement(Conn);
}

AudacityRover::Receiver::Receiver() : Jumpropes::ThreadedServer()
{
}

void AudacityRover::Receiver::SetComm(CommunicationJumpropes * AComm)
{
   this->Comm = AComm;
}

AudacityRover::CommunicationJumpropes * AudacityRover::Receiver::GetComm()
{
   return this->Comm;
}

void AudacityRover::Connection::newMessageReceived(const String * sMessage)
{
   if (sMessage->startsWith_ansi("BINCMD"))
   {
      char *msg = sMessage->getPointer(6);
      uint32_t cmdlen = (msg[0] << 24) | (msg[1] << 16) | (msg[2] << 8) | msg[3];

      OpenALRF::Command BinCmd{ OpenALRF::modVoid, OpenALRF::actVoid, 0, 0, "" };
      if (cmdlen >= 8)
      {
         BinCmd.Module = static_cast<OpenALRF::module_t>(msg[4] << 8 | msg[5]);
         BinCmd.Action = static_cast<OpenALRF::action_t>(msg[6] << 8 | msg[7]);

         if (cmdlen >= 10)
         {
            BinCmd.param1 = static_cast<int16_t>(msg[8] << 8 | msg[9]);
            if (cmdlen >= 12)
            {
               BinCmd.param2 = static_cast<int16_t>(msg[10] << 8 | msg[11]);

               BinCmd.param3 = "";
               if (cmdlen > 12)
               {
                  Groundfloor::String StrParam(sMessage->getPointer(12), cmdlen - 12);
                  BinCmd.param3.append(StrParam.getValue(), StrParam.getLength());
               }
            }
         }

         this->Server->GetComm()->GetCmdQueue()->Add(BinCmd);

         Groundfloor::String Understood("OK");
         this->socket->send(&Understood);
      }
   }
   else if (sMessage->startsWith_ansi("GET /"))
   {
      int httppos = sMessage->pos_ansi("HTTP");
      Groundfloor::String url(sMessage->getPointer(4) + 1, httppos - 6);
      std::cout << url.getValue() << std::endl;

      OpenALRF::Command StrCmd{OpenALRF::modVoid, OpenALRF::actVoid, 0, 0, ""};

      Groundfloor::Vector SplitURL;
      Groundfloor::split_p(&SplitURL, &url, "/", 5);
      Groundfloor::BValue Val;

      if (SplitURL.size() >= 2)
      {
         Val.setString(static_cast<Groundfloor::String *>(SplitURL.elementAt(0)));
         StrCmd.Module = static_cast<OpenALRF::module_t>(Val.asInteger());

         Val.setString(static_cast<Groundfloor::String *>(SplitURL.elementAt(1)));
         StrCmd.Action = static_cast<OpenALRF::action_t>(Val.asInteger());

         if (SplitURL.size() >= 3)
         {
            Val.setString(static_cast<Groundfloor::String *>(SplitURL.elementAt(2)));
            StrCmd.param1 = static_cast<int16_t>(Val.asInteger());

            if (SplitURL.size() >= 4)
            {
               Val.setString(static_cast<Groundfloor::String *>(SplitURL.elementAt(3)));
               StrCmd.param2 = static_cast<int16_t>(Val.asInteger());

               if (SplitURL.size() == 5)
               {
                  StrCmd.param3 = static_cast<Groundfloor::String *>(SplitURL.elementAt(4))->getValue();
               }
            }
         }

         this->Server->GetComm()->GetCmdQueue()->Add(StrCmd);

         Groundfloor::String Understood("HTTP/1.1 200 OK\n\nOK\n");
         this->socket->send(&Understood);
         this->socket->disconnect();
      }
      else
      {
         Groundfloor::String Error("HTTP/1.1 404 Error\n\nError\n");
         this->socket->send(&Error);
         this->socket->disconnect();
      }
   }
}

AudacityRover::Connection::Connection(Jumpropes::BaseSocket * AClient, Receiver *AReceiver) : Jumpropes::ThreadedConnection(AClient)
{
   this->Server = AReceiver;
   this->start();
}

