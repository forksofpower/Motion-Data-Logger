#ifndef LOG_H
#define LOG_H
#include "SPI.h"
#include <FS.h>

struct LogData
{
  float aX, aY, aZ, gX, gY, gZ, temp;
};

class Log
{
private:
  SPIClass sd_spi;
  void appendFile(fs::FS &fs, const char *path, const char *message);

public:
  Log(uint8_t spi_bus = HSPI);
  void begin();
  void write(LogData *data);
};

#endif /* LOG_H */