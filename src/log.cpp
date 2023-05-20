#include <config.h>
#include <log.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#define SD_CS 15
#define SD_SCK 14
#define SD_MOSI 13
#define SD_MISO 12
#define LOGS_PER_SECOND 50
#define LOG_DELIMETER ","
#define LOG_FILENAME "/log.txt"

Log::Log(uint8_t spi_bus) : sd_spi(spi_bus){};

void Log::begin()
{
  // setup SPI
  sd_spi.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  // setup SD Card
  if (!SD.begin(SD_CS, sd_spi))
  {
    Serial.println("Card Mount Failed");
    return;
  }
}
void Log::appendFile(fs::FS &fs, const char *path, const char *message)
{
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file)
  {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.println(message))
  {
    Serial.println("Message appended");
  }
  else
  {
    Serial.println("Append failed");
  }
  file.close();
};

void Log::write(LogData *data)
{
  String o = ""; // empty string
  o.concat(data->aX);
  o += LOG_DELIMETER;
  o.concat(data->aY);
  o += LOG_DELIMETER;
  o.concat(data->aZ);
  o += LOG_DELIMETER;
  o.concat(data->gX);
  o += LOG_DELIMETER;
  o.concat(data->gY);
  o += LOG_DELIMETER;
  o.concat(data->gZ);
  o += LOG_DELIMETER;
  o.concat(data->temp);

  Serial.println(o);
  // appendFile(SD, LOG_FILENAME, o.c_str());
};
