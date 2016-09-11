#!/bin/sh
find . -type f -print0 | xargs -0 dos2unix
cd lib/Crosscables/libGroundfloor && sh build.sh
cd ../libJumpropes && sh build.sh
cd ../../GoPiGo && make
cd ../OpenALRF && make
cd ../cpp-sense-hat && make all
cd ../raspicam && mkdir build
cd build && cmake .. && make
cd ../../.. && make clean && make
