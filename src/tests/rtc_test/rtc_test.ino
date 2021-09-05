#include <LiquidCrystal.h>
#include <Wire.h>
#include <RTC.h>

#define RS 2
#define EN 3

static DS3231 RTC;
LiquidCrystal lcd(RS, EN, 4, 5, 6, 7);

//#define day   RTC.getDay()
//#define month RTC.getMonth()
//#define year  RTC.getYear()
uint8_t day;
uint8_t month;
uint8_t year;
uint8_t hours;
uint8_t mins;
uint8_t secs;

struct StoreData_s{
   float temp;       
   uint8_t dayx;   
   uint8_t monthx; 
   uint8_t yearx;  
   uint8_t hoursx; 
   uint8_t minsx;  
   uint8_t secsx;  
   uint8_t distancex;     
};
int currentAddress = 0;
StoreData_s StoreData = {0.1,0,0,0,0,0,0,0}; //initializing send struct

void setup()
{
  lcd.begin(16,2);
  RTC.begin();
}

void loop()
{
  getTime(0, 0); 
}

void getTime(char x, char y){
  uint8_t hours = RTC.getHours();
  uint8_t mins  = RTC.getMinutes();
  uint8_t secs  = RTC.getSeconds();
  //Print hours
  if(hours < 10){
    lcd.setCursor(x,y);
    lcd.print("0");
    lcd.print(hours);
  }
  else{
    lcd.setCursor(x,y);
    lcd.print(hours);
  }
  lcd.setCursor(x+2,y);
  lcd.print(":");

  //Print minutes
  if(mins < 10){
    lcd.setCursor(x+3,y);
    lcd.print("0");
    lcd.print(mins);
  }
  else{
    lcd.setCursor(x+3,y);
    lcd.print(mins);
  }
  lcd.setCursor(x+5,y);
  lcd.print(":");

  //Print seconds
  if(secs < 10){
    lcd.setCursor(x+6,y);
    lcd.print("0");
    lcd.print(secs);
  }
  else{
    lcd.setCursor(x+6,y);
    lcd.print(secs);
  }  
  lcd.setCursor(x+8,y);
  lcd.print(" ");  
}
