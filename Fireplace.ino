#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
// #include <Arduino.h>
#include "fauxmoESP.h"

//for LED status
#include <Ticker.h>
Ticker ticker;
#define MOTOR_SEC 8
long fire_timer = 0;
long fire_timeout = 1000 * 60 *60 * 2;     // 2 hour timer
bool fire = false;
fauxmoESP fauxmo;

void tick()
{
  //toggle state
  int state = digitalRead(BUILTIN_LED);  // get the current state of GPIO1 pin
  digitalWrite(BUILTIN_LED, !state);     // set pin to the opposite state
}

// call to disable motor after some time

void motorOff() {
  digitalWrite(D1,LOW);
  digitalWrite(D2,LOW);
  digitalWrite(D3,LOW);
  digitalWrite(D4,LOW);
  digitalWrite(BUILTIN_LED, HIGH);
  ticker.detach();
  Serial.println("MOTOR: 8 second request complete, turning off motor");
}


//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  //entered config mode, make led toggle faster
  ticker.attach(0.2, tick);
}

void setup() {
  pinMode(D1,OUTPUT);
  pinMode(D2,OUTPUT);
  pinMode(D3,OUTPUT);
  pinMode(D4,OUTPUT);
  digitalWrite(D1,HIGH);
  digitalWrite(D3,LOW);
  delay(MOTOR_SEC * 1000);
  digitalWrite(D1,LOW);
  digitalWrite(D3,LOW);     
  // put your setup code here, to run once:
  Serial.begin(115200);
  // perform fire place turn off (in case something terrible happened)
   Serial.println("OFF");
    
  //set led pin as output
  pinMode(BUILTIN_LED, OUTPUT);
  // start ticker with 0.5 because we start in AP mode and try to connect
  ticker.attach(0.6, tick);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  //reset settings - for testing
  //wifiManager.resetSettings();

  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("Fireplace Config")) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  ticker.detach();
  //keep LED on
  digitalWrite(BUILTIN_LED, LOW);
    fauxmo.createServer(true);
    fauxmo.setPort(80);
    fauxmo.enable(true);
    fauxmo.addDevice("fireplace");
   // fauxmo.addDevice("pixels");
    fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
      Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
  if (state) {
    if(fire == false) { 
    Serial.print("WeMo: ON..");
    digitalWrite(D1,HIGH);
    digitalWrite(D3,HIGH);
    digitalWrite(BUILTIN_LED, LOW);
    ticker.attach(MOTOR_SEC,motorOff);
    Serial.println("COMPLETE.");
    fire = true;
    fire_timer = fire_timeout + millis();
  } }
  else {
    if(fire == true) { 
    Serial.print("WeMo: OFF..");
    digitalWrite(D1,HIGH);
    digitalWrite(D3,LOW);
    digitalWrite(BUILTIN_LED, LOW);
    ticker.attach(MOTOR_SEC,motorOff);
    Serial.println("COMPLETE.");
    fire = false;    
  } }
    });
}

void loop() {
  // put your main code here, to run repeatedly:
  fauxmo.handle();
  if(fire == true) { 
     if(fire_timer < millis()) { turnItOff(); } 
     if(millis() < 8000) { turnItOff(); }  // handle millis rollover in a mean way
     }
}

void callback(uint8_t device_id, const char * device_name, bool state) {
  Serial.print("Device "); Serial.print(device_name); 
  Serial.print(" state: ");
  if (state) {
    
    Serial.print("WeMo: ON..");
    digitalWrite(D1,HIGH);
    digitalWrite(D3,HIGH);
    digitalWrite(BUILTIN_LED, LOW);
    ticker.attach(MOTOR_SEC,motorOff);
    Serial.println("COMPLETE.");
    fire = true;
    fire_timer = fire_timeout + millis();
  } else {
    Serial.print("WeMo: OFF..");
    digitalWrite(D1,HIGH);
    digitalWrite(D3,LOW);
    digitalWrite(BUILTIN_LED, LOW);
    ticker.attach(MOTOR_SEC,motorOff);
    Serial.println("COMPLETE.");
    fire = false;    
  }
}

void turnItOff() { 
    Serial.print("Timer: TIMEOUT EXCEEDED----OFF..");
    digitalWrite(D1,HIGH);
    digitalWrite(D3,LOW);
    digitalWrite(BUILTIN_LED, LOW);
    ticker.attach(MOTOR_SEC,motorOff);
    Serial.println("COMPLETE.");
    fire = false;    
}
