#include <TimeLib.h>
#include <SD.h>
#include <SPI.h>
#include "DHT.h"

// dht11 sensor
#define DHTPIN 2 
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// SD card
File sdCardFile;
String fileName;
int csPin = 10; //for uno

// soil moisture sensor
const int soilMoistureSensorPin = A0;
float soilMoisture;  

float output;

void setup() {
  Serial.begin(9600);
  
  pinMode(soilMoistureSensorPin, INPUT);
  pinMode(csPin, OUTPUT);
  
  dht.begin();
  
  // sd card 
  if (SD.begin())
  {
    Serial.println("SD card is ready to use");
  } else
  {
    Serial.println("SD card initialization failed");
    return;
  }

  // set before uploading !!!
  setTime(00,00,00,5,12,2020);
  
  Serial.println("Date; Time; Soil Moisture; Temperature; Humidity");   
  
  time_t date = now();
  fileName = String(day(date)) + String(month(date))+ String(year(date));
  
  sdCardFile = SD.open(fileName, FILE_WRITE);
  if (sdCardFile) { 
    sdCardFile.println("Date; Time; Soil Moisture; Temperature; Humidity");
    sdCardFile.close();
  }
  else {
    Serial.println("Error while openning a file: " + fileName);
  }
}

void loop() {
  time_t t = now();

  // soil moisture sensor
  int sensorAnalog;
  sensorAnalog = analogRead(soilMoistureSensorPin);
  soilMoisture = ( 100 - ( (sensorAnalog/1023.00) * 100 ) );

  // reading data from dht
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  // rename file if date changes
  fileName = String(day(t)) + String(month(t))+ String(year(t));
  
  Serial.print(String(day(t)) + "/" + String(month(t)) + "/" + String(year(t)));
  Serial.print("; ");
  Serial.print(String(hour(t)) + ":" + String(minute(t)) + ":" + String(second(t)));
  Serial.print("; ");
  Serial.print(soilMoisture);
  Serial.print("; ");    
  Serial.print(temperature);
  Serial.print("; ");
  Serial.println(humidity);

  // writing data to sd card
  sdCardFile = SD.open(fileName, FILE_WRITE);
  if (sdCardFile) {    
    sdCardFile.print(String(day(t)) + "/" + String(month(t)) + "/" + String(year(t)));
    sdCardFile.print("; ");
    sdCardFile.print(String(hour(t)) + ":" + String(minute(t)) + ":" + String(second(t)));
    sdCardFile.print("; ");
    sdCardFile.print(soilMoisture);
    sdCardFile.print("; ");
    sdCardFile.print(temperature);
    sdCardFile.print("; ");
    sdCardFile.println(humidity);
    sdCardFile.close(); 
  }
  else {
    Serial.println("Error while openning a file: " + fileName);
  }
  delay(3000);
}
