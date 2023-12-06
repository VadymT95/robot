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
      defaultColorValue1 = lastColorValue1;
      defaultColorValue2 = lastColorValue2;
      tusk_ararm_flag = 0;
      float voltage = analogRead(voltagePin) * (5.0 / 1023.0) * 5; 
      if(voltage >= 21){
          turn_speed = 45;  
          main_move_speed = 115;
      }else if(voltage >= 20){
          turn_speed = 35; 
          main_move_speed = 108;
      }else{
          turn_speed = 25; 
          main_move_speed = 100;
      }
      if(voltage >= 14){
          interrrupt_voltage_point_boost = voltage * 0.93;
          V_MAX = voltage * 0.88;
          V_MIN = voltage * 0.8;
      }else{
          interrrupt_voltage_point_boost = 20;
          V_MAX = 19.5;
          V_MIN = 18;         
      }
      
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
float calculateGain(float voltage) {
    // Обмежуємо напругу між мінімальною та максимальною границями
    if(voltage >= V_MAX)return GAIN_MAX;
    if(voltage <= V_MIN)return GAIN_MIN;
    voltage = constrain(voltage, V_MIN, V_MAX);

    
    // Обчислюємо коефіцієнт усилення за лінійною залежністю
    float gain = GAIN_MAX + (GAIN_MIN - GAIN_MAX) * (voltage - V_MAX) / (V_MIN - V_MAX);
    
    return constrain(gain, GAIN_MAX, GAIN_MIN);
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
  pinMode(voltagePin, INPUT);
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
    servoRight.attach(2); // Right servo connected to D3
    servoLeft.attach(3);  // Left servo connected to D4
    delay(100);
    
  if (position1 == DISABLE) {
    servoRight.write(40);
    servoLeft.write(120);
  } else {
    servoRight.write(80);
    servoLeft.write(80);
  }
    delay(100);
    servoRight.detach(); // Right servo connected to D3
    servoLeft.detach();  // Left servo connected to D4
    
}

void initialDelay() {
  unsigned long function_start_time = millis();
  while (millis() - function_start_time < 5000) {

  }
}

void stopMotors() {
  boost_permit = 0;
  digitalWrite(RPWM, LOW);
  digitalWrite(LPWM, LOW);
  leftMotorStatus = 0;

  digitalWrite(RPWM2, LOW);
  digitalWrite(LPWM2, LOW);
  rightMotorStatus = 0;
}
void startMoveForward(byte speed_) {
  boost_permit = 1;
  leftMotorStatus = 1;
  low_time_left = speed_;
  rightMotorStatus = 1;
  low_time_right = speed_;
  digitalWrite(RPWM2, LOW);
  digitalWrite(LPWM2, HIGH);
  digitalWrite(RPWM, LOW);
  digitalWrite(LPWM, HIGH);
}
void startMoveBackward(byte speed_) {
  boost_permit = 0;
  leftMotorStatus = 1;
  low_time_left = speed_;
  rightMotorStatus = 1;
  low_time_right = speed_;
  digitalWrite(RPWM2, HIGH);
  digitalWrite(LPWM2, LOW);
  digitalWrite(RPWM, HIGH);
  digitalWrite(LPWM, LOW);
}
void startQuickTurnLeft(byte speed_) {
  boost_permit = 0;
  leftMotorStatus = 1;
  low_time_left = speed_;
  rightMotorStatus = 1;
  low_time_right = speed_;
  digitalWrite(RPWM2, HIGH);
  digitalWrite(LPWM2, LOW);
  digitalWrite(RPWM, LOW);
  digitalWrite(LPWM, HIGH);
}
void startQuickTurnRight(byte speed_) {
  boost_permit = 0;
  leftMotorStatus = 1;
  low_time_left = speed_;
  rightMotorStatus = 1;
  low_time_right = speed_;
  digitalWrite(RPWM2, LOW);
  digitalWrite(LPWM2, HIGH);
  digitalWrite(RPWM, HIGH);
  digitalWrite(LPWM, LOW);
}

