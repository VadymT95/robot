void modeButton() {

  if (!digitalRead(MODE_TOGGLE_SWITCH) == HIGH) {
         Serial.println("pressed modeButton");

    if (curr_mode == 1) {

      curr_mode = 2;
    } else {

      curr_mode = 1;
    }
  }
}


void roundButton() {

  if (!digitalRead(SET_NUM_OF_ROUND_BUTTON) == HIGH) {
         Serial.println("pressed roundButton");

    curr_round++;
    if (curr_round == 4) {
      curr_round = 1;
    }
  }
}

void startRoundButton() {
  if (!digitalRead(ROUND_START_BUTTON) == HIGH) {
    round_length_time = millis();
    round_start_flag = 1;
     Serial.println("pressed startRoundButton");
  }
}




void atack_mode(){
    Serial.println("atack_mode");
}
void defence_mode(){
    Serial.println("defence_mode");

}
