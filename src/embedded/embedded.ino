/*
 * Project: COVID-19 Automated Temperature Sensor
 * 
 * Project Supervisors: Dr. Lindon Falconer
 *                      Paul Wilson
 *                      
 * Authors: Jordan Madden
 *          Alick Campbell
 *          Jermaine James
 *          Douglas Byfield
 *          
 * This is a project undertaken by the Faculty of Engineering at The University of the West
 * Indies, Mona Campus in response to the COVID-19 pandemic.
 */

#include <Adafruit_MLX90614.h>
#include <LiquidCrystal.h>
#include <Ultrasonic.h>
#include <EEPROM.h>
#include <Wire.h>
#include <RTC.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//Temperature Sensor 
Adafruit_MLX90614 mlx = Adafruit_MLX90614();  
float ambientTemp;
float objectTemp;
uint8_t tempState = 0;

//LCD
#define RS 2
#define EN 3
#define Backlight 10
LiquidCrystal lcd(RS, EN, 4, 5, 6, 7);

//RTC
static DS3231 RTC;
uint8_t day;
uint8_t month;
uint8_t year;
uint8_t hours;
uint8_t mins;
uint8_t secs;

//Ultrasonic
#define Echo 8
#define Trig 9
Ultrasonic ultrasonic(Trig, Echo);

//Buzzer
#define buzzer    A3
#define frequency 2500

//RG LED
#define red   A1
#define green A2

//Buttons
#define modeButton  A0

//Switching Constants
#define amb 1
#define obj 2

//Interrupt
volatile uint8_t mode = 0;
volatile uint8_t index = 0;

//EEPROM
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
int currentAddress;
StoreData_s StoreData = {0.1,0,0,0,0,0,0,0}; //initializing send struct
StoreData_s RecData   = {0.1,0,0,0,0,0,0,0}; //initializing recieve struct;

void interruptSetup(){
  //CONFIGURE MODE BUTTON
  pinMode(modeButton, INPUT_PULLUP);
  
  //CONFIGURE INTERRUPT FOR MODE BUTTON 
  //select pin which will trigger the interrupt
  PCMSK1 |= 0b00000001;
  
  //PCINT[14:8] - group which this pin is from
  //Clears the interurpt flag
  //PCIFR |= (1<<1);
  
  //enable pin change interrupt for the pins selected
  PCICR |= 0b00000010;
  
  //enable global interrupt
  sei();  
}

void soundBuzzer(unsigned long durationInMillis){
  tone(buzzer, frequency, durationInMillis);
}

void greenOn(){
  //Turn on green light
  digitalWrite(green, HIGH);
}

void greenOff(){
  //Turn off green light
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
  //Turn on red light
  digitalWrite(red, HIGH);
}

