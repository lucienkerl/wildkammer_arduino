
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <temperature.h>
#include <relaischeck.h>

ESP8266WebServer server(80);

void handle_OnConnect();
void handle_OnGetTemperature();
void handle_NotFound();
void handleSpecificArg();
String SendHTML(float temperature, float humidity, float pressure, float altitude);

void WEBsetup()
{
    server.on("/", handle_OnConnect);
    server.on("/getTemperature", handle_OnGetTemperature);
    server.on("/setTemp", handleSpecificArg); //Associate the handler function to the path
    server.onNotFound(handle_NotFound);

    server.begin();
    Serial.println("HTTP server started");
}

void WEBloop()
{
    server.handleClient();
}

void handle_OnGetTemperature()
{
    const size_t capacity = JSON_OBJECT_SIZE(3);
    DynamicJsonDocument doc(capacity);

    doc["temperature"] = TEMPgetTemperature();
    doc["minTemperature"] = RCgetMinTemperature();
    doc["relayState"] = RCgetRelayState();

    String output;
    serializeJson(doc, output);

    server.send(200, "application/json", output);
}

void handle_OnConnect()
{
    server.send(200, "text/html", SendHTML(TEMPgetTemperature(), 0, 0, 0));
}

void handle_NotFound()
{
    server.send(404, "text/plain", "Not found");
}

void handleSpecificArg()
{
    String message = "";

    if (server.arg("Temperature") == "")
    { //Parameter not found

        message = "Temperature Argument not found";
    }
    else
    { //Parameter found

        message = "Temperature Argument = ";
        message += server.arg("Temperature"); //Gets the value of the query parameter
        RCsetMinTemperature(server.arg("Temperature").toFloat());
    }

    server.send(200, "text / plain", message); //Returns the HTTP response
}

String SendHTML(float temperature, float humidity, float pressure, float altitude)
{
    String ptr = "<!DOCTYPE html>";
    ptr += "<html>";
    ptr += "<head>";
    ptr += "<title>ESP8266 Thermometer mit influxDB</title>";
    ptr += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    ptr += "<link href='https://fonts.googleapis.com/css?family=Open+Sans:300,400,600' rel='stylesheet'>";
    ptr += "<style>";
    ptr += "html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #444444;}";
    ptr += "body{margin: 0px;} ";
    ptr += "h1 {margin: 50px auto 30px;} ";
    ptr += ".side-by-side{display: table-cell;vertical-align: middle;position: relative;}";
    ptr += ".text{font-weight: 600;font-size: 19px;width: 200px;}";
    ptr += ".reading{font-weight: 300;font-size: 50px;padding-right: 25px;}";
    ptr += ".temperature .reading{color: #F29C1F;}";
    ptr += ".humidity .reading{color: #3B97D3;}";
    ptr += ".pressure .reading{color: #26B99A;}";
    ptr += ".altitude .reading{color: #955BA5;}";
    ptr += ".superscript{font-size: 17px;font-weight: 600;position: absolute;top: 10px;}";
    ptr += ".data{padding: 10px;}";
    ptr += ".container{display: table;margin: 0 auto;}";
    ptr += ".icon{width:65px}";
    ptr += "</style>";
    ptr += "<script>\n";
    ptr += "setInterval(loadDoc,1000);\n";
    ptr += "function loadDoc() {\n";
    ptr += "var xhttp = new XMLHttpRequest();\n";
    ptr += "xhttp.onreadystatechange = function() {\n";
    ptr += "if (this.readyState == 4 && this.status == 200) {\n";
    ptr += "//document.getElementById(\"replaceDiv\").innerHTML =this.responseText}\n";
    ptr += "};\n";
    ptr += "xhttp.open(\"GET\", \"/\", true);\n";
    ptr += "xhttp.send();\n";
    ptr += "}\n";
    ptr += "}\n";
    ptr += "function submitClicked() {\n";
    ptr += "var url = \"/setTemp?Temperature=\"+ document.getElementById(\"temperatureInput\").value;\n";
    ptr += "window.open(url);\n";
    ptr += "}\n";
    ptr += "</script>\n";
    ptr += "</head>";
    ptr += "<body>";
    ptr += "<div id=\"replaceDiv\">";
    ptr += "<h1>ESP8266 Weather Station</h1>";
    ptr += "<div class='container'>";
    ptr += "<div class='data temperature'>";
    ptr += "<div class='side-by-side icon'>";
    ptr += "<svg enable-background='new 0 0 19.438 54.003'height=54.003px id=Layer_1 version=1.1 viewBox='0 0 19.438 54.003'width=19.438px x=0px xml:space=preserve xmlns=http://www.w3.org/2000/svg xmlns:xlink=http://www.w3.org/1999/xlink y=0px><g><path d='M11.976,8.82v-2h4.084V6.063C16.06,2.715,13.345,0,9.996,0H9.313C5.965,0,3.252,2.715,3.252,6.063v30.982";
    ptr += "C1.261,38.825,0,41.403,0,44.286c0,5.367,4.351,9.718,9.719,9.718c5.368,0,9.719-4.351,9.719-9.718";
    ptr += "c0-2.943-1.312-5.574-3.378-7.355V18.436h-3.914v-2h3.914v-2.808h-4.084v-2h4.084V8.82H11.976z M15.302,44.833";
    ptr += "c0,3.083-2.5,5.583-5.583,5.583s-5.583-2.5-5.583-5.583c0-2.279,1.368-4.236,3.326-5.104V24.257C7.462,23.01,8.472,22,9.719,22";
    ptr += "s2.257,1.01,2.257,2.257V39.73C13.934,40.597,15.302,42.554,15.302,44.833z'fill=#F29C21 /></g></svg>";
    ptr += "</div>";
    ptr += "<div class='side-by-side text'>Temperature</div>";
    ptr += "<div class='side-by-side reading'>";
    //ptr +=(float)temperature;
    ptr += (float)temperature;
    ptr += "<span class='superscript'>°C</span></div>";
    ptr += "</div>";
    ptr += "</div>";
    ptr += "<div>";
    ptr += "<input type=\"number\" id=\"temperatureInput\">";
    ptr += "<button onclick=\"submitClicked()\">Submit</button>";
    ptr += "</div>";
    ptr += "</body>";
    ptr += "</html>";
    return ptr;
}
