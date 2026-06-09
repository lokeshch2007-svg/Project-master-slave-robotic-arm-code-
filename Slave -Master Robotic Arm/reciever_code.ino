#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <WiFi.h>
#include <esp_now.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN 150
#define SERVOMAX 540

typedef struct struct_message {
  int command;
  float angle1;  // Pot1 reversed (GPIO 32)
  float angle2;  // Pot2 (GPIO 33)
  float angle3;  // Pot3 (GPIO 34)
} struct_message;

struct_message receivedData;

int angleToPulse(float angle) {
  return map((int)angle, 0, 180, SERVOMIN, SERVOMAX);
}

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  memcpy(&receivedData, incomingData, sizeof(receivedData));
  Serial.println("Data received via ESP-NOW:");

  Serial.print("Command (button): "); Serial.println(receivedData.command);
  Serial.print("Angle1 (GPIO 32 reversed): "); Serial.println(receivedData.angle1);
  Serial.print("Angle2 (GPIO 33): "); Serial.println(receivedData.angle2);
  Serial.print("Angle3 (GPIO 34): "); Serial.println(receivedData.angle3);
  Serial.println("-----------------------------");

  // Apply values to correct channels:
  pwm.setPWM(0, 0, angleToPulse(receivedData.command));  // Button
  pwm.setPWM(1, 0, angleToPulse(receivedData.angle2));   // Pot2 → Channel 1
  pwm.setPWM(2, 0, angleToPulse(receivedData.angle1));   // Pot1 reversed → Channel 2
  pwm.setPWM(3, 0, angleToPulse(receivedData.angle3));   // Pot3 → Channel 3
}

void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setPWMFreq(50);
  delay(1000);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("Receiver ready");
}

void loop() {
  // No logic here; all handled in callback
}
