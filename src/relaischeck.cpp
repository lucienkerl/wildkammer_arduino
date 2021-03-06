
#include <Arduino.h>
#include <temperature.h>

const int RELAY_PIN = D6;

long RCinterval = 10000;
long RCpreviousMillis = 0;

float RCminTemperature = 4;
float RCmaxTemperature = 7;

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
    Serial.println((String) "RCmaxTemperature" + RCmaxTemperature);
    if (TEMPgetTemperature() < RCminTemperature)
    {
        // Shut off fridge
        digitalWrite(RELAY_PIN, HIGH);
        relayState = false;
    }
    else if (!relayState && TEMPgetTemperature() <= RCmaxTemperature)
    { // when relay is off and temperature is not yet 1C warmer than minTemp = keep off
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

void RCsetMaxTemperature(float maxTemperature)
{
    RCmaxTemperature = maxTemperature;
    RELAYrunCheck();
}

float RCgetMinTemperature()
{
    return RCminTemperature;
}

float RCgetMaxTemperature()
{
    return RCmaxTemperature;
}

bool RCgetRelayState()
{
    return relayState;
}