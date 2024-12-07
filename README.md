# Automated Light Control and Bill Prediction

## Overview:
This project automates the control of an LED light based on human presence detected by a PIR motion sensor and an ultrasonic sensor. It also predicts the bill for electricity usage based on the time the light is on.

## Components Needed:
- Arduino Uno (or any compatible Arduino board)
- HC-SR04 Ultrasonic Sensor (for distance measurement)
- PIR Motion Sensor (for detecting motion)
- LED (to simulate a light bulb)
- 220Ω Resistor (for LED current limiting)
- Jumper wires
- Breadboard

## Working:
- The light (LED) turns on when motion is detected by the PIR sensor or if someone is detected by the ultrasonic sensor (distance below a certain threshold).
- If no movement is detected for a specified timeout (2 seconds), the light turns off automatically.
- The system keeps track of how long the light has been on and calculates the electricity bill based on the time the light has been on, assuming a cost per second.

## Pin Connections:
- **Ultrasonic Sensor (HC-SR04):**
  - Trigger Pin -> Arduino Pin 9
  - Echo Pin -> Arduino Pin 10

- **PIR Sensor:**
  - Output Pin -> Arduino Pin 8

- **LED (Light):**
  - LED Pin -> Arduino Pin 13

## Code Explanation:

1. **Setup:**
   - Set pin modes for the ultrasonic sensor, PIR sensor, and LED.
   - Start serial communication for debugging.

2. **Main Loop:**
   - The PIR sensor checks for movement and turns the light on when motion is detected.
   - The ultrasonic sensor measures distance and turns the light on if the distance is below a threshold.
   - If no movement is detected for 2 seconds, the light is turned off.
   - The code also calculates the total time the light has been on and estimates the electricity cost based on that.

3. **Bill Prediction:**
   - The bill is calculated by multiplying the total light-on time by a predefined cost per second.
   - Every 2 seconds, the predicted bill is printed on the Serial Monitor.

## Example Arduino Code:

```cpp
const int trigPin = 9;    // Trig pin of HC-SR04
const int echoPin = 10;   // Echo pin of HC-SR04

const int pirPin = 8;     // Output pin of PIR sensor

const int ledPin = 13;    // Pin connected to LED

bool lightOn = false;
unsigned long lastMovementTime = 0; 
const unsigned long timeoutDuration = 2000; // 2 seconds

const float unitCost = 0.12; // Cost per second of light on (in dollars)
unsigned long totalLightOnTime = 0; 
unsigned long lastUpdateTime = 0;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int pirState = digitalRead(pirPin);
  if (pirState == HIGH) {
    lightOn = true;
    lastMovementTime = millis();
    digitalWrite(ledPin, HIGH);
  }

  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration * 0.0343) / 2;

  if (distance < 150) {
    lastMovementTime = millis();
    lightOn = true;
    digitalWrite(ledPin, HIGH);
  }

  if (millis() - lastMovementTime > timeoutDuration && lightOn) {
    lightOn = false;
    totalLightOnTime += millis() - lastUpdateTime;
    digitalWrite(ledPin, LOW);
  }

  if (lightOn) {
    lastUpdateTime = millis();
  }

  float predictedBill = (totalLightOnTime / 1000.0) * unitCost;

  if (millis() % 2000 < 100) {
    Serial.print("Predicted bill for today (based on current usage): $");
    Serial.println(predictedBill, 4);
  }

  delay(100);
}
