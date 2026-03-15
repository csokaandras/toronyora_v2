#include <Arduino.h>

#include "../sensor.h"
#include "globals.h"

// kiolvassa egy senzor mert erteket es ertelmezi
void checkSensor(sensor *s);

// kiszamolja az ora szenzorok alltal kapott adatok alapjan az orat
int calculateHour(sensor *s0, sensor *s1, sensor *s2, sensor *s3);

// vissaadja egy szenzor erteket es a szoveget (logolashoz)
void logSensor(String text, sensor *s);

// szep formatuman irja ki a 10-nel kissebb szamokat
void printDigits(int digits);

// vissaadja a megadott idot es a szoveget szepen kiiratva (logolashoz)
void showTime(time_t t, String text);

// az osszes ido kirasa
void printTimes();

// Terminalos kezelofelulet kiirasa
void PrintVT100();
