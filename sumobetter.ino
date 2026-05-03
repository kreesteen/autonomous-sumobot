#include <stdlib.h>

// =======================================================
// CONFIGURATION & TUNING
// =======================================================

// --- Speeds (0 = off, 255 = full power) ---
const int ATTACK_SPEED = 255;
const int SEARCH_SPEED = 150;
const int REVERSE_SPEED = 200;

// --- Durations (in milliseconds) ---
// *** TUNE THIS VALUE! ***
// This is how long the robot will turn to evade the border. Adjust it until it makes a clean 90-degree turn.
const int PIVOT_DURATION_MS = 350;

// --- Sensor Thresholds ---
const int OPPONENT_DISTANCE = 55; // in cm

// --- IR Sensor Cooldowns ---
unsigned long lastEvasionTime = 0; // Stores when the last evasion happened
const unsigned long EVASION_COOLDOWN_MS = 1500; // Cooldown period of 500ms

// --- Motor A (Left) ---
const int LEFT_SPEED_PIN = 10;
const int LEFT_F_PIN = 12;
const int LEFT_R_PIN = 13;

// --- Motor B (Right) ---
const int RIGHT_SPEED_PIN = 9;
const int RIGHT_F_PIN = 7;
const int RIGHT_R_PIN = 8;

// --- Sensors ---
const int IR_BORDER_PIN = 5;
const int US_TRIG_PIN = 3;
const int US_ECHO_PIN = 4;


void setup() {
  Serial.begin(9600);
  pinMode(RIGHT_SPEED_PIN, OUTPUT);
  pinMode(RIGHT_F_PIN, OUTPUT);
  pinMode(RIGHT_R_PIN, OUTPUT);
  pinMode(LEFT_SPEED_PIN, OUTPUT);
  pinMode(LEFT_F_PIN, OUTPUT);
  pinMode(LEFT_R_PIN, OUTPUT);
  pinMode(US_TRIG_PIN, OUTPUT);
  pinMode(US_ECHO_PIN, INPUT);
  pinMode(IR_BORDER_PIN, INPUT);
  delay(5000);
}

void loop() {
  // 1. SENSE
  int borderDetected = digitalRead(IR_BORDER_PIN);
  int distanceToOpponent = getDistance();

  // 2. DECIDE & ACT

  int border_detected = 0;


  // if (borderDetected == HIGH) {
  //   delay(5);
  //   if (borderDetected == HIGH) {
  //     delay(5);

  //   } if (borderDetected == HIGH) {
  //     delay(5);
  //   }
  //   border_detected = 1;
  // }
  // PRIORITY 1: BORDER DETECTED
  // Check for the border AND check if the cooldown has passed
  // if (borderDetected == HIGH && (millis() - lastEvasionTime > EVASION_COOLDOWN_MS)) { // White
  //   Serial.println("Border! Executing safe evasion.");
    
  //   // --- The evasion maneuver is the same ---
  //   // driveBackwards(REVERSE_SPEED, REVERSE_SPEED);
  //   delay(50);
  //   pivotLeft(SEARCH_SPEED, SEARCH_SPEED);
  //   delay(PIVOT_DURATION_MS);
  //   driveForward(ATTACK_SPEED, ATTACK_SPEED);
  //   delay(400);

  //   // *** IMPORTANT: Record the time of this evasion to start the cooldown ***
  //   lastEvasionTime = millis(); 
  // }
  // PRIORITY 2: OPPONENT IN RANGE
  if (distanceToOpponent > 0 && distanceToOpponent < OPPONENT_DISTANCE) {
    Serial.print("Opponent detected. Attacking!");
    driveForward(ATTACK_SPEED, ATTACK_SPEED);

    // if (borderDetected == HIGH && (millis() - lastEvasionTime > EVASION_COOLDOWN_MS)) { // White
    //   Serial.println("Border! Executing safe evasion.");
      
    //   // --- The evasion maneuver is the same ---
    //   driveBackwards(REVERSE_SPEED, REVERSE_SPEED);
    //   delay(1000);
    //   stopMotors();
    //   delay(1000);
    //   pivotLeft(SEARCH_SPEED, SEARCH_SPEED);
    //   delay(PIVOT_DURATION_MS);
    //   driveForward(ATTACK_SPEED, ATTACK_SPEED);
    //   delay(400);

    //   // *** IMPORTANT: Record the time of this evasion to start the cooldown ***
    //   lastEvasionTime = millis(); 
    // }
  }
  // PRIORITY 3: SEARCH (DEFAULT STATE)
  else {
    Serial.println("Searching...");
    spin(SEARCH_SPEED, SEARCH_SPEED);

    if (borderDetected == HIGH && (millis() - lastEvasionTime > EVASION_COOLDOWN_MS)) { // White
      Serial.println("Border! Executing safe evasion.");
      
      // --- The evasion maneuver is the same ---
      driveBackwards(REVERSE_SPEED, REVERSE_SPEED);
      delay(1000);
      stopMotors();
      delay(1000);
      pivotLeft(SEARCH_SPEED, SEARCH_SPEED);
      delay(PIVOT_DURATION_MS);
      driveForward(ATTACK_SPEED, ATTACK_SPEED);
      delay(400);

      // *** IMPORTANT: Record the time of this evasion to start the cooldown ***
      lastEvasionTime = millis(); 
    }
  }
}

