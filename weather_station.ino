/*
 * Weather Station
 * - MS5607 Barometer
 * - DHT11 Temperature/Humidity
 * - Photoresistor
 */

#include <DHT.h>
#include <SdFat.h>
#include <Wire.h>
#include "IntersemaBaro_MODIFIED.h"

#define chipSelect 4
#define DHT11Pin 2
//#define lightInput A0

SdFat sd;
SdFile logFile;

DHT dht(DHT11Pin, DHT11);

Intersema::BaroPressure_MS5607B baro(true);

const bool dataLogging = true;

const int dataIntervalSec = 5; // Interval in seconds between successive data read/write events
const unsigned long dataInterval = dataIntervalSec * 1000;

void setup() {
  Serial.begin(57600);

  Serial.print("Initiating SD card...");
  if (!sd.begin(chipSelect, SPI_HALF_SPEED)) {
    Serial.println("failed.");
    sd.initErrorHalt();
  }
  Serial.println("complete.");

  Serial.print("Initiating barometer...");
  baro.init();
  Serial.println("complete.");
}

void loop() {
  //int lightVal = analogRead(lightInput);
  //delay(100);
  float tempF = dht.readTemperature(true);
  delay(400);
  float humidity = dht.readHumidity();
  delay(400);
  unsigned long pressPa = baro.getPressurePascals();
  delay(100);

  String dataString = String(lightVal) + ", " + String(tempF) + ", " + String(humidity) + ", " + String(pressPa);
  Serial.println(dataString);

  if (dataLogging == true) {
    Serial.print("Writing to SD card...");
    writeSD(dataString);
    Serial.println("complete.");
  }

  delay(dataInterval);
}

void writeSD(String dataString) {
  if (!logFile.open("data_log.txt", O_RDWR | O_CREAT | O_AT_END)) sd.errorHalt("Failed to open log file.");

  Serial.print("Writing data to log file...");
  logFile.println(dataString);
  Serial.println("complete.");
  logFile.close();
}
