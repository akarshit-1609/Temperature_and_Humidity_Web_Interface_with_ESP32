#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DHTesp.h>
#include <LittleFS.h>


const char* ssid = "Your_ssid";
const char* password = "your_password";

int sensorPin = 21;

IPAddress ip(192, 168, 0, 1);
IPAddress mask(255, 255, 255, 0);

AsyncWebServer s(80);
DHTesp dht;

String data;
int temp, humidity;


void setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(ip, ip, mask);

  dht.setup(sensorPin, DHTesp::DHT11);

  if (!LittleFS.begin(true)) {
    return;
  }

  s.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", "text/html");
  });

  s.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/style.css", "text/css");
  });

  s.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/script.js", "application/javascript");
  });

  s.on("/thermometer.png", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/thermometer.png", "image/png");
  });

  s.on("/humidity.png", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/humidity.png", "image/png");
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