
const int debounceDelay = 50; // Затримка для дебаунсу кнопки в мілісекундах
unsigned long lastDebounceTimeRound = 0; // Останній час "дребезгу" для круглої кнопки
unsigned long lastDebounceTimeStart = 0; // Останній час "дребезгу" для кнопки старту
unsigned long last_print1 = 0;

int lastButtonStateRound = LOW; // Попередній стан круглої кнопки
int lastButtonStateStart = LOW; // Попередній стан кнопки старту
char* lastSensorName;
int defaultColorValue;
int lastColorValue;


void modeButton();
void roundButton();
void startRoundButton();
void atack_mode();
void defence_mode();
void setup_leds_and_buttons();
void processSensor(Adafruit_TCS34725softi2c &tcs, const char* sensorName, float multiplier = 1.0);
void init_color_sensors();
