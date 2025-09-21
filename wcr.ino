#include <Servo.h>

Servo esc;   // ESC for propeller

// Motor driver pins
int IN1 = 8;
int IN2 = 9;
int IN3 = 10;
int IN4 = 11;
int ENA = 5;   // Left motor enable (PWM)
int ENB = 6;   // Right motor enable (PWM)

bool propellerEnabled = false;  // Safety flag

// Approx. pulse width for ~20k RPM
int propellerThrottle = 1820;   // (~82% of max throttle)

void setup() {
  Serial.begin(9600);

  // Attach ESC to pin 3
  esc.attach(3);
  esc.writeMicroseconds(1000);   // Min throttle for arming
  delay(3000);                   // Wait for ESC to arm

  // Motor driver pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  stopWheels();
  Serial.println("System Ready. Waiting for Bluetooth commands...");
}

void loop() {
  if (Serial.available()) {
    char command = Serial.read();
    Serial.println(command);

    switch (command) {
      case 'S':   // START button → enable propeller
        propellerEnabled = true;
        esc.writeMicroseconds(propellerThrottle);  // Set to ~20k RPM
        Serial.println("Propeller ON (~20k RPM)");
        break;

      case 'P':   // PAUSE button → disable everything
        propellerEnabled = false;
        esc.writeMicroseconds(1000);  // Stop propeller
        stopWheels();
        Serial.println("Propeller OFF + Wheels STOP");
        break;

      case 'U':   // UP arrow → move forward (only if suction active)
        if (propellerEnabled) moveForward();
        break;

      case 'D':   // DOWN arrow
        if (propellerEnabled) moveBackward();
        break;

      case 'L':   // LEFT arrow
        if (propellerEnabled) moveLeft();
        break;

      case 'R':   // RIGHT arrow
        if (propellerEnabled) moveRight();
        break;

      case 'X':   // STOP wheels only
        stopWheels();
        break;

      default:
        break;
    }
  }
}

// ----------------- Motor Functions -----------------
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 180);  // Left motor speed (0–255)
  analogWrite(ENB, 180);  // Right motor speed
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, 180);
  analogWrite(ENB, 180);
}

void moveLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 180);
  analogWrite(ENB, 180);
}

void moveRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, 180);
  analogWrite(ENB, 180);
}

void stopWheels() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
