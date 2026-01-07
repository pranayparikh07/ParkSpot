#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "Redmi Note 12 5G";
const char* password = "23232323";
const char* server = "http://10.111.123.192:5000/update";  // Flask endpoint

WiFiClient client;

void setup() {
  Serial.begin(9600);   // From Mega TX1 (pin 18)
  delay(1000);
  
  Serial.println("\n\n=== ESP8266 Starting ===");
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(1000);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected!");
    Serial.print("ESP IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("Server: ");
    Serial.println(server);
  } else {
    Serial.println("\nWiFi FAILED! Check SSID/password");
  }
  
  Serial.println("Waiting for data from Mega on Serial...");
}

void loop() {
  // Show WiFi status periodically
  static unsigned long lastCheck = 0;
  if (millis() - lastCheck > 10000) {
    lastCheck = millis();
    Serial.print("[WiFi Status: ");
    Serial.print(WiFi.status() == WL_CONNECTED ? "Connected" : "Disconnected");
    Serial.println("]");
  }
  
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    line.trim();
    if (line.length() > 0) {
      Serial.println(">>> Received from Mega: " + line);

      // Send to Flask server
      if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(client, server);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        http.setTimeout(5000);

        String postData = "event=" + line;
        Serial.println(">>> Sending: " + postData);
        
        int httpResponse = http.POST(postData);
        
        if (httpResponse > 0) {
          Serial.println(">>> HTTP Response: " + String(httpResponse));
        } else {
          Serial.println(">>> HTTP Error: " + http.errorToString(httpResponse));
        }
        http.end();
      } else {
        Serial.println(">>> WiFi not connected, cannot send!");
      }
    }
  }
}
