#pragma once

#include <OpenALRF/Communication/Communication.h>
#include <OpenALRF/Command/CommandQueue.h>

#include <Jumpropes/ThreadedServer.h>
#include <Jumpropes/ThreadedConnection.h>

namespace AudacityRover
{
   class Connection;
   class Receiver;
   class CommunicationJumpropes;

   class Connection : public Jumpropes::ThreadedConnection
   {
   protected:
      Receiver *Server;
      void newMessageReceived(const String * sMessage) override;
   public:
      Connection(Jumpropes::BaseSocket * AClient, Receiver *AReceiver);
   };

   class Receiver : public Jumpropes::ThreadedServer
   {
   protected:
      CommunicationJumpropes *Comm;
      Groundfloor::Vector Clients;

      void newClientConnection(Jumpropes::BaseSocket * aClient) override;
   public:
      Receiver();

      void SetComm(CommunicationJumpropes *AComm);
      CommunicationJumpropes *GetComm();

      std::string LastSender;
   };

   class CommunicationJumpropes : public OpenALRF::ICommunication
   {
   protected:
      OpenALRF::ICommandQueue *CmdQueue;

      std::string incomingdata;

      Receiver Server;

      void LoadFromBackLog();
   public:
      CommunicationJumpropes(OpenALRF::ICommandQueue *Queue);

      void Process() override;

      void SendToStation(const std::string AMessage) override;

      OpenALRF::ICommandQueue *GetCmdQueue();

      // Inherited via ICommunication
      virtual std::string GetStatusInfo() override;
   };
};
