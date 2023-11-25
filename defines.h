// connecting libraries
#define COLOR_SENSOR_DELAY_CHECK 4       // (ms)


//#define ENABLE_COLOR_SENSOR_PRINTS
#define ENABLE_INIT_PRINTS
#define COLOR_SENSOR_TYPE 1 // 0 - I2C color sensors; 1 - photoResistors and leds



#define ROUND_START_BUTTON A8 
#define SET_NUM_OF_ROUND_BUTTON A9 
#define MODE_TOGGLE_SWITCH A10 
#define TOTAL_ROUND_LENGTH 20000/3

#define LED_ROUND_START A11
#define LED_ROUND_1 A12
#define LED_ROUND_2 A13
#define LED_ROUND_3 A14
#define LED_MODE A15


#define SDApinFront 33
#define SCLpinFront 35

#define SDApinRear 20 
#define SCLpinRear 21



// Піни для інфрачервоних датчиків
const int pinInfraredFront = A1; // Front sensor connected to A1
const int pinInfraredRear = A0;  // Rear sensor connected to A0

// Піни для ультразвукових датчиків
const int pinUltrasonicFrontRightTrig = 12;
const int pinUltrasonicFrontRightEcho = 11;
const int pinUltrasonicFrontLeftTrig = 10;
const int pinUltrasonicFrontLeftEcho = 9;
const int pinUltrasonicRearTrig = 8;
const int pinUltrasonicRearEcho = 5;

// Визначення пінів
const int RPWM = 24; // purple
const int LPWM = 22; // yellow
const int PWM_Right = 7;   // green

const int RPWM2 = 30; // purple
const int LPWM2 = 28; // yellow
const int PWM_Left = 6;   // green

const int pwmValueHigh = 255; // Максимальне значення ШІМ
const int pwmValueLow = 0;    // Мінімальне значення ШІМ

enum MotorDirection { FORWARD, BACKWARD };
unsigned long lastTimeMotorSet_Right = 0;
unsigned long lastTimeMotorSet_Left = 0;

// interrupts for listening port
volatile byte interrupts_count = 0;

long round_length_time = 0;
boolean round_start_flag = 0;


byte curr_mode = 1;
byte curr_round = 1;

enum TuskPosition {
  DISABLE,
  ENABLE
};

/////////////////////////////////////////////////////////////////////////////////
//////////////////// - main system parameters - /////////////////////////////////

byte low_time_left = 20;
byte low_time_right = 20;
boolean leftMotorStatus = 0;
boolean rightMotorStatus = 0;
