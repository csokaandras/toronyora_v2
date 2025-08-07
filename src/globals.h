#ifndef GLOBALS_H
#define GLOBALS_H

#include <TimeLib.h>
#include "sensor.h"
#include <TinyGPS.h> // http://arduiniana.org/libraries/TinyGPS/
#include <SoftwareSerial.h>
#include <EEPROM.h>


extern time_t savedTime;
extern time_t showedTime;
extern time_t currentTime;
extern time_t gpsTime;

extern int manual_rotate_count;
extern bool manual_rotate;
extern bool monitorTime;
extern bool monitorSensor;

extern int rotation_counter;

extern sensor s0;
extern sensor s1;
extern sensor s2;
extern sensor s3;
extern sensor sMin;
extern sensor sHour;

extern int eeAdress;
extern int errEeAdr;

extern int diffInMin;
extern int offset; // Central European Time
extern TinyGPS gps;
extern SoftwareSerial SerialGPS; // receive on pin 10

extern bool turning;

extern int errCode;

#endif
