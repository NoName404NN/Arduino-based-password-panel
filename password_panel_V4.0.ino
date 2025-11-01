#include <Servo.h>

const int buttons[] = {2, 3, 4, 5, 6};
const int yellowLeds[] = {7, 8, 9, 10, 11};
const int greenLed = 13;
const int redLed = A1;  // It is necessary to use a 220 ohm resistor.
const int buzzer = A0;
const int servoPin = 12;

const int password[] = {1, 4, 2, 3, 5}; 
const int passwordLength = 5;

int enteredCode[5] = {0, 0, 0, 0, 0};
int codePosition = 0;
bool inputComplete = false;
unsigned long resetTime = 0;
bool isError = false;

bool isLocked = false;
int failedAttempts = 0;
const int maxAttempts = 2;

Servo myServo;

void setup() {
  for (int i = 0; i < 5; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
    pinMode(yellowLeds[i], OUTPUT);
    digitalWrite(yellowLeds[i], LOW);
  }
  
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);    
  pinMode(buzzer, OUTPUT);
  
  digitalWrite(greenLed, LOW);
  digitalWrite(redLed, LOW);
  
  myServo.attach(servoPin);
  myServo.write(0);
  delay(500);
}

void loop() {
  if (isLocked) {
    return;
  }
  
  if ((inputComplete || isError) && millis() - resetTime >= 3000) {
    resetSystem();
  }
  
  if (!inputComplete && !isError) {
    checkButtons();
  }
}

void checkButtons() {
  for (int i = 0; i < 5; i++) {
    if (digitalRead(buttons[i]) == LOW) {
      buttonPressed(i + 1); 
      delay(250); 
      while (digitalRead(buttons[i]) == LOW); 
    }
  }
}

void buttonPressed(int buttonNumber) {
  digitalWrite(buzzer, HIGH);
  delay(50);
  digitalWrite(buzzer, LOW);
  
  if (buttonNumber == password[codePosition]) {
    digitalWrite(yellowLeds[codePosition], HIGH);
    enteredCode[codePosition] = buttonNumber;
    codePosition++;
    
    if (codePosition >= passwordLength) {
      passwordCorrect();
    }
  } else {
    passwordError();
  }
}

void passwordCorrect() {
  inputComplete = true;
  resetTime = millis();
  failedAttempts = 0;
  
  delay(250);
  digitalWrite(greenLed, HIGH);
  playSuccessSound();
  
  myServo.write(180);
  
  fadeOutYellowLeds();
}

void passwordError() {
  isError = true;
  resetTime = millis();
  failedAttempts++;
  
  delay(250);
  digitalWrite(redLed, HIGH);  
  playErrorSound();
  
  fadeOutYellowLeds();
  
  if (failedAttempts >= maxAttempts) {
    activateLock();
  }
}

void activateLock() {
  isLocked = true;
  
  
  digitalWrite(redLed, HIGH);
  
  playLockSound();
  
  for (int i = 0; i < 5; i++) {
    digitalWrite(yellowLeds[i], LOW);
  }
}

void fadeOutYellowLeds() {
  for (int i = 4; i >= 0; i--) {
    digitalWrite(yellowLeds[i], LOW);
    delay(300);
  }
}

void playSuccessSound() {
  delay(250);
  for (int i = 0; i < 2; i++) {
    digitalWrite(buzzer, HIGH);
    delay(50);
    digitalWrite(buzzer, LOW);
    delay(50);
  }
}

void playErrorSound() {
  delay(250);
  digitalWrite(buzzer, HIGH);
  delay(300);
  digitalWrite(buzzer, LOW);
}

void playLockSound() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(buzzer, HIGH);
    delay(50);
    digitalWrite(buzzer, LOW);
    delay(100);
  }
}

void resetSystem() {
  if (isLocked) {
    return;
  }
  
  for (int i = 0; i < 5; i++) {
    digitalWrite(yellowLeds[i], LOW);
  }
  digitalWrite(greenLed, LOW);
  digitalWrite(redLed, LOW);  
  
  
  myServo.write(0);
  
  codePosition = 0;
  inputComplete = false;
  isError = false;
  for (int i = 0; i < 5; i++) {
    enteredCode[i] = 0;
  }
}