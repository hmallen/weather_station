/*
   Weather Station
   - MS5607 Barometer
   - DHT11 Temperature/Humidity
   - Photoresistor

   TO DO:
   - Add user input of start time
   - Calculate and log actual time based on user input
*/

#include <DHT.h>
#include <SdFat.h>
#include <TimeLib.h>
#include <Wire.h>
#include "IntersemaBaro_MODIFIED.h"

#define chipSelect 10
#define DHT11Pin 2
//#define lightInput A0

SdFat sd;
SdFile logFile;

DHT dht(DHT11Pin, DHT11);

Intersema::BaroPressure_MS5607B baro(true);

const bool requestDateTime = true;
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

  //writeSD("Light, Temperature(F), Humidity(%RH), Barometric Pressure(Pa)");
  writeSD("Temperature(F), Humidity(%RH), Barometric Pressure(Pa)");

  for (int x = 0; x < 5; x++) {
    float tempF = dht.readTemperature(true);
    delay(400);
    float humidity = dht.readHumidity();
    delay(400);
    unsigned long pressPa = baro.getPressurePascals();
    delay(100);
  }

  if (requestDateTime == true) inputDateTime();
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

  String dataString;

  if (requestDateTime == true) {
    String minuteFormatted = formatDigits(minute());
    String secondFormatted = formatDigits(second());

    String dateString = String(month()) + "/" + String(day()) + "/" + String(year());
    String timeString = String(hour()) + ":" + minuteFormatted + ":" + secondFormatted;

    //String dataString = dateString + ", " + timeString + ", " + String(lightVal) + ", " + String(tempF) + ", " + String(humidity) + ", " + String(pressPa);
    dataString = dateString + ", " + timeString + ", " + String(tempF) + ", " + String(humidity) + ", " + String(pressPa);
  }
  else {
    unsigned long elapsedTime = millis();
    //String dataString = String(elapsedTime) + ", " + String(lightVal) + ", " + String(tempF) + ", " + String(humidity) + ", " + String(pressPa);
    dataString = String(elapsedTime) + ", " + String(tempF) + ", " + String(humidity) + ", " + String(pressPa);
    Serial.println(dataString);
  }

  if (dataLogging == true) {
    Serial.print("Writing to SD card...");
    writeSD(dataString);
    Serial.println("complete.");
  }

  delay(dataInterval);
}

void writeSD(String dataString) {
  if (!logFile.open("data_log.txt", O_RDWR | O_CREAT | O_AT_END)) sd.errorHalt("Failed to open log file.");

  logFile.println(dataString);
  logFile.close();
}

void inputDateTime() {
  String monthInput, dayInput, yearInput, hourInput, minuteInput, secondInput;

  Serial.println("Please input date.");
  Serial.println();
  Serial.print("Month: ");

  while (!Serial.available()) {
    delay(10);
  }

  while (Serial.available()) {
    char c = Serial.read();
    if (c != '\n' || c != '\r') monthInput += c;
    delay(10);
  }
  int monthInt = monthInput.toInt();

  Serial.println(monthInt);
  Serial.print("Day: ");

  while (!Serial.available()) {
    delay(10);
  }

  while (Serial.available()) {
    char c = Serial.read();
    if (c != '\n' || c != '\r') dayInput += c;
    delay(10);
  }
  int dayInt = dayInput.toInt();

  Serial.println(dayInt);
  Serial.print("Year: ");

  while (!Serial.available()) {
    delay(10);
  }

  while (Serial.available()) {
    char c = Serial.read();
    if (c != '\n' || c != '\r') yearInput += c;
    delay(10);
  }
  int yearInt = yearInput.toInt();

  /*Serial.println(yearInt);
  Serial.println();
  String dateInput = String(monthInt) + "/" + String(dayInt) + "/" + String(yearInt);
  Serial.println(dateInput);
  Serial.println();
  Serial.println("Please input time in 24-hour format.");
  Serial.println();
  Serial.print("Hour: ");*/

  while (!Serial.available()) {
    delay(10);
  }

  while (Serial.available()) {
    char c = Serial.read();
    if (c != '\n' || c != '\r') hourInput += c;
    delay(10);
  }
  int hourInt = hourInput.toInt();

  Serial.println(hourInt);
  Serial.print("Minute: ");

  while (!Serial.available()) {
    delay(10);
  }

  while (Serial.available()) {
    char c = Serial.read();
    if (c != '\n' || c != '\r') minuteInput += c;
    delay(10);
  }
  int minuteInt = minuteInput.toInt();

  Serial.println(minuteInt);
  Serial.print("Second: ");

  while (!Serial.available()) {
    delay(10);
  }

  while (Serial.available()) {
    char c = Serial.read();
    if (c != '\n' || c != '\r') secondInput += c;
    delay(10);
  }
  int secondInt = secondInput.toInt();

  /*Serial.println(secondInt);
  Serial.println();
  String minuteFormatted = formatDigits(minuteInt);
  String secondFormatted = formatDigits(secondInt);
  String timeInput = String(hourInt) + ":" + minuteFormatted + ":" + secondFormatted;
  Serial.println(timeInput);
  Serial.println();*/

  setTime(hourInt, minuteInt, secondInt, dayInt, monthInt, yearInt);

  //time_t timeNow = now();
  //Serial.println(timeNow);
}

String formatDigits(int digits) {
  String digitsFormatted;

  if (digits < 10) digitsFormatted = '0' + String(digits);
  else digitsFormatted = String(digits);

  return digitsFormatted;
}
