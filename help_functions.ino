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
  int buttonState = digitalRead(SET_NUM_OF_ROUND_BUTTON);
  unsigned long currentMillis = millis();

  // Перевірка стану кнопки та "дребезгу"
  if (buttonState != lastButtonStateRound && (currentMillis - lastDebounceTimeRound) > debounceDelay) {
    lastDebounceTimeRound = currentMillis;

    if (buttonState == HIGH) {
      curr_round++;
      if (curr_round == 4) {
        curr_round = 1;
      }
       switch(curr_round){
        case 1:
            digitalWrite(LED_ROUND_1, HIGH);
        break;
        case 2:
            digitalWrite(LED_ROUND_2, HIGH);
        break;       
        case 3:
            digitalWrite(LED_ROUND_3, HIGH);
        break;        
    }
    }
  }
  lastButtonStateRound = buttonState;
}

void startRoundButton() {
  int buttonState = digitalRead(ROUND_START_BUTTON);
  unsigned long currentMillis = millis();

  // Перевірка стану кнопки та "дребезгу"
  if (buttonState != lastButtonStateStart && (currentMillis - lastDebounceTimeStart) > debounceDelay) {
    lastDebounceTimeStart = currentMillis;

    if (buttonState == HIGH) {
      round_length_time = currentMillis;
      round_start_flag = 1;
      digitalWrite(LED_ROUND_START, HIGH);
    }
  }
  lastButtonStateStart = buttonState;
}





void atack_mode(){
    Serial.println("atack_mode");
}
void defence_mode(){
    Serial.println("defence_mode");

}
