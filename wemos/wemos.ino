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
void kitchenLightsOn();
void kitchenLightsOff();

// Change this before you flash
const char* ssid = "YourSSID";
const char* password = "****************";

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *Deckenlicht = NULL;
Switch *kitchen = NULL;

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
    kitchen = new Switch("kitchen lights", 81, kitchenLightsOn, kitchenLightsOff);

    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*Deckenlicht);
    upnpBroadcastResponder.addDevice(*kitchen);
  }
}
 
void loop()
{
	 if(wifiConnected){
      upnpBroadcastResponder.serverLoop();
      
      kitchen->serverLoop();
      Deckenlicht->serverLoop();
	 }
}

void DeckenlichtAn() {
    Serial.print("Switch 1 turn on ...");
    mySwitch.switchOn("140690", "00010"); //Deckenlicht Anschaltcode
    delay(1000);
}

void DeckenlichtAus() {
    Serial.print("Switch 1 turn off ...");
    mySwitch.switchOn("030392", "00010"); //Deckenlicht Abschaltcode
    delay(1000);
}

void kitchenLightsOn() {
    Serial.print("Switch 2 turn on ...");
}

void kitchenLightsOff() {
  Serial.print("Switch 2 turn off ...");
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
