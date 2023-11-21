// connecting libraries


#define COLOR_SENSOR_DELAY_CHECK 4       // (ms)


#define ROUND_START_BUTTON A8 
#define SET_NUM_OF_ROUND_BUTTON A9 
#define MODE_TOGGLE_SWITCH A10 
#define TOTAL_ROUND_LENGTH 20000


// interrupts for listening port
volatile byte interrupts_count = 0;

unsigned long round_length_time = 0;
boolean round_start_flag = 0;


unsigned byte curr_mode = 1;
unsigned byte curr_round = 1;
