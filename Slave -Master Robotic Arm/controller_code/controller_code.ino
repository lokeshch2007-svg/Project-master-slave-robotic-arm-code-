const int buttonPin = 18;
const int potPin1 = 32;  // REVERSED
const int potPin2 = 33;
const int potPin3 = 34;

void setup() {
  Serial.begin(115200);

  pinMode(buttonPin, INPUT_PULLUP);  // Button pin setup
  pinMode(potPin1, INPUT);
  pinMode(potPin2, INPUT);
  pinMode(potPin3, INPUT);
}

void loop() {
  // --- Push Button ---
  int buttonState = digitalRead(buttonPin);
  Serial.println(buttonState == LOW ? "Button: PRESSED" : "Button: NOT PRESSED");

  // --- Read Raw Potentiometer Values ---
  int raw1 = analogRead(potPin1);  // GPIO 32 (reversed)
  int raw2 = analogRead(potPin2);  // GPIO 33
  int raw3 = analogRead(potPin3);  // GPIO 34

  // --- Reverse GPIO 32 reading ---
  int reversed1 = 4095 - raw1;

  // --- Calculate Angles ---
  float angle1 = min(reversed1 * 0.072, 330.0);  // GPIO 32 reversed
  float angle2 = min(raw2 * 0.072, 330.0);       // GPIO 33 normal
  float angle3 = min(raw3 * 0.072, 330.0);       // GPIO 34 normal

  // --- Print Results ---
  Serial.print("Pot1 (GPIO 32 - REVERSED): ");
  Serial.print(raw1);
  Serial.print(" -> Angle: ");
  Serial.print(angle1, 1);
  Serial.println("°");

  Serial.print("Pot2 (GPIO 33): ");
  Serial.print(raw2);
  Serial.print(" -> Angle: ");
  Serial.print(angle2, 1);
  Serial.println("°");

  Serial.print("Pot3 (GPIO 34): ");
  Serial.print(raw3);
  Serial.print(" -> Angle: ");
  Serial.print(angle3, 1);
  Serial.println("°");

  Serial.println("-----------------------------");
  delay(500);
}
