#pragma once

#include "../Sensors/DummySensor.h"
#include "../Sensors/AccelerometerSenseHAT.h"
#include "../Sensors/GyroscopeSenseHAT.h"
#include "../Sensors/MagnometerSenseHAT.h"
#include "../Sensors/SystemTempSensor.h"
#include "../Sensors/TemperatureSenseHAT.h"

#include <cgreen/cgreen.h>
#include <sstream>
#include <iostream>
#include <cstdlib>

using namespace cgreen;
using namespace AudacityRover;

Describe(AudacityTests);
BeforeEach(AudacityTests) {}
AfterEach(AudacityTests) {}

Ensure(AudacityTests, DummyHappyPath) {
   DummySensor Sensor(1);

   assert_that(Sensor.IsPowered());

   assert_that(Sensor.GetIdentifier() == 1);
   assert_that(Sensor.LatestValue().Timestamp == 0);
   assert_that(Sensor.GetStatusInfo().find("<LatestSensorData />") != 0);

   OpenALRF::Sensor3DData Data;
   assert_that(Sensor.NextValue(Data));

   assert_that(Data.Timestamp > 0);
   assert_that(Sensor.GetStatusInfo().find("<LatestSensorData>") != 0);
   assert_that(Sensor.LatestValue().Timestamp == Data.Timestamp);
}

Ensure(AudacityTests, AccelLatestValue) {
   AccelerometerSenseHAT Accel(1);
   OpenALRF::Sensor3DData Data;

   assert_that(Accel.LatestValue().Timestamp == 0);

   while (!Accel.NextValue(Data));
   assert_that(Accel.LatestValue().Timestamp != 0);
}

Ensure(AudacityTests, Serialization) {
   DummySensor Sensor(1);
   OpenALRF::Sensor3DData Data;
   assert_that(Sensor.NextValue(Data));

   std::stringstream stream;
   stream << Data;

   std::string str = stream.str();
   const unsigned char *buffer = reinterpret_cast<const unsigned char *>(str.c_str());

   // is probably going to test false on ARM?

   assert_equal(str.length(), 32);
   assert_equal(buffer[7], ((Data.Timestamp >> 56) & 0xff));
   assert_equal(buffer[6], ((Data.Timestamp >> 48) & 0xff));
   assert_equal(buffer[5], ((Data.Timestamp >> 40) & 0xff));
   assert_equal(buffer[4], ((Data.Timestamp >> 32) & 0xff));
   assert_equal(buffer[3], ((Data.Timestamp >> 24) & 0xff));
   assert_equal(buffer[2], ((Data.Timestamp >> 16) & 0xff));
   assert_equal(buffer[1], ((Data.Timestamp >> 8) & 0xff));
   assert_equal(buffer[0], ((Data.Timestamp) & 0xff));

   int64_t *p = reinterpret_cast<int64_t *>(&(Data.Data1));
   int64_t dint = *p;
   assert_equal(buffer[15], ((dint >> 56) & 0xff));
   assert_equal(buffer[14], ((dint >> 48) & 0xff));
   assert_equal(buffer[13], ((dint >> 40) & 0xff));
   assert_equal(buffer[12], ((dint >> 32) & 0xff));
   assert_equal(buffer[11], ((dint >> 24) & 0xff));
   assert_equal(buffer[10], ((dint >> 16) & 0xff));
   assert_equal(buffer[9], ((dint >> 8) & 0xff));
   assert_equal(buffer[8], ((dint) & 0xff));

   p = reinterpret_cast<int64_t *>(&(Data.Data2));
   dint = *p;
   assert_equal(buffer[23], ((dint >> 56) & 0xff));
   assert_equal(buffer[22], ((dint >> 48) & 0xff));
   assert_equal(buffer[21], ((dint >> 40) & 0xff));
   assert_equal(buffer[20], ((dint >> 32) & 0xff));
   assert_equal(buffer[19], ((dint >> 24) & 0xff));
   assert_equal(buffer[18], ((dint >> 16) & 0xff));
   assert_equal(buffer[17], ((dint >> 8) & 0xff));
   assert_equal(buffer[16], ((dint) & 0xff));

   p = reinterpret_cast<int64_t *>(&(Data.Data3));
   dint = *p;
   assert_equal(buffer[31], ((dint >> 56) & 0xff));
   assert_equal(buffer[30], ((dint >> 48) & 0xff));
   assert_equal(buffer[29], ((dint >> 40) & 0xff));
   assert_equal(buffer[28], ((dint >> 32) & 0xff));
   assert_equal(buffer[27], ((dint >> 24) & 0xff));
   assert_equal(buffer[26], ((dint >> 16) & 0xff));
   assert_equal(buffer[25], ((dint >> 8) & 0xff));
   assert_equal(buffer[24], ((dint) & 0xff));
}

int DoAudacityTests(int argc, char ** argv)
{
   TestSuite *suite = create_test_suite();
   add_test_with_context(suite, AudacityTests, DummyHappyPath);
   add_test_with_context(suite, AudacityTests, AccelLatestValue);
   add_test_with_context(suite, AudacityTests, Serialization);

   if (argc > 1) {
      return run_single_test(suite, argv[1], create_text_reporter());
   }

   return run_test_suite(suite, create_text_reporter());
}
