# Internet-of-Things-NodeMCU-MQTT-Arduino-IDE
This repository is all about using NodeMCU devkit and MQTT protocol, for M2M communication in IoT.
The documentation for NodeMCU can be easily found on the internet https://nodemcu.readthedocs.io/en/master/
I have mostly tried to program the NodeMCU in C using Arduino IDE.
It is an IoT project for implementing wireless smart switches for home/office lights. It works on Publish and Subscribe model.
In Publish-Subscribe model, there is an MQTT broker set up and all the other devices connected to it are called the clients.
There are different MQTT broker setups available: HiveMQ, Mosquitto etc. And for the clients, other than the devices, we can use eclipse paho, myMqtt apps and many more.
To program the NodeMCU in Arduino IDE, one must save this link http://arduino.esp8266.com/stable/package_esp8266com_index.json in the additional boards manager url in the Prefernces of the IDE.
