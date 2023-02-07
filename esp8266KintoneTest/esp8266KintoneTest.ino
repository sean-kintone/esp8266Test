#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid = "LUSE QUEST 2.4";
const char* password = "seanandshizuka";
const char* apiToken = "N8HC1NLI7DyjcbKdE3c37PzGiCjoHKQwolWLGN9B";
//Your Domain name with URL path or IP address with path
const char* serverName = "https://sean.kintone.com/k/v1/record.json?app=49";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
  // Send an HTTP POST request depending on timerDelay
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;

      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);

      const char* json = "{\"app\":\"49\", \"record\":{\"text\":{\"value\":\"test\"}}}";
      int responseCode = 0;
      http.addHeader("X-Cybozu-API-Token", apiToken);
      http.addHeader("Content-type", "application/json");
      responseCode = http.POST(json);
      Serial.printf("http Response Code = %d \n", responseCode);
      String payload = http.getString();
      Serial.println(payload);

      if (responseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(responseCode);
        String payload = http.getString();
        Serial.println(payload);
      } else {
        Serial.print("Error code: ");
        Serial.println(responseCode);
      }
      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}