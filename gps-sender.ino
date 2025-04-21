#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>

// GPS Module RX/TX Pins
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// TinyGPS++ Object
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

String gpsData = "No Data";
#define I2C_ADDRESS 8  // I2C Slave Address

void setup() {
  Serial.begin(9600);
  ss.begin(GPSBaud);
  Wire.begin(I2C_ADDRESS);  // Initialize as I2C Slave
  Wire.onRequest(requestEvent); // Register the request handler

  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
  Serial.print(F("Testing TinyGPS++ library v. "));
  Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();
}

void loop() {
  // Process GPS Data
  while (ss.available() > 0) {
    if (gps.encode(ss.read())) {
      // Format GPS Data
    
        gpsData = String(gps.location.lat(), 8) + "," +
                  String(gps.location.lng(), 8) + "," +
                  (gps.time.hour() < 10 ? "0" : "") + String(gps.time.hour()) + ":" +
                  (gps.time.minute() < 10 ? "0" : "") + String(gps.time.minute()) +
                  (gps.time.second() < 10 ? "0" : "") + String(gps.time.second());
                   
      
    }
  }
  Serial.println(gpsData);
  // Display GPS Data on Serial Monitor
  //displayInfo();

  // Error Check for GPS Module
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("No GPS detected: check wiring."));
    while (true); // Halt execution
  }
}

// Function to Send GPS Data When Master Requests
void requestEvent() {
  Wire.write(gpsData.c_str(), min(32, gpsData.length()));  // Send up to 32 bytes
}

// Function to Display GPS Data on Serial Monitor
void displayInfo() {
  Serial.print(F("Location: "));
  if (gps.location.isValid()) {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  } else {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" Date/Time: "));
  if (gps.date.isValid()) {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  } else {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid()) {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
  } else {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}
