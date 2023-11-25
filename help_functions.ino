void setup_leds_and_buttons(){
    pinMode(LED_ROUND_START, OUTPUT);
    pinMode(LED_ROUND_1, OUTPUT);
    pinMode(LED_ROUND_2, OUTPUT);
    pinMode(LED_ROUND_3, OUTPUT);
    pinMode(LED_MODE, OUTPUT);
    
    pinMode(ROUND_START_BUTTON, INPUT_PULLUP);
    pinMode(SET_NUM_OF_ROUND_BUTTON, INPUT_PULLUP);
    pinMode(MODE_TOGGLE_SWITCH, INPUT_PULLUP);
}


void modeButton() {
  
    if (!digitalRead(MODE_TOGGLE_SWITCH) == HIGH) {
      curr_mode = 2;
      digitalWrite(LED_MODE, HIGH);
    } else {
      curr_mode = 1;
      digitalWrite(LED_MODE, LOW);
    }
}



void roundButton() {
  int buttonState = !digitalRead(SET_NUM_OF_ROUND_BUTTON);
  unsigned long currentMillis = millis();

  // Перевірка стану кнопки та "дребезгу"
  if (buttonState == 1 && lastButtonStateRound == 0 && millis() - lastDebounceTimeRound > debounceDelay) {
    lastDebounceTimeRound = currentMillis;
    lastButtonStateRound = 1;

      curr_round++;
      if (curr_round == 4) {
        curr_round = 1;
      }
       switch(curr_round){
        case 1:
            digitalWrite(LED_ROUND_1, HIGH);
            digitalWrite(LED_ROUND_3, LOW);
        break;
        case 2:
            digitalWrite(LED_ROUND_1, LOW);
            digitalWrite(LED_ROUND_2, HIGH);
        break;       
        case 3:
            digitalWrite(LED_ROUND_2, LOW);
            digitalWrite(LED_ROUND_3, HIGH);
        break;        
    }
    
  }
    if (buttonState == 0 && lastButtonStateRound == 1) {
    lastButtonStateRound = 0;
  }
}



void startRoundButton() {
  int buttonState = !digitalRead(ROUND_START_BUTTON);
  unsigned long currentMillis = millis();

  // Перевірка стану кнопки та "дребезгу"
  if (buttonState == 1 && lastButtonStateStart == 0 && millis() - lastDebounceTimeStart > debounceDelay) {
    lastDebounceTimeStart = currentMillis;
    lastButtonStateStart = 1;

    if (buttonState == HIGH) {
      round_length_time = currentMillis;
      round_start_flag = 1;
      digitalWrite(LED_ROUND_START, HIGH);
      defaultColorValue = lastColorValue;
    }
  }
  if (buttonState == 0 && lastButtonStateStart == 1) {
    lastButtonStateStart = 0;
  }
}

#if COLOR_SENSOR_TYPE == 0
  void processSensor(Adafruit_TCS34725softi2c &tcs, const char* sensorName) {
    uint16_t clear1, red, green, blue;
  
    tcs.setInterrupt(false); // Включення світлодіода
    //delay(60); // Час для зчитування
    tcs.getRawData(&red, &green, &blue, &clear1);
    tcs.setInterrupt(true); // Вимикання світлодіода
  
   // clear *= multiplier;
   // red *= multiplier;
   // green *= multiplier;
   // blue *= multiplier;
     lastColorValue = clear1;
  
    #ifdef ENABLE_COLOR_SENSOR_PRINTS
    if (millis() - last_print1 >= 500 && sensorName != lastSensorName) {
      last_print1 = millis(); // Оновлюємо час останнього виводу
      lastSensorName = sensorName;
  
  
      Serial.print(sensorName); Serial.print(" - C:\t"); Serial.print(clear1);
      Serial.print("\tR:\t"); Serial.print(red);
      Serial.print("\tG:\t"); Serial.print(green);
      Serial.print("\tB:\t"); Serial.println(blue);
  
      int isBlack = clear1 < defaultColorValue/2 ? 1 : 0;
      Serial.print(sensorName); Serial.print(" Is Black: "); Serial.println(isBlack);
      
    }
    #endif
  }
#endif
void init_color_sensors(){
  #if COLOR_SENSOR_TYPE == 0
    #ifdef ENABLE_INIT_PRINTS
    Serial.println("Color View Test!");
    #endif
    if (tcsFront.begin()) {
      #ifdef ENABLE_INIT_PRINTS
      Serial.println("Front sensor found");
      #endif
    } else {
      #ifdef ENABLE_INIT_PRINTS
      Serial.println("No front sensor found ... check your connections");
      #endif
    }
  
    if (tcsRear.begin()) {
      #ifdef ENABLE_INIT_PRINTS
      Serial.println("Rear sensor found");
      #endif
    } else {
      #ifdef ENABLE_INIT_PRINTS
      Serial.println("No rear sensor found ... check your connections");
      #endif
    }
  #endif
}



