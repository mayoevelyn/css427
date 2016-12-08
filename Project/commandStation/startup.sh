#!/bin/sh
cd /home/pi/css427/Project/webServer/sprinklerPage/
python runSprinklerPage.py &
cd /home/pi/css427/Project/commandStation/
python commandStation.py &
