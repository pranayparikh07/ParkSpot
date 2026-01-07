#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "Redmi Note 12 5G";
const char* password = "23232323";
const char* server = "http://10.111.123.192:5000/update";  // Flask endpoint

WiFiClient client;

void setup() {
  Serial.begin(9600);   // From Mega
  WiFi.begin(ssid, password);

  Serial.println("Connecting WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
}

void loop() {
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    line.trim();
    if (line.length() > 0) {
      Serial.println("Forwarding: " + line);

      // Send to Flask server
      if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(client, server);  // Use WiFiClient
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        // Send as 'event' parameter
        String postData = "event=" + line;
        int httpResponse = http.POST(postData);
        Serial.println("HTTP Response: " + String(httpResponse));
        http.end();
      }
    }
  }
}
