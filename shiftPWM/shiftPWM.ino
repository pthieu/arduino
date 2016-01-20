/*Software PWM on Attiny85
Ernst Christensen 16.okt. 2011
*/

int led[]={0,1,2,3,4};
int count=0;

void setup(){
  for (int z=1;z<6;z++){
    pinMode(led[z],OUTPUT);
  } //for z
}


void loop(){
  for (int x=1;x<255;x++){
    for (int i=0; i<5; i++){
      spwm(led[i], x,6);
      if (count>4){
        count=0;
      }
    }
  }
  for (int x=254;x>0;x--){
    for (int i=0; i<5; i++){
      spwm(led[i],x,6);
      if (count>4){
        count=0;
      }
    }
  }
}


void spwm(int spin, int freq,int sp){
  //on
  digitalWrite(spin,HIGH);
  delayMicroseconds(sp*freq);

  // off
  digitalWrite(spin,LOW);
  delayMicroseconds(sp*(255-freq));
} //spwm
