#define green A2
#define red A1

void setup() {
  // put your setup code here, to run once:
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
  greenOff();
  redOff();
}

void loop() {
  // put your main code here, to run repeatedly:
  blinkGreen();
  blinkRed();
}

void greenOn(){
  digitalWrite(green, HIGH);
}

void greenOff(){
  digitalWrite(green, LOW);
}

void blinkGreen(){
  int i;
  for(i=0; i<5; i++){
    greenOn();
    delay(100);
    greenOff();
    delay(100);
  }
}

void redOn(){
  digitalWrite(red, HIGH);
}

void redOff(){
  digitalWrite(red, LOW);
}

void blinkRed(){
  int i;
  for(i=0; i<5; i++){
    redOn();
    delay(100);
    redOff();
    delay(100);
  }
}
