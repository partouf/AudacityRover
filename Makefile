# Project: AudacityRover

OBJ_COMMUNICATION = Communication/CommunicationJumpropes.o Communication/SensorDataReceiver.o Communication/SensorDataTransmitter.o
OBJ_COMMAND = Command/CommandQueueDefault.o
OBJ_PILOT = Pilot/RemotePilotGoPiGo.o
OBJ_CAMERA = Camera/CameraRaspi.o
OBJ_SENSORS = Sensors/DummySensor.o Sensors/SenseHATSensor.o Sensors/AccelerometerSenseHAT.o Sensors/MagnometerSenseHAT.o Sensors/GyroscopeSenseHAT.o Sensors/TemperatureSenseHAT.o
OBJ_SYSTEM = System/WatchCat.o System/SystemAudacity.o System/Logging.o System/Modules.o System/Configuration.o
OBJ_MAIN = main.o

OBJ = $(OBJ_COMMUNICATION) $(OBJ_COMMAND) $(OBJ_PILOT) $(OBJ_CAMERA) $(OBJ_SENSORS) $(OBJ_SYSTEM) $(OBJ_MAIN)
LINKOBJ = $(OBJ)
LIBS = -L"./lib/GoPiGo" -L"./lib/Crosscables/libGroundfloor" -L"./lib/Crosscables/libJumpropes" -L"./lib/OpenALRF" -L"./lib/raspicam/build/src" -L"./lib/cpp-sense-hat/lib" -lGoPiGo -lOpenALRF -lJumpropes -lGroundfloor -lraspicam -lsense-hat -lRTIMULib -lpthread -lrt
BIN  = AudacityRover
CXXINCS = -I"./lib" -I"./lib/Crosscables/include" -I"./lib/cpp-sense-hat/src"
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

Sensors/DummySensor.o: Sensors/DummySensor.cpp
	$(CXX) -c Sensors/DummySensor.cpp -o Sensors/DummySensor.o $(CXXFLAGS)

Sensors/AccelerometerSenseHAT.o: Sensors/AccelerometerSenseHAT.cpp
	$(CXX) -c Sensors/AccelerometerSenseHAT.cpp -o Sensors/AccelerometerSenseHAT.o $(CXXFLAGS)

Sensors/SenseHATSensor.o: Sensors/SenseHATSensor.cpp
	$(CXX) -c Sensors/SenseHATSensor.cpp -o Sensors/SenseHATSensor.o $(CXXFLAGS)

Sensors/MagnometerSenseHAT.o: Sensors/MagnometerSenseHAT.cpp
	$(CXX) -c Sensors/MagnometerSenseHAT.cpp -o Sensors/MagnometerSenseHAT.o $(CXXFLAGS)

Sensors/TemperatureSenseHAT.o: Sensors/TemperatureSenseHAT.cpp
	$(CXX) -c Sensors/TemperatureSenseHAT.cpp -o Sensors/TemperatureSenseHAT.o $(CXXFLAGS)

Sensors/GyroscopeSenseHAT.o: Sensors/GyroscopeSenseHAT.cpp
	$(CXX) -c Sensors/GyroscopeSenseHAT.cpp -o Sensors/GyroscopeSenseHAT.o $(CXXFLAGS)

System/Logging.o: System/Logging.cpp
	$(CXX) -c System/Logging.cpp -o System/Logging.o $(CXXFLAGS)

main.o: main.cpp
	$(CXX) -c main.cpp -o main.o $(CXXFLAGS)
