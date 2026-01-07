#define NUM_SLOTS 10
int slotPins[NUM_SLOTS] = {2,3,4,5,6,7,8,9,10,11};
int slotStatus[NUM_SLOTS] = {0}; // 0=free, 1=occupied
int prevSlotStatus[NUM_SLOTS] = {0};

void setup() {
  Serial.begin(9600);      // Debug to PC
  Serial1.begin(9600);     // Mega → ESP8266 (TX1=18, RX1=19)

  for (int i=0; i<NUM_SLOTS; i++) {
    pinMode(slotPins[i], INPUT);
  }

  Serial.println("System Ready: 10 IR slots monitoring...");
}

void loop() {
  for (int i=0; i<NUM_SLOTS; i++) {
    int status = digitalRead(slotPins[i]);
    if (status != prevSlotStatus[i]) {
      prevSlotStatus[i] = status;
      String msg = String(i+1) + "," + (status == HIGH ? "OCCUPIED" : "FREE");
      sendMessage(msg);
    }
  }
  delay(100); // Polling delay
}

void sendMessage(String msg) {
  Serial.println(msg);    // Debug
  Serial1.println(msg);   // To ESP8266
}
