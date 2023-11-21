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

  
////////////////


} 

ISR(TIMER2_A) {
      Timer2.disableISR(CHANNEL_A);
      Timer2.stop();
      
      interrupts_count++;
      
      if(interrupts_count == COLOR_SENSOR_DELAY_CHECK){
          interrupts_count = 0;
          
        bool buttonStateA8 = !digitalRead(ROUND_START_BUTTON) == LOW;
        bool buttonStateA9 = !digitalRead(SET_NUM_OF_ROUND_BUTTON) == LOW;
        bool buttonStateA10 = !digitalRead(MODE_TOGGLE_SWITCH) == LOW;

        // Виведення стану кнопок в консоль
        Serial.print("Button A8: ");
        Serial.print(buttonStateA8 ? "Pressed" : "Released");
        Serial.print(", Button A9: ");
        Serial.print(buttonStateA9 ? "Pressed" : "Released");
        Serial.print(", Button A10: ");
        Serial.println(buttonStateA10 ? "Pressed" : "Released");                 
      }

    Timer2.resume();
    Timer2.enableISR();              

}
