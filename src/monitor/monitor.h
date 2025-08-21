#include <Arduino.h>

#include "../sensor.h"
#include "globals.h"

void checkSensor(sensor *s);

int calculateHour(sensor *s0, sensor *s1, sensor *s2, sensor *s3);

void logSensor(String text, sensor *s);

void printDigits(int digits);

void showTime(time_t t, String text);

void printTimes();

void PrintVT100();
