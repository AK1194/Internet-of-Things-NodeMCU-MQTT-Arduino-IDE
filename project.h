#ifndef _project_h
#define _project_h
#define _DEBUG_
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Client.h>
#include <Arduino.h>
void set_up(void);
void interruptHandler(void);
void checkClient(void);
void offButton(void);
void onButton(void);
void timer0_ISR (void);
void timer0_ISRforHold(void);
void timer0_ISRforJitter(void);
void buttonjustpressed(void);
void wifi_setup();
void connectToClient();
void clientConnected();
void callback(char* topic, byte* payload, unsigned int length);


#ifdef _Func_
int red = 5;                               //LEDs pin number
int green = 4;
int Led = 12;

int WifiRetries = 1000;                    //number of retries attempted to connect to the wifi
int ClientRetries = 25;                    //number of retries to connect to the MQTT server



//-----------------------------OPEN WiFi--------------------------------//
char ssid[] = "CDAC";                         //WiFi SSID
char wifipswd[] = "";                         //No wifi password
//------------------PASSWORD PROTECTED WiFi----------------------------//
//char ssid[] = "Archana";                      
//char wifipswd[] = "ARYANPIYUSH"; 
//----------------------------------------------------------------------// 
  


//--------------------------MQTT SERVER--------------------------------//
//IPAddress ip(192,168,43,107);              //MQTT server ip
//IPAddress ip(10,208,6,98);                   

char clientID[] = "ESP8266Client";         //Client ID of the mqtt client
char username[]= "";           //Username and paswd to connect to the MQTT Broker
char password[] = "";

char subjectON[]="Building/Floor1/Pi1/Ctrl";                     //Topics specified for message publish for ON/OFF Button
char subjectOFF[]="Building/Floor1/Pi1/Ctrl";                 

int shortPressMsg=100;                     //Payload or msg to be published on a short press event
int longPressMsg=0;                        //Payload for a long Press release event
int shortPressON=100;
int shortPressOFF=0;
int longPressON=10;
int longPressOFF=5;
char msg[10];
//----------------------------------------------------------------------//

bool r=false;
volatile int buton;
volatile boolean inTerrupt = false;
volatile boolean locked = false;
volatile boolean hold = false;
volatile boolean released = false;
volatile int holdFlag = 0;
WiFiClient esp;
PubSubClient client(esp);
#else
extern int red;
extern int green;
extern int Led;
extern bool r;
extern int WifiRetries;
extern int ClientRetries;
//extern IPAddress ip;
extern char ssid[];
extern char wifipswd[];
extern char clientID[];
extern char username[];
extern char password[];
extern char subjectON[];
extern char subjectOFF[];
extern int shortPressMsg;
extern int longPressMsg;
extern int shortPressON;
extern int shortPressOFF;
extern int longPressON;
extern int longPressOFF;
extern char msg[10];
extern volatile int buton;
extern volatile boolean inTerrupt;
extern volatile boolean locked;
extern volatile boolean hold;
extern volatile boolean released;
extern volatile int holdFlag;
extern WiFiClient esp;
extern PubSubClient client;
#endif
#endif
