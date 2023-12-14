#include "defines.h"

#if COLOR_SENSOR_TYPE == 0
#include "Adafruit_TCS34725softi2c.h"
#endif
#include "help_functions.h"
#include "GyverTimers.h"
#include "avr/wdt.h"
#include <Servo.h>


#if COLOR_SENSOR_TYPE == 0
Adafruit_TCS34725softi2c tcsFront = Adafruit_TCS34725softi2c(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_4X, SDApinFront, SCLpinFront);
Adafruit_TCS34725softi2c tcsRear = Adafruit_TCS34725softi2c(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_4X, SDApinRear, SCLpinRear);
#endif

Servo servoRight;
Servo servoLeft;

void setup() {
    
    Serial.begin(115200);             // opens serial port, sets data rate to 115200 baud
    delay(1000);

    setup_leds_and_buttons();
    init_color_sensors();
    setupSensorsPins();
    setupMotorsPins();
    //servoRight.attach(2); // Right servo connected to D3
    //servoLeft.attach(3);  // Left servo connected to D4

    //ENABLE DISABLE
    setTusksPosition(DISABLE);
   Serial.println("voltage, empty, boost_coef, filtred_voltage");
   
    Timer2.setPeriod(1000);           // set
    Timer2.enableISR();               // Подключить стандартное прерывание, канал А, без сдига фаз
}



void loop() {
//while (true){ 
//  printSensorsData();
//  delay(50);
//}
 /*       
while (true){ 
  Track();
  get_enemy_position_horizontaly();
}*/
  
    if(round_start_flag == 1){
  
        if(curr_mode == 1){
            defence_mode();
        }
        if(curr_mode == 2){
            atack_mode();
        }
    }else{
      GAIN_MIN = START_GAIN_MIN;
      right_motor_add_boost_permit = 0;
      digitalWrite(LED_ROUND_START, LOW);
    }

  
////////////////


} 
int counter1 = 0;
int counter2 = 0;
int counter_backward = 0;
ISR(TIMER2_A) {
      Timer2.disableISR(CHANNEL_A);
      Timer2.stop();
      
      interrupts_count++;
      if(counter_backward > 0) counter_backward--;
      
      if(millis() - lastTimeMotorSet_Left >= (low_time_left/boost_coef) && leftMotorStatus == 1){
          lastTimeMotorSet_Left = millis();
          counter1 = high_time_left;
          analogWrite(PWM_Left, pwmValueHigh);
          //Serial.println("PWM_Left 111");
      }else{
          
          if(counter1 == 0){
            analogWrite(PWM_Left, 0);
          }else{counter1--;}
          //Serial.println("PWM_Left 000");

      }
//right_motor_additional_boost
      if(millis() - lastTimeMotorSet_Right >= ((low_time_right/RIGHT_MOTOR_BOOST_COEF)/(boost_coef*right_motor_additional_boost)) && rightMotorStatus == 1){
          lastTimeMotorSet_Right = millis();
          counter2 = high_time_right;
          analogWrite(PWM_Right, pwmValueHigh);
         // Serial.println("PWM_Right 111");
      }else{
        if(counter2 == 0){
            analogWrite(PWM_Right, 0);
          }else{counter2--;}
         // Serial.println("PWM_Right 000");
      }

/*
      Serial.print("LIGHT_RESISTOR_1 -- ");
      Serial.print(read_light_resistor_average(LIGHT_RESISTOR_1));
            Serial.print("      lastColorValue2 -- ");
      Serial.print(lastColorValue2);
      Serial.print("            LIGHT_RESISTOR_2 -- ");
      Serial.println(read_light_resistor_average(LIGHT_RESISTOR_2));
*/
      
      
      
      
      if(interrupts_count == COLOR_SENSOR_DELAY_CHECK){
          interrupts_count = 0;
          manageBlinking();
          
          lastColorValue1 = analogRead(LIGHT_RESISTOR_1);
          lastColorValue2 = analogRead(LIGHT_RESISTOR_2);


      if(round_start_flag == 1){
          if(start_round_time_for_boost != 0 && millis() - start_round_time_for_boost >= TIME_RIGHT_BOOST_BLOCK ){
              right_motor_add_boost_permit = 1;  
               Serial.println("right_motor_add_boost_permit right_motor_add_boost_permit");
          }else{
              right_motor_additional_boost = 1.0;
              right_motor_add_boost_permit = 0;
          }
          
          if(start_round_time_for_main_boost != 0 && millis() - start_round_time_for_main_boost >= TIME_MAIN_BOOST_BLOCK){
              GAIN_MIN = MAIN_GAIN_MIN;  
          }else{
              GAIN_MIN = START_GAIN_MIN;
          }
               
          if(lastColorValue1 < defaultColorValue1 - 150){
          if(photoresistor_ararm_flag == 0){
              stopMotors();
              //round_length_time = 0;
              counter_backward = 1000;
              photoresistor_ararm_flag = 1;
            }
          }
          if(lastColorValue2 < defaultColorValue2 - 150){
              tusk_ararm_flag = 1;
              if(counter_backward > 1) counter_backward = 1;
          }
        }
        if(counter_backward >= 1 && counter_backward <= 10){
            stopMotors();
            counter_backward = 0; 
          }else if(counter_backward > 0){
              startMoveBackward(40);
          }else{
              photoresistor_ararm_flag = 0;
          }

          
          roundButton();
          modeButton();
          startRoundButton();
          
          float voltage = analogRead(voltagePin) * (5.0 / 1023.0) * 5;   
         
          if(voltage < interrrupt_voltage_point_boost && voltage > 10.0){   
            low_voltage_motors_filtred = expRunningAverage4(voltage);
            if( boost_permit == 1){
              boost_coef = expRunningAverage5(calculateGain(low_voltage_motors_filtred));
            }else{
              right_motor_additional_boost = 1.0;
              boost_coef = 1.0;
            }
          }
          /*  Serial.print(voltage);
            Serial.print(",");
            Serial.print(12);
            Serial.print(",");
            Serial.print(boost_coef);
            Serial.print(",");          
            Serial.println(low_voltage_motors_filtred);
            */
          #if COLOR_SENSOR_TYPE == 0
              processSensor(tcsFront, "Front"); // Обробка переднього датчика
              processSensor(tcsRear, "Rear"); // Обробка заднього датчика з множником
          #endif
  
          // виклик функція для перевірки кольору обох датчиків.
          // якщо колір чорний то зупинитися і поставить змінну відповідну у 1. щоб потім від'їхать.  

          // чекать акселерометр щоб знать чи нас штовхнули. якщо штовхнули бити табло. 
                       
      }

    Timer2.resume();
    Timer2.enableISR();              

}
