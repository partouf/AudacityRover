#!/bin/sh

cd lib/Crosscables/libGroundfloor && sh build.sh
cd ../libJumpropes && sh build.sh
cd ../../GoPiGo && make
cd ../OpenALRF && make
cd ../.. && make
