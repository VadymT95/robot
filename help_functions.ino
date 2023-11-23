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


void atack_mode(){
    Serial.println("atack_mode");
}
void defence_mode(){
    Serial.println("defence_mode");

}

void processSensor(Adafruit_TCS34725softi2c &tcs, const char* sensorName, float multiplier = 1.0) {
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

  if (millis() - last_print1 >= 500 && sensorName != lastSensorName) { // Перевіряємо, чи минуло 50 мс
    last_print1 = millis(); // Оновлюємо час останнього виводу
    lastSensorName = sensorName;
    
    Serial.print(sensorName); Serial.print(" - C:\t"); Serial.print(clear1);
    Serial.print("\tR:\t"); Serial.print(red);
    Serial.print("\tG:\t"); Serial.print(green);
    Serial.print("\tB:\t"); Serial.println(blue);
  
    int isBlack = clear1 < defaultColorValue/2 ? 1 : 0;
    Serial.print(sensorName); Serial.print(" Is Black: "); Serial.println(isBlack);
    
  }
}

void init_color_sensors(){
  Serial.println("Color View Test!");

  if (tcsFront.begin()) {
    Serial.println("Front sensor found");
  } else {
    Serial.println("No front sensor found ... check your connections");
  }

  if (tcsRear.begin()) {
    Serial.println("Rear sensor found");
  } else {
    Serial.println("No rear sensor found ... check your connections");
  }
}
