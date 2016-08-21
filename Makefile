# Project: AudacityRover

OBJ_COMMUNICATION = Communication/CommunicationJumpropes.o Communication/SensorDataReceiver.o Communication/SensorDataTransmitter.o
OBJ_COMMAND = Command/CommandQueueDefault.o
OBJ_PILOT = Pilot/RemotePilotGoPiGo.o
OBJ_CAMERA = Camera/CameraRaspi.o
OBJ_SENSORS = 
OBJ_SYSTEM = System/WatchCat.o System/SystemAudacity.o System/Logging.o System/Modules.o System/Configuration.o
OBJ_MAIN = main.o

OBJ = $(OBJ_COMMUNICATION) $(OBJ_COMMAND) $(OBJ_PILOT) $(OBJ_CAMERA) $(SENSORS) $(OBJ_SYSTEM) $(OBJ_MAIN)
LINKOBJ = $(OBJ)
LIBS = -L"./lib/GoPiGo" -L"./lib/Crosscables/libGroundfloor" -L"./lib/Crosscables/libJumpropes" -L"./lib/OpenALRF" -lGoPiGo -lOpenALRF -lJumpropes -lGroundfloor -lpthread -lrt
BIN  = AudacityRover
CXXINCS = -I"./lib" -I"./lib/Crosscables/include"
CXXFLAGS = $(CXXINCS) -fexceptions -std=c++11 -O2
RM = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after

clean:
	${RM} $(LINKOBJ) $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(LINKOBJ) -o "$(BIN)" $(LIBS)

Command/CommandQueueDefault.o: Command/CommandQueueDefault.cpp
	$(CXX) -c Command/CommandQueueDefault.cpp -o Command/CommandQueueDefault.o $(CXXFLAGS)

Pilot/RemotePilotGoPiGo.o: Pilot/RemotePilotGoPiGo.cpp
	$(CXX) -c Pilot/RemotePilotGoPiGo.cpp -o Pilot/RemotePilotGoPiGo.o $(CXXFLAGS)

Camera/CameraRaspi.o: Camera/CameraRaspi.cpp
	$(CXX) -c Camera/CameraRaspi.cpp -o Camera/CameraRaspi.o $(CXXFLAGS)

System/WatchCat.o: System/WatchCat.cpp
	$(CXX) -c System/WatchCat.cpp -o System/WatchCat.o $(CXXFLAGS)

System/SystemAudacity.o: System/SystemAudacity.cpp
	$(CXX) -c System/SystemAudacity.cpp -o System/SystemAudacity.o $(CXXFLAGS)

System/Modules.o: System/Modules.cpp
	$(CXX) -c System/Modules.cpp -o System/Modules.o $(CXXFLAGS)

System/Configuration.o: System/Configuration.cpp
	$(CXX) -c System/Configuration.cpp -o System/Configuration.o $(CXXFLAGS)

System/Logging.o: System/Logging.cpp
	$(CXX) -c System/Logging.cpp -o System/Logging.o $(CXXFLAGS)

main.o: main.cpp
	$(CXX) -c main.cpp -o main.o $(CXXFLAGS)
