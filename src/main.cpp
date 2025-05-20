#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DHTesp.h>


const char* ssid = "Your_ssid";
const char* password = "your_password";

int sensorPin = 21;

IPAddress ip(192, 168, 0, 1);
IPAddress mask(255, 255, 255, 0);

AsyncWebServer s(80);
DHTesp dht;

const char* content = R"rawliteral(
<!DOCTYPE html>
<html lang="en" dir="ltr">

<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Temperature and Humidity</title>
  <style>
#heading {
  display: flex;
  margin: 50px auto;
  align-items: center;
  justify-content: center;
  color: green;
}

.container {
  display: flex;
  align-items: center;
  margin-top: 40px;
}
.container img {
  margin-left: 20px;
  height: 48px;
}
.container .name {
  margin-left: 10px;
  font-size: 32px;
  width: 200px;
}
.container .colon {
  margin-left: 4px;
  font-size: 32px;
}

.data {
  margin-left: 16px;
  font-size: 36px;
}

#temp {
  color: #ff5500;
}

#humd {
  color: #0099ff;
}
  </style>
</head>

<body>
  <h1 id="heading">Weather Report :-</h1>
  <div class="container">
    <img src="data:image/png;base64,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">
    <div class="name">Temperature</div>
    <div class="colon">:</div>
    <div id="temp" class="data">--</div>
  </div>
  <div class="container">
    <img src="data:image/png;base64,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">
    <div class="name">Humidity</div>
    <div class="colon">:</div>
    <div id="humd" class="data">--</div>
  </div>
  <script>
var temp = document.getElementById("temp");
var humidity = document.getElementById("humd");

function weather(){
  fetch("/request", {
      method: "POST",
      headers: {
          'Accept': 'application/json',
          'Content-Type': 'text/plain'
      },
      body: ""
  }).then(res => {
      return res.json();
  }).then(weather_data => {
      temp.innerHTML = String(parseInt(weather_data.temp)) + "&deg;C";
      humidity.innerHTML = String(parseInt(weather_data.humidity)) + "&percnt;";
  }).catch(e => {
      temp.innerHTML = "Err";
      humidity.innerHTML = "Err";
  });
}
weather();

setInterval(() => {
  weather();
}, 5000);
  </script>
</body>

</html>
)rawliteral";

String data;
int temp, humidity;


void setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(ip, ip, mask);

  dht.setup(sensorPin, DHTesp::DHT11);

  s.on("/", HTTP_GET, [](AsyncWebServerRequest *req){
    req->send_P(200, "text/html", content);
  });

  s.on("/request", HTTP_POST, [](AsyncWebServerRequest *req){
    temp = dht.getTemperature();
    humidity = dht.getHumidity();
    data = "{\"temp\": \"" + String(temp) + "\", \"humidity\": \"" + String(humidity) + "\"}";
    if (humidity <= 100 && humidity >= 0){
      req->send_P(200, "text/plain", data.c_str());
    }
  });

  s.begin();
  
  
}

void loop() {
  
}