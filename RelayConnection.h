#pragma once

#include <Jumpropes/ThreadedServer.h>
#include <Jumpropes/ThreadedConnection.h>
#include <Jumpropes/Common/HttpClient.h>
#include <OpenALRF/Communication/SensorBusTypes.h>
#include <Groundfloor/Molecules/BaseCommunicator.h>
#include <Groundfloor/Materials/Functions.h>
#include <Groundfloor/Bookshelfs/BValue.h>
#include <OpenALRF/Communication/SensorBus.h>
#include "Sensors/RelayedSensors.h"
#include "SensorRelay.h"
#include "sha1.h"


#include <wslay/wslay.h>

SensorRelay PI1;
SensorRelay PI2;

class RelayConnection : public Jumpropes::ThreadedConnection, AudacityRover::ProxySensorRelayListener
{
protected:
   bool WebSocketMode;
   OpenALRF::SensorBus *Bus;

   wslay_event_context_ptr ctx;
   struct wslay_event_callbacks callbacks;
public:
   RelayConnection(OpenALRF::SensorBus *ABus, Jumpropes::BaseSocket *aSocket) : ThreadedConnection(aSocket), AudacityRover::ProxySensorRelayListener()
   {
      this->Bus = ABus;
      this->WebSocketMode = false;

      this->callbacks = {
         RelayConnection::recv_callback,
         RelayConnection::send_callback,
         NULL,
         NULL,
         NULL,
         NULL,
         RelayConnection::on_msg_recv_callback
      };

      this->start();
   }

   void Subscribe(const String *ASystem, const String *ASensor)
   {
      if (ASystem->match_ansi("PI1"))
      {
         auto t = OpenALRF::SensorDictionary::GetSensorTypeFromName(ASensor->getValue());

		 BValue SensorID;
		 SensorID.setString(ASensor);
		 
		 auto sensor = PI1.GetSensorRelayByID(SensorID.asInteger());
		 if (sensor != nullptr)
		 {
			 sensor->Subscribe(this);
		 }
	  }
      else if (ASystem->match_ansi("PI2"))
      {
		  auto t = OpenALRF::SensorDictionary::GetSensorTypeFromName(ASensor->getValue());

		  BValue SensorID;
		  SensorID.setString(ASensor);

		  auto sensor = PI1.GetSensorRelayByID(SensorID.asInteger());
		  if (sensor != nullptr)
		  {
			  sensor->Subscribe(this);
		  }
      }
   }

   void Unsubscribe(const String *ASystem, const String *ASensor)
   {
	   if (ASystem->match_ansi("PI1"))
	   {
		   auto t = OpenALRF::SensorDictionary::GetSensorTypeFromName(ASensor->getValue());

		   BValue SensorID;
		   SensorID.setString(ASensor);

		   auto sensor = PI1.GetSensorRelayByID(SensorID.asInteger());
		   if (sensor != nullptr)
		   {
			   sensor->Unsubscribe(this);
		   }
	   }
	   else if (ASystem->match_ansi("PI2"))
	   {
		   auto t = OpenALRF::SensorDictionary::GetSensorTypeFromName(ASensor->getValue());

		   BValue SensorID;
		   SensorID.setString(ASensor);

		   auto sensor = PI1.GetSensorRelayByID(SensorID.asInteger());
		   if (sensor != nullptr)
		   {
			   sensor->Unsubscribe(this);
		   }
	   }
   }

   void History(const String *ASystem, const String *ASensor)
   {
      /*
         struct wslay_event_msg msgarg = {
            OpCode, (const uint8_t *)json.getValue(), json.getLength()
         };

         wslay_event_queue_msg(ctx, &msgarg);
      */
   }


   static ssize_t send_callback(wslay_event_context_ptr ctx, const uint8_t *data, size_t len, int flags, void *user_data)
   {
      RelayConnection *session = (RelayConnection*)user_data;
      ssize_t r;

      r = len;
      String strdata(data, len);

      CommReturnData errData;

      session->socket->send(&strdata, &errData);

      if (errData.error)
      {
         wslay_event_set_error(ctx, WSLAY_ERR_CALLBACK_FAILURE);
         return -1;
      }

      return r;
   }

   static ssize_t recv_callback(wslay_event_context_ptr ctx, uint8_t *buf, size_t len, int flags, void *user_data)
   {
      RelayConnection *session = (RelayConnection*)user_data;
      ssize_t r;

      session->sMessage->setSize(len);
      session->sMessage->setLength(0);

      if (session->socket->receive(session->sMessage)) {
         if (session->sMessage->getLength() > 0) {
            if (session->sMessage->getLength() > len)
            {
               // help we have a problem
               return -1;
            }

            memcpy(
               buf,
               session->sMessage->getPointer(0),
               session->sMessage->getLength()
            );
         }

         return session->sMessage->getLength();
      }

      wslay_event_set_error(ctx, WSLAY_ERR_CALLBACK_FAILURE);

      return -1;
   }

