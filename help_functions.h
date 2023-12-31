
const int debounceDelay = 50; // Затримка для дебаунсу кнопки в мілісекундах
unsigned long lastDebounceTimeRound = 0; // Останній час "дребезгу" для круглої кнопки
unsigned long lastDebounceTimeStart = 0; // Останній час "дребезгу" для кнопки старту
unsigned long last_print1 = 0;

int lastButtonStateRound = LOW; // Попередній стан круглої кнопки
int lastButtonStateStart = LOW; // Попередній стан кнопки старту
char* lastSensorName;
int defaultColorValue1;
int defaultColorValue2;
int lastColorValue1;
int lastColorValue2;

void modeButton();
void roundButton();
void startRoundButton();
void atack_mode();
void defence_mode();
void setup_leds_and_buttons();

#if COLOR_SENSOR_TYPE == 0
    void processSensor(Adafruit_TCS34725softi2c &tcs, const char* sensorName);
#endif

void init_color_sensors();
float getFrontInfraredDistance();
float getRearInfraredDistance();
float convertToDistance(int sensorValue);
float getFrontRightUltrasonicDistance();
float getFrontLeftUltrasonicDistance();
float getRearUltrasonicDistance();
float getDistance(int trigPin, int echoPin);

void printSensorsData();
void setupSensorsPins();
float expRunningAverage(float newVal);
float expRunningAverage(float newVal);
int read_light_resistor_average(byte pin);

void stopMotors();
void startMoveForward(byte speed_);
void startMoveBackward(byte speed_);
void startQuickTurnLeft(byte speed_);
float calculateGain(float low_voltage_motors_filtered_local);
