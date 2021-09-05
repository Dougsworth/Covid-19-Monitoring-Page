#define buzzer A3
#define frequency 2000
#define duration 1000

void setup(){
  pinMode(buzzer, OUTPUT);
}

void loop(){
  tone(buzzer, frequency, duration);
  delay(2000);
  tone(buzzer, 0, duration);
  delay(2000);  
}
