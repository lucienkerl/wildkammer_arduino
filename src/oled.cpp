#include <Wire.h>
#include "SSD1306.h"
#include <temperature.h>
#include <relaischeck.h>
#include <ESP8266WiFiMulti.h>

SSD1306 display(0x3C, D2, D5); //Address set here 0x3C that I found in the scanner, and pins defined as D2 (SDA/Serial Data), and D5 (SCK/Serial Clock).

long OLEDinterval = 1000;
long OLEDpreviousMillis = 0;

String ip2Str(IPAddress ip);

void OLEDsetup()
{
    display.init();
    display.flipScreenVertically();
    display.drawString(0, 0, "Hello world");
    display.display();
}

void OLEDloop()
{
    unsigned long currentMillis = millis();
    if (currentMillis - OLEDpreviousMillis > OLEDinterval)
    {
        OLEDpreviousMillis = currentMillis;

        display.clear();
        display.drawString(0, 0, "IP: " + ip2Str(WiFi.localIP()) + "\nCurrent Temp:" + (String)TEMPgetTemperature() + "\nMin Temp: " + (String)RCgetMinTemperature() + "\nRelay state: " + (RCgetRelayState() ? "on" : "off"));
        display.display();
    }
}

String ip2Str(IPAddress ip)
{
    String s = "";
    for (int i = 0; i < 4; i++)
    {
        s += i ? "." + String(ip[i]) : String(ip[i]);
    }
    return s;
}