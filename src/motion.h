#ifndef MOTION_H
#define MOTION_H
#include <Adafruit_MPU6050.h>

class Motion
{
private:
    Adafruit_MPU6050 mpu;

public:
    sensors_event_t a, g, temp;
    void getData();
    void begin();
};

#endif /* MOTION_H */