// Функції для інфрачервоних датчиків
float getFrontInfraredDistance() {
  int sensorValue = analogRead(pinInfraredFront);
  return convertToDistance(sensorValue);
}

float getRearInfraredDistance() {
  int sensorValue = analogRead(pinInfraredRear);
  return convertToDistance(sensorValue);
}

// Конвертація значення сенсора в відстань
float convertToDistance(int sensorValue) {
  return 30431 * pow(sensorValue, -1.169); // Формула може потребувати калібрування
}

// Функції для ультразвукових датчиків
float getFrontRightUltrasonicDistance() {
  return getDistance(pinUltrasonicFrontRightTrig, pinUltrasonicFrontRightEcho);
}

float getFrontLeftUltrasonicDistance() {
  return getDistance(pinUltrasonicFrontLeftTrig, pinUltrasonicFrontLeftEcho);
}

float getRearUltrasonicDistance() {
  return getDistance(pinUltrasonicRearTrig, pinUltrasonicRearEcho);
}

// Отримання відстані для ультразвукового датчика
float getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2; // Швидкість звуку поділена на 2 (туди і назад)
}

void printSensorsData(){
  Serial.print("Front IR: ");
  Serial.print(getFrontInfraredDistance());
  Serial.print(" cm\tRear IR: ");
  Serial.print(getRearInfraredDistance());
  Serial.print(" cm\tFR Ultr: ");
  Serial.print(getFrontRightUltrasonicDistance());
  Serial.print(" cm\tFL Ultr: ");
  Serial.print(getFrontLeftUltrasonicDistance());
  Serial.print(" cm\tRear Ultr: ");
  Serial.print(getRearUltrasonicDistance());
  Serial.println(" cm");  
}
void setupSensorsPins(){
  pinMode(pinUltrasonicFrontRightTrig, OUTPUT);
  pinMode(pinUltrasonicFrontRightEcho, INPUT);
  pinMode(pinUltrasonicFrontLeftTrig, OUTPUT);
  pinMode(pinUltrasonicFrontLeftEcho, INPUT);
  pinMode(pinUltrasonicRearTrig, OUTPUT);
  pinMode(pinUltrasonicRearEcho, INPUT);  
}
void setupMotorsPins(){
  pinMode(RPWM, OUTPUT);
  pinMode(LPWM, OUTPUT);
  pinMode(PWM_Right, OUTPUT);

  pinMode(RPWM2, OUTPUT);
  pinMode(LPWM2, OUTPUT);
  pinMode(PWM_Left, OUTPUT);  
}
void setTusksPosition(TuskPosition position1) {
  if (position1 == ENABLE) {
    servoRight.write(40);
    servoLeft.write(120);
  } else {
    servoRight.write(80);
    servoLeft.write(80);
  }
}

void initialDelay() {
  unsigned long function_start_time = millis();
  while (millis() - function_start_time < 5000) {
    // Цикл виконується 5 секунд

    // тут має бути визначення позиції робота якщо це можливо. визначення усіх початкових величин. 
  }
}

void stopMotors() {
  digitalWrite(RPWM, LOW);
  digitalWrite(LPWM, LOW);
  leftMotorStatus = 0;

  digitalWrite(RPWM2, LOW);
  digitalWrite(LPWM2, LOW);
  rightMotorStatus = 0;
}
void startMoveForward() {
  leftMotorStatus = 1;
  low_time_left = 20;
  rightMotorStatus = 1;
  low_time_right = 20;
  digitalWrite(RPWM2, LOW);
  digitalWrite(LPWM2, HIGH);
  digitalWrite(RPWM, LOW);
  digitalWrite(LPWM, HIGH);
}

void startTurnLeft() {
  leftMotorStatus = 1;
  low_time_left = 15;
  rightMotorStatus = 1;
  low_time_right = 15;
  digitalWrite(RPWM2, HIGH);
  digitalWrite(LPWM2, LOW);
  digitalWrite(RPWM, LOW);
  digitalWrite(LPWM, HIGH);
}

////////////////////<<<>>>>>>//////////////////
void atack_round_1() {
  setTusksPosition(ENABLE); 
  initialDelay();
  Serial.println("atack_round_1");
}

void atack_round_2() {
  setTusksPosition(ENABLE); 
  initialDelay();
  Serial.println("atack_round_2");
}

void atack_round_3() {
  setTusksPosition(ENABLE); 
  initialDelay();
  Serial.println("atack_round_3");
}



void atack_mode() {

  switch (curr_round) {
    case 1:
      atack_round_1();
      break;
    case 2:
      atack_round_2();
      break;
    case 3:
      atack_round_3();
      break;
    default:
      Serial.println("Invalid round number");
      break;
  }
}
void defence_mode(){
    Serial.println("defence_mode");

}
