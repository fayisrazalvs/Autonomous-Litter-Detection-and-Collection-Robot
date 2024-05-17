//Project: Autonomous Litter Detection and Collection Robot
//Author: Razal Fayis V. S.
//Date: 11 April 2024
// Note: Update and Changes to the libraries may be required in the future to properly run the program

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define TRIGGER_PIN D8
#define ECHO_PIN D2

// Define pin numbers for L298N motor driver
const int ENA = D0;  // Pin connected to ENA of L298N
const int IN_1 = D3; // Pin connected to IN1 of L298N
const int IN_2 = D4; // Pin connected to IN2 of L298N
const int IN_3 = D5; // Pin connected to IN3 of L298N
const int IN_4 = D6; // Pin connected to IN4 of L298N
const int ENB = D7;  // Pin connected to ENB of L298N

// Define variables
const int obstacleDistance = 15; // Distance to detect an obstacle in cm

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define MIN_PULSE_WIDTH 600
#define MAX_PULSE_WIDTH 2600
#define FREQUENCY 50

// PCA9685 pins
const int SERVO_PIN_1 = 0;
const int SERVO_PIN_2 = 3;
const int VACUUM_PIN = 5;

void setup() {
  // Initialize pins
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Initialize Serial communication
  Serial.begin(115200);  // Match this baud rate with the Python code

  // Initialize PCA9685
  pwm.begin();
  pwm.setPWMFreq(FREQUENCY);
}

void loop() {
  // Check for incoming commands from Raspberry Pi
  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == 'D') {
      float distance = measureDistance();
      Serial.println(distance);
    } else if (command == 'S') {
      stopMotors();
    } else if (command == 'P') {
      if (measureDistance() <= 15) {
        stopMotors();
        pickLitter();
      }
    } else if (command == 'R') {
      stopMotors();
      releaseLitter();
    }
  }

  // Check for obstacle
  if (checkForObstacle()) {
    // Obstacle detected, stop and avoid obstacle
    avoidObstacle();
  } else {
    // No obstacle detected, move forward
    goForward();
  }
}

bool checkForObstacle() {
  // Trigger ultrasonic sensor
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  // Measure echo pulse width
  unsigned long duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate distance based on pulse width
  int distance = duration * 0.034 / 2;

  // Print distance to Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Check if obstacle is within distance threshold
  return distance <= obstacleDistance;
}

void goForward() {
  // Motor control to move forward
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, 255);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, 255);
}

void stopMotors() {
  // Stop the motors
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, 0);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, 0);
}

void avoidObstacle() {
  // Reverse a bit
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, 200);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, 200);

  delay(1000); // Hold for 1 second

  // Turn right
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, 255);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, 0);
}

float measureDistance() {
  // Trigger ultrasonic sensor
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  // Measure echo pulse width
  unsigned long duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate distance based on pulse width
  float distance = duration * 0.034 / 2;
  return distance;
}

void pickLitter() {
  setServoAngle(SERVO_PIN_1, 130);  // Adjust angle for servo motor 1
  setServoAngle(SERVO_PIN_2, 0);    // Adjust angle for servo motor 2
  pwm.setPWM(VACUUM_PIN, 0, 4096);  // Turn on vacuum pump
  delay(3000);                      // Hold for 3 seconds
}

void releaseLitter() {
  setServoAngle(SERVO_PIN_1, 0);    // Adjust angle for servo motor 1
  setServoAngle(SERVO_PIN_2, 45);   // Adjust angle for servo motor 2
  delay(3000);                      // Hold for 3 seconds
  pwm.setPWM(VACUUM_PIN, 0, 0);     // Turn off vacuum pump
}

void setServoAngle(int servoNum, int angle) {
  int pulse_wide = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  int analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  pwm.setPWM(servoNum, 0, analog_value);
}
