// Basic demo for accelerometer readings from Adafruit MPU6050

// ESP32 Guide: https://RandomNerdTutorials.com/esp32-mpu-6050-accelerometer-gyroscope-arduino/
// ESP8266 Guide: https://RandomNerdTutorials.com/esp8266-nodemcu-mpu-6050-accelerometer-gyroscope-arduino/
// Arduino Guide: https://RandomNerdTutorials.com/arduino-mpu-6050-accelerometer-gyroscope/

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#define SD_CS 15
#define SD_SCK 14
#define SD_MOSI 13
#define SD_MISO 12
#define LOGS_PER_SECOND 50
#define LOG_DELIMETER ","

SPIClass sd_spi(HSPI);
Adafruit_MPU6050 mpu;

struct LogData {
  float aX, aY, aZ, gX, gY, gZ, temp;

  String toString() {
    String o = ""; // empty string
    o.concat(aX);
    o += LOG_DELIMETER;
    o.concat(aY);
    o += LOG_DELIMETER;
    o.concat(aZ);
    o += LOG_DELIMETER;
    o.concat(gX);
    o += LOG_DELIMETER;
    o.concat(gY);
    o += LOG_DELIMETER;
    o.concat(gZ);
    o += LOG_DELIMETER;
    o.concat(temp);

    return o;
  }
};

void appendFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file){
      Serial.println("Failed to open file for appending");
      return;
  }
  if(file.println(message)){
      Serial.println("Message appended");
  } else {
      Serial.println("Append failed");
  }
  file.close();
};

void logToSD(LogData *data) {
  appendFile(SD, "/log.txt", data->toString().c_str());
};



void setup(void) {
  // setup serial connection
  Serial.begin(115200);

  // setup SPI
  sd_spi.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);

  // setup SD Card
  if(!SD.begin(SD_CS, sd_spi)){
      Serial.println("Card Mount Failed");
      return;
  }

  // setup MPU-6050 Accelerometer/Gyro
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);

  delay(100);
}

void loop() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  LogData data = { 
    aX: a.acceleration.x,
    aY: a.acceleration.y,
    aZ: a.acceleration.z,
    gX: g.gyro.y,
    gY: g.gyro.y,
    gZ: g.gyro.z,
    temp: temp.temperature
  };

  logToSD(&data);
  

  delay(1000 / LOGS_PER_SECOND);
}