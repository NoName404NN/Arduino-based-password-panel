const int buttons[] = {2, 3, 4, 5, 6};
const int yellowLeds[] = {7, 8, 9, 10, 11};
const int redLed = 12;
const int greenLed = 13;
const int buzzer = A0;

const int password[] = {1, 4, 2, 3, 5}; 
const int passwordLength = 5;

int enteredCode[5] = {0, 0, 0, 0, 0};
int codePosition = 0;
bool inputComplete = false;
unsigned long resetTime = 0;

void setup() {
  for (int i = 0; i < 5; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
    pinMode(yellowLeds[i], OUTPUT);
    digitalWrite(yellowLeds[i], LOW);
  }
  
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, LOW);
  
}

void loop() {
  
  if (inputComplete && millis() - resetTime >= 3000) {
    resetSystem();
  }
  
  
  if (!inputComplete) {
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
   
  enteredCode[codePosition] = buttonNumber;
  
  digitalWrite(yellowLeds[codePosition], HIGH);
  
  codePosition++;
  
  if (codePosition >= passwordLength) {
    verifyPassword();
  }
}

void verifyPassword() {
  inputComplete = true;
  resetTime = millis();
  
  bool correct = true;
  for (int i = 0; i < passwordLength; i++) {
    if (enteredCode[i] != password[i]) {
      correct = false;
      break;
    }
  }
  
  if (correct) {
    digitalWrite(greenLed, HIGH);
    playSuccessSound();
  } else {
    digitalWrite(redLed, HIGH);
    playErrorSound();
  }
}

void playSuccessSound() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    delay(100);
  }
}

void playErrorSound() {
  digitalWrite(buzzer, HIGH);
  delay(500);
  digitalWrite(buzzer, LOW);
}

void resetSystem() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(yellowLeds[i], LOW);
  }
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, LOW);
  
  codePosition = 0;
  inputComplete = false;
  for (int i = 0; i < 5; i++) {
    enteredCode[i] = 0;
  }
  
}
