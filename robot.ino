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

      if(millis() - lastTimeMotorSet_Right >= ((low_time_right/RIGHT_MOTOR_BOOST_COEF)/boost_coef) && rightMotorStatus == 1){
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


      //Serial.print("LIGHT_RESISTOR_1 -- ");
      //Serial.print(read_light_resistor_average(LIGHT_RESISTOR_1));
      //Serial.print("            LIGHT_RESISTOR_2 -- ");
      //Serial.println(read_light_resistor_average(LIGHT_RESISTOR_2));

      
    /*  lastColorValue = read_light_resistor_average(LIGHT_RESISTOR_1);
      
      if(analogRead(LIGHT_RESISTOR_1) < defaultColorValue - 150 || analogRead(LIGHT_RESISTOR_2) < defaultColorValue - 150){
          stopMotors();
          round_start_flag = 0;
          round_length_time = 0;
      }
      */
      if(interrupts_count == COLOR_SENSOR_DELAY_CHECK){
          interrupts_count = 0;
          roundButton();
          modeButton();
          startRoundButton();
          
          float voltage = analogRead(voltagePin) * (5.0 / 1023.0) * 5;   
         
          if(voltage < 18.5 && voltage > 10.0){   
            low_voltage_motors_filtred = expRunningAverage4(voltage);
            boost_coef = expRunningAverage5(calculateGain(low_voltage_motors_filtred));
          }
            Serial.print(voltage);
            Serial.print(",");
            Serial.print(12);
            Serial.print(",");
            Serial.print(boost_coef);
            Serial.print(",");          
            Serial.println(low_voltage_motors_filtred);
            
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
