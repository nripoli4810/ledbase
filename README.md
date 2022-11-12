# ledbase

## Base

Components: 
* Adafruit Huzzah Feather Board ESP8266
* NeoPixel v2 (x3)

Purpose: Display data from the mqtt feed

## MQTT Broker

MQTT Broker hosted in docker

Require creation of a `config`, `data`, and `logs` folder in the same directory as the docker-compose file

Requires creation of `mosquitto.config` file (example provided, read reference article for more information)

References:
* https://hometechhacker.com/mqtt-home-assistant-using-docker-eclipse-mosquitto/