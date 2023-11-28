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
    //servoRight.attach(3); // Right servo connected to D3
    //servoLeft.attach(4);  // Left servo connected to D4
    //setTusksPosition(DISABLE);

    
   
    Timer2.setPeriod(1000);           // set
    Timer2.enableISR();               // Подключить стандартное прерывание, канал А, без сдига фаз
}



void loop() {
  int d1_filtred = 0;
    int d2_filtred = 0;

  startMoveForward(20);
  while(true){
      Track();
      if(d1 >= 90 && d1 <= 105){
        d1 = last_d1;  
      }else{
        last_d1 = d1;
      }
      Serial.print(d1);
      Serial.print(",");
      d1_filtred = expRunningAverage(d1);
      Serial.print(d1_filtred);
      Serial.print(",");
      d2_filtred = expRunningAverage2(d2);
      Serial.print(d2);
      Serial.print(",");
      Serial.print(0);
      Serial.print(",");
      Serial.println(d2_filtred);
      if(millis() >= 15000){
        stopMotors();
      }
  }
  
  
    if(round_start_flag == 1){
  
        if(curr_mode == 1){
            defence_mode();
        }
        if(curr_mode == 2){
            atack_mode();
        }
    }else{
      digitalWrite(LED_ROUND_START, LOW);
    }

  
////////////////


} 
int counter1 = 0;
int counter2 = 0;
ISR(TIMER2_A) {
      Timer2.disableISR(CHANNEL_A);
      Timer2.stop();
      
      interrupts_count++;

      if(millis() - lastTimeMotorSet_Left >= low_time_left && leftMotorStatus == 1){
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

      if(millis() - lastTimeMotorSet_Right >= low_time_right && rightMotorStatus == 1){
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



      
      if(interrupts_count == COLOR_SENSOR_DELAY_CHECK){
          interrupts_count = 0;

          roundButton();
          modeButton();
          startRoundButton();

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
