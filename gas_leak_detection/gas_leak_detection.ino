#include <Servo.h>

const int gasSensorPin = A5;  // Analog pin connected to the MQ2 gas sensor
const int buzzerPin = 5;      // Digital pin connected to the buzzer
const int servoPin1 = 9;       // Digital pin connected to the for rakulater servo1
const int gasThreshold = 450; // Threshold value for gas detection (adjust based on your testing)

Servo myServo;  // Create servo object to control a servo
bool servoMoved = false; // Flag to check if the servo has already moved

void setup() {
  pinMode(gasSensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin,HIGH);
  myServo.attach(servoPin1);
  myServo.write(0); // Initial position of the servo1 // Initial position of the servo2
  Serial.begin(9600); // Initialize serial communication for debugging
}

void loop() {
  int gasLevel = analogRead(gasSensorPin); // Read the gas sensor value
  Serial.println(gasLevel); // Print the gas sensor value to the serial monitor for debugging

  if (gasLevel > gasThreshold && !servoMoved) {
    digitalWrite(buzzerPin,LOW); // Turn on the buzzer
    Serial.println("relay on");
   // delay(2000); // Beep for 2 seconds
   // digitalWrite(buzzerPin, LOW); // Turn off the buzzer

    myServo.write(90); // Rotate the servo to 90 degrees
    servoMoved = true; // Set the flag to indicate the servo has moved
  }

  delay(1000); // Wait for 1 second before checking the gas sensor value again
}
 
