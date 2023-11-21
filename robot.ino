#include "defines.h"

#include "help_functions.h"
#include "GyverTimers.h"
#include "avr/wdt.h"




void setup() {
    
    Serial.begin(115200);             // opens serial port, sets data rate to 115200 baud
    delay(1000);

    
    pinMode(ROUND_START_BUTTON, INPUT_PULLUP);
    pinMode(SET_NUM_OF_ROUND_BUTTON, INPUT_PULLUP);
    pinMode(MODE_TOGGLE_SWITCH, INPUT_PULLUP);
   
    Timer2.setPeriod(1000);           // set
    Timer2.enableISR();               // Подключить стандартное прерывание, канал А, без сдига фаз
}


void loop() {
  if(round_start_flag == 1 && millis() - round_length_time <= TOTAL_ROUND_LENGTH){

  if(curr_mode == 1){
    defence_mode();
  }
  if(curr_mode == 2){
    atack_mode();
  }
  
}

  
////////////////


} 

ISR(TIMER2_A) {
      Timer2.disableISR(CHANNEL_A);
      Timer2.stop();
      
      interrupts_count++;
      
      if(interrupts_count == COLOR_SENSOR_DELAY_CHECK){
          interrupts_count = 0;

          
          // виклик функція для перевірки кольору обох датчиків.
          // якщо колір чорний то зупинитися і поставить змінну відповідну у 1. щоб потім від'їхать.  

          // чекать акселерометр щоб знать чи нас штовхнули. якщо штовхнули бити табло. 
                       
      }

    Timer2.resume();
    Timer2.enableISR();              

}
