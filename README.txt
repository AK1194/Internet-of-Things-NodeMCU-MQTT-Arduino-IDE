=====================================Quickstart===============================
Following entities are to be configured prior to upload the program into the board:
* For WiFi connection:
=>WiFi ssid only (if open network)
=>WiFi ssid and WiFipswd (encrypted Wifi)
*For MQTT connection:
=>ClientID, (Username and password, if any)
=>Topic name, MessageToPublish(Payload){
--------------------------------------------------------------------------------------------------------
These above configurations can be made by editing the Project.h file
--------------------------------------------------------------------------------------------------------

Optional Settings:
* Number of retries to attempt connection to WiFi and MQTT server.

*For Debugging, enable DEBUG mode  by uncommenting #define _DEBUG_ in the FinalProject.ino file



 