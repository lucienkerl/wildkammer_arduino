
#include <InfluxDbClient.h>
#include <ESP8266WiFiMulti.h>
#include <temperature.h>
#include <relaischeck.h>
#include <config.h>

InfluxDBClient client(INFLUXDB_HOST, INFLUXDB_DB);

long previousMillis = 0;

long interval = 10000;

void INFLUXloop()
{
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > interval)
    {
        previousMillis = currentMillis;
        Point pointDevice(INFLUXDB_MEASUREMENT);
        // Set tags
        pointDevice.addTag("device", INFLUXDB_TAG_DEVICE);
        pointDevice.addTag("SSID", WiFi.SSID());
        // Add data
        pointDevice.addField("temperature", TEMPgetTemperature());
        pointDevice.addField("minTemperature", RCgetMinTemperature());
        pointDevice.addField("relayState", RCgetRelayState());

        // Write data
        client.writePoint(pointDevice);
    }
}