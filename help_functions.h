
const int debounceDelay = 50; // Затримка для дебаунсу кнопки в мілісекундах
unsigned long lastDebounceTimeRound = 0; // Останній час "дребезгу" для круглої кнопки
unsigned long lastDebounceTimeStart = 0; // Останній час "дребезгу" для кнопки старту
int lastButtonStateRound = LOW; // Попередній стан круглої кнопки
int lastButtonStateStart = LOW; // Попередній стан кнопки старту



void modeButton();
void roundButton();
void startRoundButton();
void atack_mode();
void defence_mode();
void setup_leds_and_buttons();