void startSlowTurnRight(byte speed_, float slow_percent) {
  boost_permit = 1;
  leftMotorStatus = 1;
  low_time_left = speed_;
  rightMotorStatus = 1;
  low_time_right = (speed_ * slow_percent);
  digitalWrite(RPWM2, LOW);
  digitalWrite(LPWM2, HIGH);
  digitalWrite(RPWM, LOW);
  
  digitalWrite(LPWM, HIGH);          // comment this
  //digitalWrite(LPWM, LOW);           // comment this

}
void startSlowTurnLeft(byte speed_, float slow_percent) {
  boost_permit = 1;
  leftMotorStatus = 1;
  low_time_left = (speed_ * slow_percent);
  rightMotorStatus = 1;
  low_time_right =  speed_;
  //digitalWrite(RPWM2, HIGH);        // comment this
  digitalWrite(RPWM2, LOW);           // comment this
  
  digitalWrite(LPWM2, HIGH);
  digitalWrite(RPWM, LOW);
  digitalWrite(LPWM, HIGH);

  
}

void start_ONE_TurnLeft(byte speed_, float slow_percent) {
  boost_permit = 0;
  leftMotorStatus = 0;
  low_time_left = (speed_ * slow_percent);
  rightMotorStatus = 1;
  low_time_right =  speed_;
  //digitalWrite(RPWM2, HIGH);        // comment this
  digitalWrite(RPWM2, LOW);           // comment this
  
  digitalWrite(LPWM2, HIGH);
  digitalWrite(RPWM, LOW);
  digitalWrite(LPWM, HIGH);

  
}

float expRunningAverage(float newVal) {
  static float filVal = 0;
  filVal += (newVal - filVal) * k;
  return filVal;
}
float expRunningAverage2(float newVal) {
  static float filVal2 = 0;
  filVal2 += (newVal - filVal2) * k;
  return filVal2;
}
float expRunningAverage3(float newVal) {
  static float filVal3 = 0;
  filVal3 += (newVal - filVal3) * k;
  return filVal3;
}
float expRunningAverage4(float newVal) {
  static float filVal4 = 21;
  filVal4 += (newVal - filVal4) * k_voltage;
  return filVal4;
}

float expRunningAverage5(float newVal) {
  static float filVal5 = 1;
  filVal5 += (newVal - filVal5) * k_voltage2;
  return filVal5;
}
void Track()
{
   float dist=230; //distance between two sensors

   //pins for sensor 1 need to be uploaded
   digitalWrite(12, LOW);
   delayMicroseconds(2);
   digitalWrite(12, HIGH);
   delayMicroseconds(10);
   digitalWrite(12, LOW);
   d1_sum += 
   d1 = pulseIn(11, HIGH);
   d1=d1*343/2000;

  
   delay(10);
   digitalWrite(10, LOW);
   delayMicroseconds(2);
   digitalWrite(10, HIGH);
   delayMicroseconds(10);
   digitalWrite(10, LOW);
   d2 = pulseIn(9, HIGH);
   d2=d2*343/2000;
   
   if(d1 >= 75 && d1 <= 110){
       d1 = last_d1;  
   }else{
       last_d1 = d1;
   }
   d1_filtred = expRunningAverage(d1);
   d2_filtred = expRunningAverage2(d2);
      
theta=acos((((d1_filtred*d1_filtred)+(dist*dist)-(d2_filtred*d2_filtred)))/(2*d1_filtred*dist));

if(theta<3 && theta>0 && d1_filtred <= TRACK_DISTANCE_SENSORS && d2_filtred <= TRACK_DISTANCE_SENSORS){               // to avoid impossible values
  X=d1_filtred*cos(theta)+ dist/2; // y coordinate 
  Y=d1_filtred*sin(theta); // X coordinate
  bad_track_left = false;
  bad_track_right = false;
      
}
else{
  #ifdef ENABLE_TRACK_ULTRASONIC_PRINTS
      Serial.print("bad track");
      Serial.print("-------->>>>> ");
      Serial.print(d1_filtred);
      Serial.print("\t");
      Serial.print(" ------ ");
      Serial.print(d2_filtred);
      Serial.println("\t");
  #endif
  //
  if(d1_filtred >= TRACK_DISTANCE_SENSORS){ 
    bad_track_right = true;
  }else{
    bad_track_right = false;
  }
  if(d2_filtred >= TRACK_DISTANCE_SENSORS){ 
    bad_track_left = true;
  }else{
    bad_track_left = false;
  }
      
  /*if(d1_filtred > d2_filtred && d1_filtred < TRACK_DISTANCE_SENSORS && d2_filtred < TRACK_DISTANCE_SENSORS){
      if(d1_filtred - d2_filtred > 20)bad_track_right = true;
      
  }else{ 
      if(d2_filtred - d1_filtred > 20) bad_track_left = true;
  }
      Serial.print("d1_filtred  ");  
      Serial.print(d1_filtred);
      Serial.print("           ");
      Serial.print("d2_filtred  ");
      Serial.print(d2_filtred);
      Serial.print("  bad_track_left-");  
      Serial.print(bad_track_left);
      Serial.print("           ");
      Serial.print("     bad_track_right-");
      Serial.println(bad_track_right);
      */
      
  //
}

//delay(10);
}


