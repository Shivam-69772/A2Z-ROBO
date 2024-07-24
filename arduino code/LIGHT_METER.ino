#include <ESP8266WiFi.h>
#include <ESPAsyncWebSrv.h>
#include <WebSocketsServer.h>
#include <Ticker.h>

int analogPin = A0;

void send_sensor();
Ticker timer;

char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
    <head>
        <title>Light Monitor</title>
    </head>
    <style>
        body {
            text-align: center;
            font-size: 90px;
        }
        lable, #meter, h {
            font-size: 80px;
        }
        #meter {
            rotate: 270deg;
            margin-top: 200px;
        }
        #percentage {
            margin-top: 0px;
        }
        meter::-webkit-meter-bar{
            
        }
        meter::-webkit-meter-optimum-value{
            background: linear-gradient(0.25turn, red ,yellow);
        }
    </style>
    <body>
        <lable>LIGHT</lable>
        <br/>
        <h id="percentage">%</h>
        <br/>
        <meter id="meter" min="0" max="1024" value="0"> </meter>
    </body>
    <script>
        var connection = new WebSocket('ws://'+location.hostname+':81/');
        connection.onmessage = function(event){
           var FullDATA = event.data;
           console.log(FullDATA);
           var data = JSON.parse(FullDATA);
           light = data.light;
           document.getElementById("meter").value = light;
           document.getElementById("percentage").innerText = light;
            console.log(light);
        }
    </script>
</html>
)=====";

AsyncWebServer server(80);
WebSocketsServer webSocket(81);
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
                Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        
        // send message to client
        webSocket.sendTXT(num, "Connected");
            }
            break;
        case WStype_TEXT:
           Serial.printf("[%u] get Text: %s\n", num, payload);
           String message = String((char*)(payload));
           Serial.println(message); 
           }
}

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}
void setup() {
  Serial.begin(115200);
 WiFi.softAP("LIGHT SENSOR","");
  Serial.println("softap");
  Serial.println("...");
  Serial.println(WiFi.softAPIP());
   server.on("/", [](AsyncWebServerRequest *request)
   {
    
        request->send_P(200, "text/html", webpage );
    });
     server.onNotFound(notFound);
      server.begin();
      webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  timer.attach(1,send_sensor);
}

void loop() {
 webSocket.loop();
}
void send_sensor(){
  int light = analogRead(analogPin);
  String JSON_Data = "{\"light\":";
         JSON_Data += light;
         JSON_Data +="}";
         Serial.println(JSON_Data);
         webSocket.broadcastTXT(JSON_Data);
}
