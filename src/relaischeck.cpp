
#include <Arduino.h>
#include <temperature.h>

const int RELAY_PIN = D6;

long RCinterval = 60000;
long RCpreviousMillis = 0;

float RCminTemperature = 4;

bool relayState = false;

void RELAYrunCheck();

void RELAYsetup()
{
    pinMode(RELAY_PIN, OUTPUT);
}

void RELAYloop()
{
    unsigned long currentMillis = millis();
    if (currentMillis - RCpreviousMillis > RCinterval)
    {
        RCpreviousMillis = currentMillis;
        RELAYrunCheck();
    }
}

void RELAYrunCheck()
{
    Serial.println((String) "Within relaischeck: " + TEMPgetTemperature());
    Serial.println((String) "RCminTemperature" + RCminTemperature);
    if (TEMPgetTemperature() < RCminTemperature)
    {
        // Shut off fridge
        digitalWrite(RELAY_PIN, HIGH);
        relayState = false;
    }
    else
    {
        // Turn on fridge
        digitalWrite(RELAY_PIN, LOW);
        relayState = true;
    }
}

void RCsetMinTemperature(float minTemperature)
{
    RCminTemperature = minTemperature;
    RELAYrunCheck();
}

float RCgetMinTemperature()
{
    return RCminTemperature;
}

bool RCgetRelayState()
{
    return relayState;
}