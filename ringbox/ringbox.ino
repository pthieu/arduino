int ledPin = 0;
int fadeDirection = 1;
int brightness = 0;
int minBrightness = 0;
int maxBrightness = 255;

const int period = 2000; // in ms
unsigned long previousTime = 0;        // will store last time LED was updated
unsigned long currentTime = 0;

int fadeStep = period/(maxBrightness*2);

class Led{
  public:
    Led(int pin, int brightness=0);
    void setup();
    void updateFadeDirection();
    void updateBrightness();

  private:
    int _pin;
    int _fadeDirection;
    int _brightness;
    int _minBrightness;
    int _maxBrightness;
};

Led::Led(int pin, int brightness){
  _pin = pin;
  _brightness = brightness;
}

void Led::setup(){
  pinMode(_pin, OUTPUT);
  _fadeDirection = 1;
  _minBrightness = 0;
  _maxBrightness = 255;
}

void Led::updateFadeDirection(){
  if (_brightness <= minBrightness) {
    _fadeDirection = 1;
    _brightness = 0;
  }
  else if (_brightness >= maxBrightness) {
    _fadeDirection = -1;
    _brightness = 255;
  }
}

void Led::updateBrightness(){
  _brightness += _fadeDirection;
  analogWrite(_pin, _brightness);
  updateFadeDirection();
}

Led LED0(7);
Led LED1(11);
Led LED2(12);
Led LED3(13);

void setup() {
  LED0.setup();
  LED1.setup();
  LED2.setup();
  LED3.setup();
  currentTime = millis();
}

void loop() {
  currentTime = millis();

  if (currentTime - previousTime >= fadeStep) {
    LED0.updateBrightness();
    LED1.updateBrightness();
    LED2.updateBrightness();
    LED3.updateBrightness();
    previousTime = currentTime;
  }
}