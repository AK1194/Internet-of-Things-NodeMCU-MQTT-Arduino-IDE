#include "project.h"

void timer0_ISRforJitter(void) {                  //timer ISR for checking jitter.
  timer0_detachInterrupt();                       //detach this timer interrupt.
  buttonjustpressed();
}

void timer0_ISRforHold(void) {                 //Timer ISR for hold press
  timer0_write(ESP.getCycleCount() + 8000000);
  holdFlag = 1;
}

void timer0_ISR (void) {            //Timer ISR for hold press for 1sec
  timer0_detachInterrupt();
  if (digitalRead(buton)) {          //if button is at long press
    hold = true;
    inTerrupt = true;
    timer0_isr_init();
    timer0_attachInterrupt(timer0_ISRforHold);      //set a timer of 100ms to publish hold msg every 100ms
    timer0_write(ESP.getCycleCount() + 8000000);
  }
}

void buttonjustpressed(void) {
  if (digitalRead(buton)) {       //Rising edge of the button.
    hold = false;                 //set hold and released flags to false at rising edge
    released = false;
    timer0_isr_init();
    timer0_attachInterrupt(timer0_ISR);             //call a timer interrupt of 1sec to check if button is at long press or short press.
    timer0_write(ESP.getCycleCount() + 80000000); // 80MHz == 1sec
  }
  else {                            //falling edge of the button
    timer0_detachInterrupt();
    inTerrupt = true;
    released = true;
  }
}

void onButton() {
  if (!digitalRead(13) && digitalRead(14) && !locked) {     //once entertaining an interrupt of a button, do not entertain any interrupt for other button
    locked = true;
    buton = 14;
    timer0_isr_init();
    timer0_attachInterrupt(timer0_ISRforJitter);      //attach a timer to check the button state to avoid jitter
    timer0_write(ESP.getCycleCount() + 1600000);          //20ms timer interrupt.
  }
  else if (locked && buton == 14) {
    timer0_isr_init();
    timer0_attachInterrupt(timer0_ISRforJitter);
    timer0_write(ESP.getCycleCount() + 1600000);
  }
}

void offButton() {
  if (!digitalRead(14) && digitalRead(13) && !locked) {                       //once entertaining an interrupt of a button, do not entertain any interrupt for other button
    locked = true;
    buton = 13;
    timer0_isr_init();
    timer0_attachInterrupt(timer0_ISRforJitter);              //20ms timer interrupt.
    timer0_write(ESP.getCycleCount() + 1600000);
  }
  else if (locked && buton == 13) {
    timer0_isr_init();
    timer0_attachInterrupt(timer0_ISRforJitter);
    timer0_write(ESP.getCycleCount() + 1600000);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {      //Callback function at client side
  char message[length];
  Serial.print("Topic:[");
  Serial.print(topic);
  Serial.print("]:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void wifi_setup() {               //to connect to the WiFi.
  noInterrupts();
  digitalWrite(red, LOW);
#ifdef _DEBUG_
  Serial.println();
  Serial.print("Connecting to :");
  Serial.print(ssid);
#endif
  if (wifipswd == NULL) {                  //if there an open wifi, no pswd required.
    WiFi.begin(ssid);
  }
  else if (wifipswd != NULL) {                // pswd is required for encrypted wifi networks
    WiFi.begin(ssid, wifipswd);
  }
  int i = WifiRetries;
  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
    Serial.print(".");
    i = i - 1;
    if (i <= 0) {
#ifdef _DEBUG_
      Serial.println();
      Serial.println(WiFi.status());
      Serial.println("Trying again after reset...");
#endif
      ESP.restart();                 // restart the device if maximum number of specified retries is reached
      //      i = WifiRetries;
    }
  }
  digitalWrite(red, HIGH);
#ifdef _DEBUG_
  Serial.println();
  Serial.print("WiFi Connected in retries: ");
  Serial.println(WifiRetries - i + 1);
  Serial.print("IP address :");
  Serial.println(WiFi.localIP());
#endif
}

void clientConnected() {               //Check for encryption type to connect to the MQTT broker
  if (password == NULL)
    client.connect(clientID);
  else if (password != NULL)
    client.connect(clientID, username, password);
}

void connectToClient() {          // After successful wifi connection, connect this device/client to the MQTT broker/server.
  noInterrupts();
  digitalWrite(green, LOW);
  client.setServer("mqtt.pune.cdac.in", 1883);
  Serial.print("Attempting connection to the mqtt broker...");
  int j = ClientRetries;
  while (!client.connected()) {
    clientConnected();
    if (client.connected()) {
      Serial.print("connected to the broker ...IN ");
      Serial.println(ClientRetries - j + 1);
      client.subscribe(subjectON);               //subscribe to the topics after connecting
      client.subscribe(subjectOFF);
      digitalWrite(green, HIGH);
      client.setCallback(callback);
    }
    else
    {
      delay(5);
      Serial.print(".");
#ifdef _DEBUG_
      //      Serial.print("failed, rc=");
      //      Serial.println(client.state());
#endif

      if (j <= 0) {
#ifdef _DEBUG_
        Serial.println();
        Serial.print("WiFi status : ");
        Serial.println(WiFi.status());
        Serial.print("MQTT client status : ");
        Serial.println(client.state());
        Serial.println("Please check the MQTT Broker");
        Serial.println("Trying again after reset...");
#endif
        ESP.restart();
      }
      j = j - 1;
    }
  }
  attachInterrupt(digitalPinToInterrupt(14), onButton, CHANGE);
  attachInterrupt(digitalPinToInterrupt(13), offButton, CHANGE);
  interrupts();
}

void set_up() {   //initialise all the pins to their modes and enable serial port.
  noInterrupts();
  Serial.begin(115200);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(Led, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(14, INPUT);          // pin 13 and pin 14 are push buttons for ON and OFF respectively.
  pinMode(13, INPUT);

}

void checkClient() {                 //Check client connections
  if (!client.connected()) {          //if client is not connected, gracefully disconnect it and again connect it
    client.disconnect();
    if (WiFi.status() != WL_CONNECTED) {
      WiFi.disconnect();
      wifi_setup();
    }
    connectToClient();
  }
  client.loop();
  r = false;
}

void interruptHandler() { 
  //if interrupt is there publish msg according to the states.
  char* subject=NULL;
  r = false;
  if (!hold && released) {            //publish msg for short press
    inTerrupt = false;
    if (buton == 14){
      subject = subjectON;
      shortPressMsg=shortPressON;
    }
    else if (buton == 13){
      subject = subjectOFF;
      shortPressMsg=shortPressOFF;
    }
    snprintf(msg, 10, "%d", shortPressMsg);
    r = client.publish(subject, msg);
    locked = false;
  }
  else if (hold && released) {                    //publish msg for long press released.
    inTerrupt = false;
    if (buton == 14){
      subject = subjectON;
      longPressMsg=longPressON;
    }
    else if (buton == 13){
      subject = subjectOFF;
      longPressMsg=longPressOFF;
    }
      snprintf(msg, 10,"%d",longPressMsg);
    //r = client.publish(subject, msg);              //do not publish for long press release.
    locked = false;
  }
  else if (hold && !released && holdFlag == 1) {              //publish msg for hold state periodically
    if (buton == 14) {
      snprintf(msg, 10, "Dim Up");
      subject = subjectON;
    }
    else if (buton == 13) {
      snprintf(msg, 10, "Dim Down");
      subject = subjectOFF;
    }
    r = client.publish(subject, msg);
    holdFlag = 0;
  }

}

