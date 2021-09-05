#include <SPI.h>
#include <SD.h>

#define FILENAME "covid_data.txt"

File myFile;

void sdSetup(){
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  Serial.print("Initializing SD card...");
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  
  // SD.begin(10)
}

void sdWrite() {
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open(FILENAME, FILE_WRITE);
  
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to" +FILENAME+ "...");
    myFile.println("This is a test file :)");
    myFile.println("testing 1, 2, 3.");
    for (int i = 0; i < 20; i++) {
      myFile.println(i);
    }
    
    // close the file:
    myFile.close();
    Serial.println("done.");
  } 
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void sdRead(){
  // open the file for reading:
  myFile = SD.open(FILENAME);
  if (myFile) {
    Serial.println("test.txt:");
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } 
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  
  sdSetup();

  // Write to the SD card
  sdWrite();

  // Read the data from the SD card
  sdRead(); 
}

void loop() {
  
}
