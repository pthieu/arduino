float fadeSteps = 150; // How many steps to take until full brightness

class Led{
  public:
    Led(int pin, float fadeStep=0);
    void setup();
    // void updateFadeDirection();
    void updateBrightness();

  private:
    int _pin;
    unsigned long _currentTime;
    unsigned long _startTime; // Very beginning of fade period 
    unsigned long _fadePeriod; // How long to fade on and off
    float _fadeStep; // current fade step i.e. 1/255
    int _fadeDirection; // low to high or high to low?
    unsigned long _T_on; // Amount of time to turn LED on
    unsigned long _T_fadeStep; // Amount of time per step increment of brightness
    unsigned long _stepTime; // Start of current step period

    bool _evaluated;
};

Led::Led(int pin, float fadeStep){
  _pin = pin;
  _fadeStep = fadeStep;
}

void Led::setup(){
  pinMode(_pin, OUTPUT);
  _startTime = micros();
  _fadePeriod = 2000000; //microseconds(us)
  _fadeDirection = 1;

  _T_fadeStep = _fadePeriod/2/fadeSteps; // i.e. 2000000us/100steps = 20000us/step of brightness
  _T_on = (_fadeStep/fadeSteps) * _T_fadeStep; // i.e. 20/100 * 20000us = 4000us on
  
  _stepTime = micros();
}

void Led::updateBrightness(){
  _currentTime = micros();
  if (_currentTime - _stepTime > _T_on){
    digitalWrite(_pin, LOW);
  } else {
    digitalWrite(_pin, HIGH);
  }

  if(_currentTime - _stepTime >= _T_fadeStep){
    _fadeStep += _fadeDirection;
    _stepTime = _currentTime;
    _T_on = (_fadeStep/fadeSteps) * _T_fadeStep;
  }

  if(_fadeStep >= fadeSteps-1) {
    _fadeDirection = -1;
  } else if(_fadeStep <= 1) {
    _fadeDirection = 1;
  }
}

Led LED0(0);
Led LED1(1,fadeSteps/5);
Led LED2(2,2*fadeSteps/5);
Led LED3(3,3*fadeSteps/5);
Led LED4(4,4*fadeSteps/5);

void setup() {
  LED0.setup();
  LED1.setup();
  LED2.setup();
  LED3.setup();
  LED4.setup();
}

void loop() {
  LED0.updateBrightness();
  LED1.updateBrightness();
  LED2.updateBrightness();
  LED3.updateBrightness();
  LED4.updateBrightness();
}