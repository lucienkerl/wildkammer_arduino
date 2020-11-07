
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_PIN D4
OneWire oneWire(ONE_WIRE_PIN);
DallasTemperature sensors(&oneWire);

float temperature = 0;

long TEMPinterval = 1000;
long TEMPpreviousMillis = 0;

void TEMPloop()
{
    unsigned long currentMillis = millis();
    if (currentMillis - TEMPpreviousMillis > TEMPinterval)
    {
        Serial.print("Getting temp: ");
        sensors.requestTemperatures();
        delay(100);
        temperature = sensors.getTempCByIndex(0);
        Serial.println(temperature);
    }
}

float TEMPgetTemperature()
{
    return temperature;
}