#pragma once

#include <OpenALRF/Communication/Communication.h>
#include <OpenALRF/Command/CommandQueue.h>

#include <Jumpropes/ThreadedServer.h>
#include <Jumpropes/ThreadedConnection.h>
#include <Jumpropes/ClientSocket.h>

#include <vector>

namespace AudacityRover
{
   class Connection;
   class Receiver;
   class CommunicationJumpropes;

   class Connection : public Jumpropes::ThreadedConnection
   {
   protected:
      Receiver *Server;
      String ReceptionBuffer;
      std::vector<OpenALRF::OrderedCommand> OrderedCommandBuffer;

      void newMessageReceived(const String * sMessage) override;

      OpenALRF::OrderedCommand ReadNextCommand(String *AData);

      void ReplyBinaryOK();
      void ReplyBinaryFail();
      void ReplyHTTPOK();
      void ReplyHTTPOKWithContent(std::string AContentType, std::string AContent);
      void ReplyHTTPFail();
      void ReplyWithFile(const String *AFile);
      Groundfloor::String GetHTTPHeaderForFile(const Groundfloor::String * AFile);
   public:
      Connection(Jumpropes::BaseSocket * AClient, Receiver *AReceiver);
   };

   class Receiver : public Jumpropes::ThreadedServer
   {
   protected:
      Groundfloor::Vector Clients;

      void newClientConnection(Jumpropes::BaseSocket * aClient) override;
   public:
      Receiver();

      std::string LastSender;
   };

   class CommunicationJumpropes : public OpenALRF::ICommunication
   {
   protected:
      std::string incomingdata;

      Receiver Server;

      void InitializeStationConnection(Jumpropes::ClientSocket &Client);
      void LoadFromBackLog();
   public:
      CommunicationJumpropes();

      void Process() override;

      void SendToStation(const std::string AMessage) override;

      // Inherited via ICommunication
      virtual std::string GetStatusInfo() override;
   };
};
