//CDAC_SUMMER INTERNSHIP PROJECT
//Program for "Wireless switch using nodeMCU and MQTT"
//Date:08/07/2016
 
//----------------------------------------------------//

#define _Func_
#include "project.h"
#undef _Func_

//#define _DEBUG_

void setup() {
  set_up();             //set up pin modes of LEDs and push buttons
  wifi_setup();         //connect to wifi
  connectToClient();    //connect the mqtt client to the broker/server of mqtt and enables interrupts to button pins.
}

void loop() {
  checkClient();             //check the status of client: connected/not connected. If not connected, connect the client.
  if (inTerrupt)             //If there is any interrupt at button pins, call the function to handle it.
    interruptHandler();
  digitalWrite(Led, r);
}