   static void on_msg_recv_callback(wslay_event_context_ptr ctx, const struct wslay_event_on_msg_recv_arg *arg, void *user_data)
   {
      /* Echo back non-control message */
      if (!wslay_is_ctrl_frame(arg->opcode))
      {
         RelayConnection *session = (RelayConnection*)user_data;
         
         String ReceivedMsg(arg->msg, arg->msg_length);
         
         session->newOpenMCTMessageReceived(&ReceivedMsg, arg->opcode);
      }
   }

   virtual void execute() override
   {
      if (WebSocketMode)
      {
         if (!(wslay_event_want_read(ctx) || wslay_event_want_write(ctx)))
         {
            stop();
         }
         else
         {
            if (!wslay_event_recv(ctx))
            {
               stop();
            }
            else
            {
               if (!wslay_event_send(ctx))
               {
                  stop();
               }
            }
         }
      }
      else
      {
         ThreadedConnection::execute();
      }
   }

   void newOpenMCTMessageReceived(const String * DeflatedMessage, const uint8_t OpCode)
   {
      if (DeflatedMessage->startsWith_ansi("subscribe "))
      {
         String Argument(DeflatedMessage->getPointer(10), DeflatedMessage->getLength() - 10);
         Vector Arguments = Groundfloor::split_m(&Argument, ".");
         if (Arguments.size() >= 2)
         {
            Subscribe(reinterpret_cast<String *>(Arguments.elementAt(0)), reinterpret_cast<String *>(Arguments.elementAt(1)));
         }
      }
      else if (DeflatedMessage->startsWith_ansi("unsubscribe "))
      {
         String Argument(DeflatedMessage->getPointer(12), DeflatedMessage->getLength() - 12);
         Vector Arguments = Groundfloor::split_m(&Argument, ".");
         if (Arguments.size() >= 2)
         {
            Unsubscribe(reinterpret_cast<String *>(Arguments.elementAt(0)), reinterpret_cast<String *>(Arguments.elementAt(1)));
         }
      }
      else if (DeflatedMessage->startsWith_ansi("dictionary"))
      {
         String json;
         json.append_ansi("{\"type\":\"dictionary\",\"value\":{\"name\":\"Example Spacecraft\",\"identifier\":\"sc\",\"subsystems\":[");
         json.append(GetJSONDictionary());
         json.append_ansi("]}}");

         struct wslay_event_msg msgarg = {
            OpCode, (const uint8_t *)json.getValue(), json.getLength()
         };

         wslay_event_queue_msg(ctx, &msgarg);
         wslay_event_send(ctx);
      }
      else if (DeflatedMessage->startsWith_ansi("history "))
      {
         String Argument(sMessage->getPointer(8), sMessage->getLength() - 8);
         Vector Arguments = Groundfloor::split_m(&Argument, ".");
         if (Arguments.size() >= 2)
         {
            History(reinterpret_cast<String *>(Arguments.elementAt(0)), reinterpret_cast<String *>(Arguments.elementAt(1)));
         }
      }
   }

   virtual void newMessageReceived(const String * sMessage) override
   {
	  if (sMessage->startsWith_ansi("GET / HTTP/1.1"))
	  {
		  JumpropesCommon::HttpHeader header;
		  header.parse(sMessage);
		  BValue *websocketkey = static_cast<BValue *>(header.allvars.getObjectByString("Sec-WebSocket-Key"));
		  if (websocketkey != nullptr)
		  {
			  String Response;
			  Response.append_ansi("HTTP/1.1 101 Switching Protocols\r\n");
			  Response.append_ansi("Upgrade: websocket\r\n");
			  Response.append_ansi("Connection: Upgrade\r\n");

			  auto base64key = websocketkey->asString();
			  base64key->append_ansi("258EAFA5-E914-47DA-95CA-C5AB0DC85B11");

			  auto hash = sha1raw(base64key->getValue());

			  String Input, Output;

			  Input.setValue(hash);
			  Groundfloor::StrToBase64(&Input, &Output);

			  Response.append_ansi("Sec-WebSocket-Accept: ");
			  Response.append(&Output);
			  Response.append_ansi("\r\n\r\n");

           WebSocketMode = true;

			  Groundfloor::CommReturnData err;
			  this->socket->send(&Response, &err);

           wslay_event_context_server_init(&ctx, &callbacks, this);
		  }
	  }
   }

   std::string GetSensorMainIdentifier(const OpenALRF::SensorBusData3D ABusData)
   {
      return OpenALRF::SensorDictionary::GetSensorName(ABusData.Type);
   }

   std::string GetJSONDictionaryEntry(const std::string APrefix, const std::string ACustomSensorName, std::string ADataId, OpenALRF::sensorunit_t AUnit)
   {
      std::string json;

      json += "{";
      json += "\"name\": \"";
      json += APrefix;
      json += ACustomSensorName;
      json += " ";
      json += ADataId;
      json += "\", ";
      json += "\"identifier\": \"";
      json += APrefix;
      json += ACustomSensorName + ".";
      json += ADataId;
      json += "\", ";
      json += "\"units\": \"";
      json += OpenALRF::SensorDictionary::GetUnitSymbol(AUnit);
      json += "\", ";
      json += "\"type\": \"float\"";
      json += "}";

      return json;
   }

