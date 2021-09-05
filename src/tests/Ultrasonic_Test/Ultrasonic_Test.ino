#include <Ultrasonic.h>
#include <LiquidCrystal.h>

#define Echo 8
#define Trig 9
#define Threshold 50

//Distance Sensor Object
Ultrasonic ultrasonic(Trig, Echo);
int distance;  

#define RS 2
#define EN 3
LiquidCrystal lcd(RS, EN, 4, 5, 6, 7);

int x = 0;
int n;
unsigned char counter = 0;

int readDistance() {
  int aggregate = 0;
  n = 20;
  float temp = 31.2;

//  for(counter = 0 ; counter < n ; counter ++){
//    x = ultrasonic.read();
//    aggregate += x;
//    delayMicroseconds(5);
//  }
//   return aggregate/counter;

  return ultrasonic.read();
}

void setup () { 
  lcd.begin(16,2);
  digitalWrite(10, HIGH);
}

void loop(){
  lcd.setCursor(0, 0);
  lcd.print("Dist: ");
  lcd.print(ultrasonic.read());
  delay(1000);
  lcd.clear();
}
