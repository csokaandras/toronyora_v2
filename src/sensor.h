#ifndef SENSOR_H
#define SENSOR_H

struct sensor
{
  int name; // thats the value what we use to calculate hour
  int LL;   // Lower Low limit
  int LH;   // Lower High limit
  int HL;   // Higher High limit
  int HH;   // Higher Low limit
  int value;
  int state;
};

#endif // SENSOR_H
