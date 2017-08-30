#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"

#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch();

// prototypes
boolean connectWifi();

//on/off callbacks 
void DeckenlichtAn();
void DeckenlichtAus();
void Stecker1An();
void Stecker1Aus();
void Stecker2An();
void Stecker2Aus();
void Stecker3An();
void Stecker3Aus();
void Stecker4An();
void Stecker4Aus();

// Change this before you flash
const char* ssid = "SSID";
const char* password = "**************";

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *Deckenlicht = NULL;
Switch *Stecker1 = NULL;
Switch *Stecker2 = NULL;
Switch *Stecker3 = NULL;
Switch *Stecker4 = NULL;


void setup()
{
  Serial.begin(9600);
  
  
  mySwitch.enableTransmit(2); //Enable Transmitter at Port D4 (Wemos Mini)

  // Optional set protocol (default is 1, will work for most outlets)
  // mySwitch.setProtocol(2);

  // Optional set pulse length.
  // mySwitch.setPulseLength(320);
  
  // Optional set number of transmission repetitions.
  // mySwitch.setRepeatTransmit(15);
  
  
  // Initialise wifi connection
  wifiConnected = connectWifi();
  
  if(wifiConnected){
    upnpBroadcastResponder.beginUdpMulticast();
    
    // Define your switches here. Max 14
    // Format: Alexa invocation name, local port no, on callback, off callback
    Deckenlicht = new Switch("Deckenlicht", 80, DeckenlichtAn, DeckenlichtAus);
    Stecker1 = new Switch("Küche", 81, Stecker1An, Stecker1Aus);
    Stecker2 = new Switch("Musikanlage", 82, Stecker2An, Stecker2Aus);
    Stecker3 = new Switch("Kodi", 83, Stecker3An, Stecker3Aus);
    Stecker4 = new Switch("Router", 84, Stecker4An, Stecker4Aus);

    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*Deckenlicht);
    upnpBroadcastResponder.addDevice(*Stecker1);
    upnpBroadcastResponder.addDevice(*Stecker2);
    upnpBroadcastResponder.addDevice(*Stecker3);
    upnpBroadcastResponder.addDevice(*Stecker4);
  }
}
 
void loop()
{
	 if(wifiConnected){
      upnpBroadcastResponder.serverLoop();
      
      Stecker1->serverLoop();
      Stecker2->serverLoop();
      Stecker3->serverLoop();
      Stecker4->serverLoop();
      Deckenlicht->serverLoop();
	 }
}

void DeckenlichtAn() {
    Serial.print("Deckenlicht turn on ...");
    mySwitch.switchOn("140690", "00010"); //Deckenlicht Anschaltcode
    delay(1000);
}

void DeckenlichtAus() {
    Serial.print("Deckenlicht turn off ...");
    mySwitch.switchOn("030392", "00010"); //Deckenlicht Abschaltcode
    delay(1000);
}
//Küche
void Stecker1An() {
    Serial.print("Switch 1 turn on ...");
    mySwitch.send(83029, 24); //Deckenlicht Abschaltcode
    delay(1000);
}

void Stecker1Aus() {
  Serial.print("Switch 1 turn off ...");
    mySwitch.send(83028, 24); //Deckenlicht Abschaltcode
    delay(1000);
}

//Musikanlage
void Stecker2An() {
    Serial.print("Switch 2 turn on ...");
    mySwitch.send(86101,24); //Deckenlicht Abschaltcode
    delay(1000);
}

void Stecker2Aus() {
  Serial.print("Switch 2 turn off ...");
    mySwitch.send(86100,24); //Deckenlicht Abschaltcode
    delay(1000);
}
//Raspberry/kodi
void Stecker3An() {
    Serial.print("Switch 3 turn on ...");
    mySwitch.send(70741,24); //Deckenlicht Abschaltcode
    delay(1000);
}

void Stecker3Aus() {
  Serial.print("Switch 3 turn off ...");
    mySwitch.send(70740,24); //Deckenlicht Abschaltcode
    delay(1000);
}
//Router
void Stecker4An() {
    Serial.print("Switch 4 turn on ...");
    mySwitch.send(21589,24); //Deckenlicht Abschaltcode
    delay(1000);
}

void Stecker4Aus() {
  Serial.print("Switch 4 turn off ...");
    mySwitch.send(21588,24); //Deckenlicht Abschaltcode
    delay(1000);
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10){
      state = false;
      break;
    }
    i++;
  }
  
  if (state){
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  
  return state;
}