void redOff(){
  //Turn off red light
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

void orangeOn(){
  //Turn on both green and red to get orange
  digitalWrite(red, HIGH);
  digitalWrite(green, HIGH);
}

void orangeOff(){
  //Turn on both green and red to get orange
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
}

void blinkOrange(){
  int i;
  for(i=0; i<5; i++){
    orangeOn();
    delay(100);
    orangeOff();
    delay(100);
  }
}

void backlightOn(){
  digitalWrite(Backlight, HIGH);
}

void backlightOff(){
  digitalWrite(Backlight, LOW);
}

void printMessage(String msg, unsigned long durationInMillis){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(msg);
  delay(durationInMillis);
}

void getTemperature(){
  ambientTemp = mlx.readAmbientTempC();
  objectTemp = mlx.readObjectTempC();
}

void getDateAndTime(){
  //Get the date and time from the RTC
  day   = RTC.getDay();
  month = RTC.getMonth();
  year  = RTC.getYear();
  hours = RTC.getHours();
  mins  = RTC.getMinutes();
  secs  = RTC.getSeconds();
}

void displayTimeAndTemp(int var){
  getDateAndTime();
  getTemperature();
  
  //Display the current time 
  printTime(4, 0);

  //Display either the ambient or object temperature
  if(var == 1){
    lcd.setCursor(0, 1);
    lcd.print("Ambient: "); lcd.print(ambientTemp); lcd.print((char)223); lcd.print("C");
  }
  if(var == 2){
    lcd.setCursor(0, 1);
    lcd.print("Temp:  "); lcd.print(objectTemp); lcd.print((char)223); lcd.print("C");
    lcd.print("   ");
  }
}

void printTime(unsigned char x, unsigned char y){     
  //If the hour value is less than 10, add a 0 in front of it for aesthetic purposes,
  //otherwise let it display normally
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

  //If the minute value is less than 10, add a 0 in front of it for aesthetic purposes,
  //otherwise let it display normally
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

  //If the second value is less than 10, add a 0 in front of it for aesthetic purposes,
  //otherwise let it display normally
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

void storeInMemory(float temp, int distancex)
{
  getDateAndTime();
  
  StoreData.temp = temp;
  StoreData.dayx = day;
  StoreData.monthx = month;
  StoreData.yearx = year;
  StoreData.hoursx = hours;
  StoreData.minsx = mins;
  StoreData.secsx = secs;
  StoreData.distancex = distancex;
  EEPROM.put(currentAddress, StoreData);
  currentAddress += sizeof(StoreData);
  
  if (currentAddress >= 990) // FOR 90 RECORDS @11bytes
  {
    currentAddress = 0;
  }

  //Store the current address in EEPROM in case of power loss or reset 
  EEPROM.update(999, currentAddress);
}

void accessMemory(uint8_t index){
  const int remember;
  int addr;
  addr = sizeof(RecData) * index;
  EEPROM.get(addr, RecData);
  displayEEP(&RecData);
}

void displayEEP(StoreData_s *p) {
  //Display the temperature
  lcd.setCursor(1, 0);
  lcd.print("Temp: "); lcd.print(p->temp); lcd.print((char)223); lcd.print("C");

  //Display Date and Time
  int yearx = 20;   
  if((p->dayx == 1 ) && (p->monthx == 1)){
    yearx ++;
  }
  lcd.setCursor(0, 1);
  lcd.print(p->dayx); lcd.print("/"); lcd.print(p->monthx); lcd.print("/"); lcd.print(yearx);
  lcd.print(" ");
  lcd.print(p->hoursx); lcd.print(":"); lcd.print(p->minsx); lcd.print(":"); lcd.print(p->secsx);   
  
  if((p->dayx == 255)){
    printMessage("NO DATA AS YET", 1000);
    //lcd.clear();
  }
}

void normalOperation(){
  displayTimeAndTemp(amb);
  greenOn();
  redOff();
  backlightOff();
  boolean state = false;
  int objectDistance = ultrasonic.read();
  
  //If the object is close enough, read the objects temperature and take necessary actions
  if(objectDistance < 6){
    state = true;
    
    while(state){
      objectDistance = ultrasonic.read();
      tempState++;
      delay(300);
      
      if(tempState == 1){        
        displayTimeAndTemp(obj); 
        soundBuzzer(300);
        backlightOn();
        blinkOrange();
        delay(1500);
    
        //If temperature is a fever then sound the alarm, display red status on led and store
        //data in memory, otherwise indicate that the person is safe
        if(objectTemp >= 37.60){       
          if(tempState == 1) {
            redOn();
            soundBuzzer(2000);
            printMessage("HIGH TEMPERATURE", 1500);
            storeInMemory(objectTemp, objectDistance);
            delay(500);
            redOff();
          }
        }else{
          greenOn();
          printMessage("SAFE TEMPERATURE", 1500);
          greenOff();
        }

        //An arbitrary value that will not satisfy the conditional statements above
        tempState = 100;
      }

      objectDistance = ultrasonic.read();

      //When the person moves out of range, indicate that the system is ready to measure
      //the next person
      if(objectDistance >= 6){
        tempState = 0;
        printMessage("NEXT PERSON", 2000);
        lcd.clear();
        break;
      }      
    }  
  }  
}

ISR(PCINT1_vect) {
  _delay_ms(20);
  
  //Increment a variable each time the button is pressed
  if(digitalRead(modeButton) == LOW){
    if(mode == 1) {
      index++;
    } else {
      mode++;
    }
    _delay_ms(300);
  
    //If the button is held down for 2 seconds, then leave memory access mode and
    //resume normal operation
    while(digitalRead(modeButton) == LOW){ 
      _delay_ms(1700);
      if(digitalRead(modeButton) == LOW){
        mode = 0;
        index = 0;
        backlightOff();
        _delay_ms(1000);
        lcd.clear();
        break;
      }
    }
  }
}

void setup() {
  //Temperature Sensor Setup
  mlx.begin();  

  //LCD Setup
  lcd.begin(16, 2); 
  pinMode(Backlight, OUTPUT);
  backlightOff();

  //RTC Setup
  RTC.begin();

  //Buzzer Setup
  pinMode(buzzer, OUTPUT);

  //Button Setup
//  pinMode(modeButton, INPUT);

  //RG LED Setup
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
  greenOff();
  redOff();  

  //Interrupt Setup
  interruptSetup();

  //Configuration in case of power loss or reset. If the EEPROM is not empty
  //then load the current address from memory
  if(EEPROM.read(1) != 255){
    currentAddress = EEPROM.read(999);
  }else{
    currentAddress = 0;
  }
}

void loop() {
  switch (mode) {
    case 0:         //Normal Mode
      mode = 0;
      lcd.clear();
      while(mode == 0){
        normalOperation();
      }
      break;
  
    case 1:         //Memory Access Mode
      backlightOn();
      printMessage("RECORDED DATA", 1000);
      lcd.clear();
      while(mode == 1){
        if(index == 90){
          index = 0;
        }
        accessMemory(index);
      }     
      break;
  }
}
