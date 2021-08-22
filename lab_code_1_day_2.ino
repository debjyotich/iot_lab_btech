/***************************************************
  This is Consentium Inc's IoT library
  ----> http://consentiuminc.online/
  Check out the links above for our tutorials and product diagrams.

  This is Consentium Inc's IoT library works only for 5v tolerant Edge Beta boards 
 
  Written by Debjyoti Chowdhury for Consentium Inc.
  MIT license, all text above must be included in any redistribution
 ****************************************************/


#include <ConsentiumThings.h>
#include "DHT.h"

#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

ConsentiumThings link;   // create ConsentiumThing object
DHT dht(DHTPIN, DHTTYPE);

const char *ssid = "guest"; // add WiFi SSID
const char *pass = "guest123"; // add WiFi password
 
const char *ip = "consentiuminc.online";  // Consentium server address
const char *port = "80"; // Consentium server port
const char *key = "PHKKXV5B3UIX-SRFPZCPIQ";       // Write api key


void setup(){
  link.connect();   // init. IoT boad
  link.initWiFi(ssid, pass);  // begin WiFi connection
  dht.begin();
}

void loop(){
  float h = dht.readHumidity(); // humidity
  float t = dht.readTemperature(); // temp in C
  
  if (isnan(h) || isnan(t)){
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }   
  
  float sensor_val[] = {t, h};  // sensor data array
  String info[] = {"Temperature", "Humidity"}; // sensor info. array
  
  int sensor_num = sizeof(sensor_val)/sizeof(sensor_val[0]); // number of sensors connected 
  
  link.sendREST(ip, port, key, info, sensor_num, sensor_val); // send over REST
  delay(5000); // delay
}