///
byte get_enemy_position_horizontaly(){
  float dist = getFrontInfraredDistance_array_5();
    if(dist < TRACK_DISTANCE_SENSORS/10 && d1_filtred < d2_filtred && d2_filtred  - d1_filtred >= 40) {
        return LEFT_SMALL;
    }
    if(dist < TRACK_DISTANCE_SENSORS/10 && d2_filtred < d1_filtred && d1_filtred  - d2_filtred >= 40) {
        return RIGHT_SMALL;
    }
    if(dist < TRACK_DISTANCE_SENSORS/10 && bad_track_right == 0 && bad_track_left == 0) {
        return FRONT;
    }
    if(dist < TRACK_DISTANCE_SENSORS/10 && bad_track_right == 1 && bad_track_left == 1) {
        return FRONT;
    }

    //if(dist > TRACK_DISTANCE_SENSORS/10 && bad_track_right == 1 && bad_track_left == 0) {
    //   return LEFT_LARGE;
    //}
    //if(dist > TRACK_DISTANCE_SENSORS/10 && bad_track_right == 0 && bad_track_left == 1) {
    //    return RIGHT_LARGE;
    //}
    return UNKNOWN_;
}
float getFrontInfraredDistance_array_5(){
  float temp = 0;
  for(int i = 0; i < 5; i++){
     temp += getFrontInfraredDistance();
  }
  return temp/5;
}
float getRearInfraredDistance_array_5(){
  float temp = 0;
  for(int i = 0; i < 5; i++){
     temp += getRearInfraredDistance();
  }
  return temp/5;
}

int read_light_resistor_average(byte pin){
  float temp = 0;
  for(int i = 0; i < 5; i++){
     temp += analogRead(pin);
  }
  return temp/5;
}
////////////////////<<<>>>>>>//////////////////
void atack_round_1() {
  initialDelay();
  while(millis() - round_length_time <= TOTAL_ROUND_LENGTH){
    #ifdef ROUTE_PRINTS
        Serial.println("atack_round_1");
    #endif
  }
}