// =======================================================
// HELPER FUNCTIONS
// =======================================================

int getDistance() {
  digitalWrite(US_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(US_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(US_TRIG_PIN, LOW);
  long duration = pulseIn(US_ECHO_PIN, HIGH);
  return duration * 0.034 / 2;
}

void stopMotors() {
  analogWrite(RIGHT_SPEED_PIN, 0);
  analogWrite(LEFT_SPEED_PIN, 0);
}

void driveForward(int leftSpeed, int rightSpeed) {
  digitalWrite(RIGHT_F_PIN, HIGH);
  digitalWrite(RIGHT_R_PIN, LOW);
  digitalWrite(LEFT_F_PIN, HIGH);
  digitalWrite(LEFT_R_PIN, LOW);
  analogWrite(RIGHT_SPEED_PIN, rightSpeed);
  analogWrite(LEFT_SPEED_PIN, leftSpeed);
}

void driveBackwards(int leftSpeed, int rightSpeed) {
  digitalWrite(RIGHT_F_PIN, LOW);
  digitalWrite(RIGHT_R_PIN, HIGH);
  digitalWrite(LEFT_F_PIN, LOW);
  digitalWrite(LEFT_R_PIN, HIGH);
  analogWrite(RIGHT_SPEED_PIN, rightSpeed);
  analogWrite(LEFT_SPEED_PIN, leftSpeed);
}

void spin(int leftSpeed, int rightSpeed) {
  digitalWrite(RIGHT_F_PIN, HIGH);
  digitalWrite(RIGHT_R_PIN, LOW);
  digitalWrite(LEFT_F_PIN, LOW);
  digitalWrite(LEFT_R_PIN, HIGH);
  analogWrite(RIGHT_SPEED_PIN, rightSpeed);
  analogWrite(LEFT_SPEED_PIN, leftSpeed);
}

// --- NEW PIVOT FUNCTIONS ---

// Pivot left on the spot (Right wheel forward, Left wheel stopped)
void pivotLeft(int leftSpeed, int rightSpeed) {
  digitalWrite(RIGHT_F_PIN, HIGH);
  digitalWrite(RIGHT_R_PIN, LOW);
  digitalWrite(LEFT_F_PIN, LOW);
  digitalWrite(LEFT_R_PIN, LOW); // Stop left motor
  analogWrite(RIGHT_SPEED_PIN, rightSpeed);
  analogWrite(LEFT_SPEED_PIN, 0);
}

// Pivot right on the spot (Left wheel forward, Right wheel stopped)
void pivotRight(int leftSpeed, int rightSpeed) {
  digitalWrite(RIGHT_F_PIN, LOW); // Stop right motor
  digitalWrite(RIGHT_R_PIN, LOW);
  digitalWrite(LEFT_F_PIN, HIGH);
  digitalWrite(LEFT_R_PIN, LOW);
  analogWrite(RIGHT_SPEED_PIN, 0);
  analogWrite(LEFT_SPEED_PIN, leftSpeed);
}