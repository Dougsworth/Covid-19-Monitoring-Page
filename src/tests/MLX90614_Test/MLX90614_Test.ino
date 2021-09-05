#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal.h>

#define RS 2
#define EN 3
#define Backlight 10
LiquidCrystal lcd(RS, EN, 4, 5, 6, 7);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();


void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2); 
  mlx.begin();  
}

void loop() {
  backlightOn();
  
  //Object Temperature
  Serial.print("Object:  ");
  Serial.print(mlx.readObjectTempC());
  lcd.setCursor(0,0);
  lcd.print("Object:  ");
  lcd.print(mlx.readObjectTempC());

  //Ambient Temperature
  Serial.print("\tAmbient: ");
  Serial.println(mlx.readAmbientTempC());
  lcd.setCursor(0,1);
  lcd.print("Ambient: ");
  lcd.print(mlx.readAmbientTempC());
}


void backlightOn(){
  digitalWrite(Backlight, HIGH);
}

void backlightOff(){
  digitalWrite(Backlight, LOW);
}
