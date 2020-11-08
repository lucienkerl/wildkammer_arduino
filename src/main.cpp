#include <Arduino.h>
#include <ESP8266WiFiMulti.h>
#include <Wire.h>
#include <WiFiUdp.h>
#include <relaischeck.h>
#include <oled.h>
#include <webserver.h>
#include <temperature.h>
#include <influx.h>
#include <config.h>
#include <ArduinoOTA.h>

void setup()
{
  RELAYsetup();
  OLEDsetup();
  Serial.begin(115200);
  delay(2000);
  Serial.println("Connecting to ");
  Serial.println(WIFI_SSID);
  //connect to your local wi-fi network
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());

  ArduinoOTA.begin();
  WEBsetup();
}

void loop()
{
  ArduinoOTA.handle();
  TEMPloop();
  WEBloop();
  INFLUXloop();
  RELAYloop();
  OLEDloop();
}