   std::string GetJSONDictionary()
   {
      std::string json;

	  AddSensorDictionary(json, "PI1.", PI1.sensors);
	  AddSensorDictionary(json, "PI2.", PI2.sensors);

      return json;
   }

   void AddSensorDictionary(std::string &json, const std::string prefix, const std::vector<AudacityRover::ProxySensorRelay *> sensors)
   {
	   for (auto sensor : sensors)
	   {
		   auto sensorid = std::to_string(sensor->GetIdentifier());

		   if (json != "")
		   {
			   json += ",";
		   }

		   json += GetJSONDictionaryEntry(prefix, sensorid, "x", sensor->GetUsedUnit());
		   json += ",";
		   json += GetJSONDictionaryEntry(prefix, sensorid, "y", sensor->GetUsedUnit());
		   json += ",";
		   json += GetJSONDictionaryEntry(prefix, sensorid, "z", sensor->GetUsedUnit());
	   }
   }

   std::string GetDataJSONTemplate()
   {
      return
         "{" \
         "\"type\": \"{type}\"," \
         "\"id\": \"{id}\"," \
         "\"value\": {" \
         "\"timestamp\":\"{timestamp}\"," \
         "\"value\":\"{value}\"" \
         "}" \
         "}";
   }

   std::string GetDataJSON(const std::string AType, const OpenALRF::SensorBusData3D ABusData, int ADataNumber)
   {
      String repl(GetDataJSONTemplate());
      repl.replace_ansi("{type}", AType.c_str());

	  auto id = std::to_string(ABusData.ID);

	  auto sensor = PI1.GetSensorRelayByID(ABusData.ID);
	  if (sensor != nullptr)
	  {
		  id = "PI1." + id;
	  }
	  else
	  {
		  sensor = PI2.GetSensorRelayByID(ABusData.ID);
		  if (sensor != nullptr)
		  {
			  id = "PI2." + id;
		  }
		  else
		  {
			  id = "Unknown." + id;
		  }
	  }

      if (ADataNumber == 0)
      {
         id += ".value";
      }
      else if (ADataNumber == 1)
      {
         id += ".x";
      }
      else if (ADataNumber == 2)
      {
         id += ".y";
      }
      else if (ADataNumber == 3)
      {
         id += ".z";
      }

      repl.replace_ansi("{id}", id.c_str());

      auto ts = ABusData.Data.Timestamp * 1000;
      Groundfloor::BValue val;
      val.setInt64(ts);

      repl.replace_ansi("{timestamp}", val.asString()->getValue());

      if (ADataNumber == 0)
      {
         val.setDouble(ABusData.Data.Data1);
      }
      else if (ADataNumber == 1)
      {
         val.setDouble(ABusData.Data.Data1);
      }
      else if (ADataNumber == 2)
      {
         val.setDouble(ABusData.Data.Data2);
      }
      else if (ADataNumber == 3)
      {
         val.setDouble(ABusData.Data.Data3);
      }
      repl.replace_ansi("{value}", val.asString()->getValue());

      return repl.getValue();
   }

   virtual void NewSensorData(const OpenALRF::SensorBusData3D ABusData) override
   {
      Groundfloor::CommReturnData err;

	  Groundfloor::String JSONData;
	  JSONData.append(GetDataJSON("data", ABusData, 1));
	  JSONData.append_ansi(",");
	  JSONData.append(GetDataJSON("data", ABusData, 2));
	  JSONData.append_ansi(",");
	  JSONData.append(GetDataJSON("data", ABusData, 3));

      this->socket->send(&JSONData, &err);
   }
};

class RelayListener : public Jumpropes::ThreadedServer
{
protected:
   //   Jumpropes::ClientSocket OpenMCT;
   OpenALRF::SensorBus *Bus;
   std::vector<RelayConnection *> Connections;

public:
   RelayListener(OpenALRF::SensorBus *aBus) : ThreadedServer()
   {
      Bus = aBus;
      Connections.reserve(5);

	  this->Bus->Subscribe(&PI1);

      startListening(60812);
      /*
      Jumpropes::LookupObject Lookup;
      Groundfloor::String Address("127.0.0.1");

      if (Jumpropes::JRresolveIP(&Address, &Lookup))
      {
      OpenMCT.getRemoteAddress()->setValue(Lookup.getAddress(0));
      OpenMCT.connect();
      }
      else
      {
      std::cerr << "Err" << std::endl;
      }
      */
   }

   virtual void newClientConnection(Jumpropes::BaseSocket * aClient) override
   {
      auto connection = new RelayConnection(Bus, aClient);
      Connections.push_back(connection);
   }
};
