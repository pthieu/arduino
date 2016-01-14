
// CONFIGURATION
// Pins
const int latchPin = 2;
const int clockPin = 4;
const int dataPin = 3;

// Timers
const int countdown = 5; // amount of time to countdown from in seconds. i.e. 60*60 = 3600 seconds = 1 hour
const long reset_interval = countdown*1000000; // interval before reset in microseconds, i.e. if countdown is set to 60s, every 60 seconds will reset display to countdown start
const long water_motor_ON_duration = 1*1000000; // length of time to turn water on in microseconds 
 
// States
unsigned long last_display_update_time = 0; // Holds the last time output change was checked
unsigned long water_start_time = 0; // Holds the time when water start
unsigned long curr_time = 0; // Temporary var to grab current time and make global
bool water_trigger_EN = false; // Holds state of water motor on or off
bool fourDigit7Segment_enable = true; // set if we are using 4 digit 7 segment led

// States
byte leds = 0;
byte digits = 0;

int output_count=0;
 
void setup() 
{
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
}
 
void loop() 
{
  updateTimers(); // checks what the countdown is at
  int timeleft = countdown - (curr_time - last_display_update_time)/1000000;
  int minutesleft = timeleft/60; // i.e. (3600s)/(60s/1min) = 60min
  int secondsleft = timeleft%60; // i.e. (3600s)%60 = 0s remaining
  leds = getSevenSegmentFormation(minutesleft/10);
  updateShiftRegister(0);
  leds = getSevenSegmentFormation(minutesleft%10);
  leds = leds | 0x80;
  updateShiftRegister(1);
  leds = getSevenSegmentFormation(secondsleft/10);
  updateShiftRegister(2);
  leds = getSevenSegmentFormation(secondsleft%10);
  updateShiftRegister(3);
}
 
void updateShiftRegister(int digit)
{
  digitalWrite(latchPin, LOW); // Gets latch ready by setting to low since we need a rising edge to trigger
  if(fourDigit7Segment_enable){
    int digitON = micros();
    digits = 0; // reset all digits so it clears previous bit setting
    bitWrite(digits, digit, 1); // set the digit we want to high
    // If the motor is triggered on, we are going to shift out the corresponding bit to HIGH
    if (water_trigger_EN){
      digits = digits | B10000000; // since it's MSBFIRST, this will hit the last output of the digit selection shift register  
    }
    //shift out digit control first in LSB
    shiftOut(dataPin, clockPin, MSBFIRST, digits); // shiftOut will write to dataPin and trigger a rising on clickPin at same time while writing byte value from leds var
  }
  shiftOut(dataPin, clockPin, MSBFIRST, leds); // shiftOut will write to dataPin and trigger a rising on clickPin at same time while writing byte value from leds var
  digitalWrite(latchPin, HIGH); // Trigger with rising edge
}

//Turns off all 7 segments of a digit, will need to specify digits 0 to n
void clearMemory(int i){
  leds = 0;
  updateShiftRegister(i);
}

//Updating timer for resetting countdown
//Will take absolute current time and compare it to the last time checked, if it is greater than countdown value, we reset
void updateTimers(){
  curr_time = micros();
  if (curr_time - last_display_update_time >= reset_interval){
    last_display_update_time = curr_time; // reset_interval set to 10000000, because LED counts 0 to 9, and then needs to reset // actually set to 60*1000000 to countdown 1 minute
    water_trigger_EN = true; // everytime we reset the timer, means it's time to trigger the water motor
    water_start_time = curr_time; // set the start time of water
  }
  if (water_trigger_EN == true && (curr_time - water_start_time >= water_motor_ON_duration)){
    water_trigger_EN = false; // Turn off motor if length of time past is greater than the duration we want
  }
}

byte getSevenSegmentFormation(int i){
  switch (i) {
      case 0:
        return 0x7E; //01111110
        break;
      case 1:
        return 0x48; //01001000
        break;
      case 2:
        return 0x3D; //00111101
        break;
      case 3:
        return 0x6D; //01101101
        break;
      case 4:
        return 0x4B; //01001011
        break;
      case 5:
        return 0x67; //01100111
        break;
      case 6:
        return 0x77; //01110111
        break;
      case 7:
        return 0x4C; //01001100
        break;
      case 8:
        return 0x7F; //01111111
        break;
      case 9:
        return 0x6F; //01101111
        break;   
      default:
        return 0x0; //Arduino doesn't handle bytes in binary
  }
}