void atack_round_2() {
  byte stage = 1;
  float result = 200;
  byte enemy_position = UNKNOWN_;
  #ifdef ROUTE_PRINTS
      Serial.println("atack_round_2");
  #endif
  
    //setTusksPosition(ENABLE); 


  initialDelay();
  #ifdef ROUTE_PRINTS
      Serial.println("go");
  #endif
  
  /*
   // startSlowTurnRight(60,1.30);
  while(millis() - round_length_time <= TOTAL_ROUND_LENGTH){
  Track();
    
  Serial.print("aaaaaaaaaaaa    ");
  Serial.print(get_enemy_position_horizontaly());
  Serial.print("  ----  ");
  Serial.print("bad_tr_right ");
  Serial.print(bad_track_right);
  Serial.print("  ----  ");
  Serial.print("bad_tr_left ");
  Serial.print(bad_track_left);
  Serial.print("  ---  ");
  Serial.print("Front ");
  Serial.print(getFrontInfraredDistance());
  
  Serial.print("  ---  d1  ");
  Serial.print(d1_filtred);
  Serial.print("  ----  ");
  Serial.print("d2  ");
  Serial.println(d2_filtred);

  delay(5);

    }
  return;
  */
  if(getFrontInfraredDistance_array_5() <= TRACK_DISTANCE_SENSORS/10){ 
    stage = 2;
  }else{
  startQuickTurnLeft(turn_speed);
  }

    while(millis() - round_length_time <= TOTAL_ROUND_LENGTH){
        //Serial.println("atack_round_2");
        Track();
        if(tusk_ararm_flag == 1) {
          tusk_ararm_flag = 2;
          setTusksPosition(DISABLE);
          }
        if(photoresistor_ararm_flag ==1)continue;
        
        if(stage == 1){
            #ifdef ROUTE_PRINTS
              Serial.println("stage == 1");
            #endif
             while(millis() - round_length_time <= TOTAL_ROUND_LENGTH){   
                 // stopMotors();
                 // Serial.print("getFrontInfraredDistance() == ");
                 // Serial.println(getFrontInfraredDistance());
                  //continue;
                  if(getRearInfraredDistance_array_5() < (TRACK_DISTANCE_SENSORS/10 + 5)){
                    delay(5);
                    start_ONE_TurnLeft(turn_speed+40, 1.70);
                    }
                  result = getFrontInfraredDistance_array_5();
                  if(result < TRACK_DISTANCE_SENSORS/10){
                    delay(8);
                    result = getFrontInfraredDistance_array_5();
                    if(result >= TRACK_DISTANCE_SENSORS/10){
                        stage = 2;
                        continue;
                    }
                  }
                  if(result < TRACK_DISTANCE_SENSORS/10){
                      stopMotors();
                      stage = 2;
                      //startSlowTurnRight(25, 1.60);
                      //if(result > 60)delay(280);
                      //if(result <= 60 && result > 30)delay(180);
                      //if(result <= 30)delay(50);
                      //stopMotors();
                      startMoveForward(turn_speed+20);
                      for(int i = 0; i < 8; i++){  
                        Track();
                      }
                      stopMotors();
                     // stage = 3;
                      break;
                  }
            }
        }
        if(stage == 2){
            enemy_position = get_enemy_position_horizontaly();
               switch(enemy_position){
                case FRONT:
                   //stopMotors();
                    startMoveForward(main_move_speed);
                    #ifdef ROUTE_PRINTS
                      Serial.print("Front ");
                      Serial.print(getFrontInfraredDistance());
                      Serial.println("-forward  ");
                    #endif
                break;
                case LEFT_SMALL:
                    startSlowTurnLeft(main_move_speed, 1.30);
                    #ifdef ROUTE_PRINTS
                      Serial.print("Front ");
                      Serial.print(getFrontInfraredDistance());
                      Serial.println("-LEFT_SMALL 3 3 3 3 3 ");
                    #endif
                break;       
                case RIGHT_SMALL:
                    startSlowTurnRight(main_move_speed, 1.30);
                    #ifdef ROUTE_PRINTS
                      Serial.print("Front ");
                      Serial.print(getFrontInfraredDistance());
                      Serial.println("-RIGHT_SMALL 3 3 3 3 3 ");
                    #endif
                break;     
                case LEFT_LARGE:
                    startSlowTurnLeft(main_move_speed, 1.60);
                    #ifdef ROUTE_PRINTS
                      Serial.print("Front ");
                      Serial.print(getFrontInfraredDistance());
                      Serial.println("-LEFT_LARGE  6 6 6 6 6");
                    #endif
                break;   
                case RIGHT_LARGE:
                    startSlowTurnRight(main_move_speed, 1.60);  
                    #ifdef ROUTE_PRINTS
                      Serial.print("Front ");
                      Serial.print(getFrontInfraredDistance());
                      Serial.println("-RIGHT_LARGE 6 6 6 6 6 6");
                    #endif 
                break; 
                case UNKNOWN_:
                    startQuickTurnLeft(turn_speed+20);
                    #ifdef ROUTE_PRINTS
                      Serial.print("Front ");
                      Serial.print(getFrontInfraredDistance());
                      Serial.println("-UNKNOWN_    startQuickTurnLeft");
                    #endif
                    stage = 1;
                break;            
            }  
        }



        
  }
#ifdef ROUTE_PRINTS
  Serial.println("end");
#endif
}

void atack_round_3() {
  initialDelay();
    while(millis() - round_length_time <= TOTAL_ROUND_LENGTH){
        Serial.println("atack_round_3");
  }
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
  round_start_flag = 0;
  setTusksPosition(DISABLE);
  stopMotors();
}
void defence_mode(){
    Serial.println("defence_mode");

}
