#include <WiFi.h>
#include <esp_now.h>

typedef struct struct_message {
  int command;
  float angle1;
  float angle2;
  float angle3;
} struct_message;

struct_message sendData;

// Receiver MAC Address
uint8_t receiverAddress[] = {0x24, 0x6F, 0x28, 0xAA, 0xBB, 0xCC}; // CHANGE THIS

esp_now_peer_info_t peerInfo;

// Pins
const int buttonPin = 25;
const int pot1Pin = 32;
const int pot2Pin = 33;
const int pot3Pin = 34;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);

  pinMode(buttonPin, INPUT_PULLUP);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to Add Peer");
    return;
  }

  Serial.println("Sender Ready");
}

void loop() {

  // Button: pressed = 180, released = 0
  sendData.command = digitalRead(buttonPin) == LOW ? 180 : 0;

  // Read potentiometers
  int pot1 = analogRead(pot1Pin);
  int pot2 = analogRead(pot2Pin);
  int pot3 = analogRead(pot3Pin);

  // Convert to angles
  sendData.angle1 = map(pot1, 0, 4095, 180, 0);   // Reversed
  sendData.angle2 = map(pot2, 0, 4095, 0, 180);
  sendData.angle3 = map(pot3, 0, 4095, 0, 180);

  esp_err_t result = esp_now_send(
    receiverAddress,
    (uint8_t *)&sendData,
    sizeof(sendData)
  );

  if (result == ESP_OK) {
    Serial.println("Sent");
  } else {
    Serial.println("Error Sending");
  }

  Serial.print("Button: ");
  Serial.print(sendData.command);

  Serial.print("  A1: ");
  Serial.print(sendData.angle1);

  Serial.print("  A2: ");
  Serial.print(sendData.angle2);

  Serial.print("  A3: ");
  Serial.println(sendData.angle3);

  delay(50);
}