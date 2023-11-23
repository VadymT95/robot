#include "defines.h"

#include "Adafruit_TCS34725softi2c.h"
#include "help_functions.h"
#include "GyverTimers.h"
#include "avr/wdt.h"


Adafruit_TCS34725softi2c tcsFront = Adafruit_TCS34725softi2c(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_4X, SDApinFront, SCLpinFront);
Adafruit_TCS34725softi2c tcsRear = Adafruit_TCS34725softi2c(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_4X, SDApinRear, SCLpinRear);


void setup() {
    
    Serial.begin(115200);             // opens serial port, sets data rate to 115200 baud
    delay(1000);

    setup_leds_and_buttons();
    init_color_sensors();
    setupSensorsPins();
   
    Timer2.setPeriod(1000);           // set
    Timer2.enableISR();               // Подключить стандартное прерывание, канал А, без сдига фаз
}


void loop() {
  printSensorsData();
    if(round_start_flag == 1 && millis() - round_length_time <= TOTAL_ROUND_LENGTH){
  
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

ISR(TIMER2_A) {
      Timer2.disableISR(CHANNEL_A);
      Timer2.stop();
      
      interrupts_count++;
      
      if(interrupts_count == COLOR_SENSOR_DELAY_CHECK){
          interrupts_count = 0;

          roundButton();
          modeButton();
          startRoundButton();

          processSensor(tcsFront, "Front"); // Обробка переднього датчика
          processSensor(tcsRear, "Rear"); // Обробка заднього датчика з множником
  
          // виклик функція для перевірки кольору обох датчиків.
          // якщо колір чорний то зупинитися і поставить змінну відповідну у 1. щоб потім від'їхать.  

          // чекать акселерометр щоб знать чи нас штовхнули. якщо штовхнули бити табло. 
                       
      }

    Timer2.resume();
    Timer2.enableISR();              

}
