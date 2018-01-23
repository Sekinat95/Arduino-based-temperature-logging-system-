#include <SD.h>
#include <SPI.h>
// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;


char time[30]; 
char date[30];


int CS_pin = 10;

int temp_out = 0;

long id = 1;

 
void setup() {

  Serial.begin(9600);
  
  Serial.println("Initializing card");
  //CS pin is an output
  pinMode(CS_pin, OUTPUT);

//initialize card
      if(!SD.begin(CS_pin))
        {
          Serial.println("Card Failure");
          return;
        
        }
        Serial.println("card ready");
      
        //Write log file header
        File logFile = SD.open("LOG.csv", FILE_WRITE);
        if(logFile)
          {
            logFile.println(", , , ,"); // just a leading blank, incase there was a prev data
            String header = "ID, TEMP, TIME" ;
            logFile.println(header);
            logFile.close();
            Serial.println(header);
          
          }
          else
            {
            Serial.println("couldnt open log file");
            }

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
}

void loop() {
 // read from the RTC sensor

      getTime();     
    int rawV = analogRead(temp_out);
   float milliV = (rawV/1023.0) * 5000;
   float celsius_Temp = milliV/10;
  Serial.println(celsius_Temp);
  Serial.println("°C");
//   Serial.println(rawV);
// // Serial.println("°C");


   //create Data string for storing to SD card using csv format
   String dataString = String(id) + "," + String(celsius_Temp) + "," + time;
//   String dataString = String(id) + "," + String(rawV) + "," + date + "," + time;
//open a  file to write to. only one file can be opened at a time
File logFile = SD.open("LOG.csv", FILE_WRITE);
if (logFile)
  {
    logFile.println(dataString);
    logFile.close();
    Serial.println(dataString);
  }
   else
   {
    Serial.println("could not open log file");
    }
    //increment ID num
    id++;
    delay(5000);
}
void getTime(void)
{
 //get the time and put it into "time" chars
 DateTime now = rtc.now();

 sprintf( time, "%02hhu:%02hhu:%02hhu", now.hour(), now.minute(), now.second() );
  //sprintf( date, "%02hhu:%02hhu:%02hhu", now.day(), now.month(), now.year() );

 Serial.print(now.day(), DEC);
 Serial.print('/');
 Serial.print(now.month(), DEC);
 Serial.print('/');
 Serial.print(now.year(), DEC);
 Serial.print(' ');
 Serial.print(now.hour(), DEC);
 Serial.print(':');
 Serial.print(now.minute(), DEC);
 Serial.print(':');
 Serial.print(now.second(), DEC);
 Serial.println();

 
}
