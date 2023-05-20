#include <config.h>
#include <log.h>
#include <motion.h>

Log logger;
Motion motion;

void setup(void)
{
  // setup serial connection
  Serial.begin(115200);

  // setup log
  logger.begin();

  // setup motion sensor
  motion.begin();

  delay(100);
}

void loop()
{
  /* Get new sensor events with the readings */
  motion.getData();

  LogData data = {
    aX : motion.a.acceleration.x,
    aY : motion.a.acceleration.y,
    aZ : motion.a.acceleration.z,
    gX : motion.g.gyro.y,
    gY : motion.g.gyro.y,
    gZ : motion.g.gyro.z,
    temp : motion.temp.temperature
  };

  logger.write(&data);

  delay(1000 / LOGS_PER_SECOND);
}