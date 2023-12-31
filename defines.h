// connecting libraries
#define COLOR_SENSOR_DELAY_CHECK 4       // (ms)


//#define ENABLE_COLOR_SENSOR_PRINTS
//#define ENABLE_TRACK_ULTRASONIC_PRINTS
#define ENABLE_INIT_PRINTS
#define ROUTE_PRINTS

#define COLOR_SENSOR_TYPE 1 // 0 - I2C color sensors; 1 - photoResistors and leds



#define ROUND_START_BUTTON A8 
#define SET_NUM_OF_ROUND_BUTTON A9 
#define MODE_TOGGLE_SWITCH A10 
#define TOTAL_ROUND_LENGTH 125000
#define TRACK_DISTANCE_SENSORS 1000


#define LED_ROUND_START A14 // 11
#define LED_ROUND_1 A12
#define LED_ROUND_2 A13
#define LED_ROUND_3 A11 // 14
#define LED_MODE A15

#define LIGHT_RESISTOR_1 A6
#define LIGHT_RESISTOR_2 A5
#define voltagePin A7 // Дільник напруги

#define SDApinFront 33
#define SCLpinFront 35

#define SDApinRear 20 
#define SCLpinRear 21

#define RIGHT_MOTOR_BOOST_COEF 1.45

#define GAIN_MAX 1.0f    // при 19.59!!!! треба прописать авто калібровку
#define GAIN_MIN 3.8f
float V_MAX = 17.4f;      // при 19.59!!!! треба прописать авто калібровку
float V_MIN = 15.5f;      // при 19.59!!!! треба прописать авто калібровку
float interrrupt_voltage_point_boost = 18.5f; // при 19.59!!!! треба прописать авто калібровку
boolean boost_permit = 0;
#define GAIN_DIFF (GAIN_MIN - GAIN_MAX)
#define VOLTAGE_RANGE (V_MAX - V_MIN)


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
const int RPWM = 22; // purple
const int LPWM = 24; // yellow
const int PWM_Right = 7;   // green

const int RPWM2 = 30; // purple
const int LPWM2 = 28; // yellow
const int PWM_Left = 6;   // green

const int pwmValueHigh = 255; // Максимальне значення ШІМ
const int pwmValueLow = 0;    // Мінімальне значення ШІМ

enum MotorDirection { FORWARD, BACKWARD };
unsigned long lastTimeMotorSet_Right = 0;
unsigned long lastTimeMotorSet_Left = 0;
boolean photoresistor_ararm_flag = 0;
byte  tusk_ararm_flag = 0;

// interrupts for listening port
volatile byte interrupts_count = 0;

long round_length_time = 0;
boolean round_start_flag = 0;


byte curr_mode = 1;
byte curr_round = 1;

unsigned long previousMillis = 0; 
const long blinkDuration = 200;   
const long pauseDuration = 200;   
const long endPause = 1000;       
int counter = 0;
boolean blink_start_flag = 1;
boolean ledState = 0;


enum TuskPosition {
  DISABLE,
  ENABLE
};
enum EnemyPosition {
  FRONT,
  LEFT_SMALL,
  RIGHT_SMALL,
  LEFT_LARGE,
  RIGHT_LARGE,
  UNKNOWN_
};
/////////////////////////////////////////////////////////////////////////////////
//////////////////// - main system parameters - /////////////////////////////////

byte low_time_left = 10;
byte low_time_right = 10;
byte high_time_left = 1;
byte high_time_right = 1;

boolean leftMotorStatus = 0;
boolean rightMotorStatus = 0;

float  low_voltage_motors_filtred = 21.0;
float  boost_coef = 1.0;

float  X,Y;
float k = 0.1;  // коэффициент фильтрации, 0.0-1.0
float k_voltage = 0.5;
float k_voltage2 = 0.3;

float d1_sum,d2_sum;
float d1,d2, theta;
float last_d1;
  int d1_filtred = 0;
    int d2_filtred = 0;
    
boolean bad_track_left;
boolean bad_track_right;


int turn_speed = 25;
int main_move_speed = 100;
