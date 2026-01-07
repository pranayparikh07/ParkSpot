#define NUM_SLOTS 8

// Arduino Mega analog pins A0-A7 = pins 54-61
int slotPins[NUM_SLOTS] = {54, 55, 56, 57, 58, 59, 60, 61};
int prevStatus[NUM_SLOTS] = {-1, -1, -1, -1, -1, -1, -1, -1};

const int SENSOR_THRESHOLD = 500; // adjust for your IR sensors

void setup() {
  Serial.begin(9600);      // Debug to PC
  Serial1.begin(9600);     // Mega TX1(pin 18) → ESP8266 RX
  Serial.println("Parking System Ready: Monitoring A0-A7");
}

void loop() {
  for (int i = 0; i < NUM_SLOTS; i++) {
    int value = analogRead(slotPins[i]);
    int status = (value > SENSOR_THRESHOLD) ? 1 : 0; // 1=occupied, 0=free

    if (status != prevStatus[i]) {
      prevStatus[i] = status;
      String msg = String(i + 1) + "," + (status == 1 ? "OCCUPIED" : "FREE");
      
      Serial.println(msg);   // Debug to PC
      Serial1.println(msg);  // To ESP8266
    }
  }
  delay(200);
}
