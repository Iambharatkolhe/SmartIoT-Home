// Define pins for ultrasonic sensor
const int trigPin = 9;    // Trig pin of HC-SR04
const int echoPin = 10;   // Echo pin of HC-SR04

// Define pin for PIR sensor
const int pirPin = 8;     // Output pin of PIR sensor

// Define pin for LED
const int ledPin = 13;    // Pin connected to LED

// Variables for state tracking
bool lightOn = false;
unsigned long lastMovementTime = 0; // Timestamp for the last movement detected
const unsigned long timeoutDuration = 2000; // Time in milliseconds (2 seconds)

// Variables for bill prediction
const float unitCost = 0.12; // Cost per second of light on (in dollars)
unsigned long totalLightOnTime = 0;  // Accumulated time light has been on
unsigned long lastUpdateTime = 0;    // Tracks the last time light was on

void setup() {
  // Set pin modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // Start Serial Monitor for debugging
  Serial.begin(9600);
}

void loop() {
  // Read PIR sensor state
  int pirState = digitalRead(pirPin);

  // Check if motion is detected by the PIR sensor
  if (pirState == HIGH) {
    lightOn = true;
    lastMovementTime = millis();  // Update the last movement time
    digitalWrite(ledPin, HIGH);   // Turn LED on
  }

  // Ultrasonic Sensor Distance Measurement
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration * 0.0343) / 2;

  // If the distance is below a threshold, assume someone is present
  if (distance < 150) {  // Adjust threshold based on the room size
    lastMovementTime = millis();
    lightOn = true;
    digitalWrite(ledPin, HIGH);   // Ensure LED is on
  }

  // Turn off the light if no movement is detected for a timeout duration
  if (millis() - lastMovementTime > timeoutDuration && lightOn) {
    lightOn = false;
    totalLightOnTime += millis() - lastUpdateTime; // Accumulate light on time
    digitalWrite(ledPin, LOW);    // Turn LED off
  }

  // Update the last active time for light
  if (lightOn) {
    lastUpdateTime = millis();
  }

  // Calculate the predicted bill based on total light-on time
  float predictedBill = (totalLightOnTime / 1000.0) * unitCost;

  // Display the predicted bill periodically (every 2 seconds)
  if (millis() % 2000 < 100) { // Every 2 seconds
    Serial.print("Predicted bill for today (based on current usage): $");
    Serial.println(predictedBill, 4);
  }

  delay(100);  // Short delay for stability
}