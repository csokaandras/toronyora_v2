#ifndef GLOBALS_H
#define GLOBALS_H

#include <TimeLib.h>
#include "sensor.h"
#include <TinyGPS.h> // http://arduiniana.org/libraries/TinyGPS/
#include <SoftwareSerial.h>
#include <EEPROM.h>


extern time_t savedTime;    // EEPROM-ban elmentett ido
extern time_t showedTime;   // mechanika alltal mutatott ido
extern time_t currentTime;  // pontos ido
extern time_t gpsTime;      // GPS-bol jovo ido

extern int manual_rotate_count; // hanyat kell kulso beavatkozasra (manualisan) fordulnia
extern bool manual_rotate;      // kell-e manualisan fordulnia
extern bool monitorTime;        // ki kell-e irni az ido monitorozast
extern bool monitorSensor;      // ki kell-e irni az senzor monitorozast

extern int rotation_counter;    // hanyat kell meg fordulnia

// Szenzoroktol bejovo adat
extern sensor s0;       // ora 1-es
extern sensor s1;       // ora 2-es
extern sensor s2;       // ora 4-es
extern sensor s3;       // ora 8-as
extern sensor sMin;     // perc erzekelo (fordult-e egy percet)
extern sensor sHour;    // amikor oran all akkor jon tole jel

extern int eeAdress;    // ido elmentese szolgalo EEPROM cim
extern int errEeAdr;    // error kod elmentesere szolgalo EEPROM cim

extern int diffInMin;   // az idokulonbseg percben
extern int offset;      // mekkora az elteres a Grinichi idotol (Central Europe Time)
extern TinyGPS gps;     // GPS-tol erkezo jel
extern SoftwareSerial SerialGPS; // soros komunikacio receive on pin 10

extern bool turning;    // jelenleg forog-e 

extern int errCode;     // kiolvasott errorkod

#endif
