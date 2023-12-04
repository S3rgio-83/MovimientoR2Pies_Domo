#include "BTS7960.h"

#define CH1_PIN 4
#define CH2_PIN 5
#define CH3_PIN 6

#define MOTOR1_SPEED_PIN 9
#define MOTOR1_DIR_PIN 2
#define MOTOR2_SPEED_PIN 10
#define MOTOR2_DIR_PIN 3

////////motor Domo////////
const uint8_t EN = 7;
const uint8_t L_PWM = 8;
const uint8_t R_PWM = 11;
//const uint8_t ch3_pin = 6;

BTS7960 motorController(EN, L_PWM, R_PWM);

void setup() {
  Serial.begin(115200);
  pinMode(CH1_PIN, INPUT);
  pinMode(CH2_PIN, INPUT);
  pinMode(CH3_PIN, INPUT);
  
  pinMode(MOTOR1_SPEED_PIN, OUTPUT); 
  pinMode(MOTOR1_DIR_PIN, OUTPUT);
  pinMode(MOTOR2_SPEED_PIN, OUTPUT);
  pinMode(MOTOR2_DIR_PIN, OUTPUT);
}

void loop() {
  motorController.Enable();
  
  int ch1Value = pulseIn(CH1_PIN, HIGH);
  int ch2Value = pulseIn(CH2_PIN, HIGH);
  int ch3Value = pulseIn(CH3_PIN, HIGH);
  
  // Print the channel values
  printChannelValues(ch1Value, ch2Value, ch3Value);

  int pwmValue1 = map(ch1Value, 1070, 1910, 0, 255);
  int pwmValue2 = map(ch2Value, 1070, 1910, 0, 255);

  int direction1 = 0; // 0: Neutral, 1: Forward, -1: Reverse
  int direction2 = 0; // 0: Neutral, 1: Forward, -1: Reverse

  if (ch1Value > 1450 && ch1Value < 1550) {
    // Neutral position, stop the motor
    pwmValue1 = 0;
  } else if (ch1Value <= 1450) {
    // Reverse direction
    direction1 = -1;
    pwmValue1 = map(ch1Value, 1450, 1070, 0, 127);
  } else if (ch1Value >= 1550) {
    // Forward direction
    direction1 = 1;
    pwmValue1 = map(ch1Value, 1550, 1910, 0, 127);
  }

  if (ch2Value > 1450 && ch2Value < 1550) {
    // Neutral position, stop the motor
    pwmValue2 = 0;
  } else if (ch2Value <= 1450) {
    // Reverse direction
    direction2 = -1;
    pwmValue2 = map(ch2Value, 1450, 1070, 0, 127);
  } else if (ch2Value >= 1550) {
    // Forward direction
    direction2 = 1;
    pwmValue2 = map(ch2Value, 1550, 1910, 0, 127);
  }

  // Control del motor 1
  analogWrite(MOTOR1_SPEED_PIN, pwmValue1);
  digitalWrite(MOTOR1_DIR_PIN, direction1 > 0 ? HIGH : LOW);

  // Control del motor 2
  analogWrite(MOTOR2_SPEED_PIN, pwmValue2);
  digitalWrite(MOTOR2_DIR_PIN, direction2 > 0 ? HIGH : LOW);
  
/////////motor Domo////////
if (ch3Value >= 1070 && ch3Value <= 1480) {
    // Map the PWM signal to the motor speed for one direction
    int speed = map(ch3Value, 1070, 1480, 255, 0);
    Serial.print("Moving in one direction at speed: ");
    Serial.println(speed);
    
    motorController.TurnRight(speed);
  } else if (ch3Value >= 1530 && ch3Value <= 1910 ) {
    // Map the PWM signal to the motor speed for the other direction
    int speed = map(ch3Value, 1530, 1910, 0, 255);
    Serial.print("Moving in the other direction at speed: ");
    Serial.println(speed);

    motorController.TurnLeft(speed);
  } else {
    // Stop if the PWM signal is outside both ranges
    Serial.println("Stopping");
    motorController.Stop();
  }

  motorController.Disable();

  delay(10);
}

// Función para imprimir los valores de los canales en el Monitor Serie
void printChannelValues(int ch1Value, int ch2Value, int ch3Value) {
  Serial.print("CH1: ");
  Serial.print(ch1Value);
  Serial.print(" - CH2: ");
  Serial.print(ch2Value);
  Serial.print(" - CH3: ");
  Serial.println(ch3Value);
}
