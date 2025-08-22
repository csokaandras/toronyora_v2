#include <Arduino.h>
#include <TinyGPS.h> // http://arduiniana.org/libraries/TinyGPS/

#include "monitor/monitor.h"
#include "time/time.h"
#include "serial/serial.h"
#include "globals.h"

static const int RXPin = D0, TXPin = D1;

SoftwareSerial SerialGPS = SoftwareSerial(RXPin, TXPin); // receive on pin 10
int offset = 1;
TinyGPS gps;

#define HALL_0 A0
#define HALL_1 A1
#define HALL_2 A2
#define HALL_3 A3
#define HALL_MIN A4
#define HALL_HOUR A5

int man_rotate = PIN_D4;
int man_back = PIN_D3;

int motor_rotate = PIN_D8;
int motor_back = PIN_D7;

// --------------------------------- IMPORTANT ---------------------------------
bool SET_TIME_START = false; // this need allways be false when last patch is uploaded to arduino
// --------------------------------- IMPORTANT ---------------------------------

const byte powerLossPin = PIN_D2;

time_t savedTime;
time_t showedTime;
time_t currentTime = now();
time_t gpsTime;

time_t ido;
int prevSec;

int manual_rotate_count = 0;
int diffInMin = 0;
int prevdiff = 0;
int currentmin = 0;
int currentsec = 0; // only for monitoring
int rotation_counter = 0;

bool forward;
bool backward;

bool manual_rotate = false;
bool monitorTime = false;
bool monitorSensor = false;
bool turning = false;

sensor s0 = {1, 10, 200, 700, 1000, 0};
sensor s1 = {2, 10, 200, 700, 1000, 0};
sensor s2 = {4, 10, 200, 700, 1000, 0};
sensor s3 = {8, 10, 200, 700, 1000, 0};
sensor sMin = {0, 0, 350, 700, 1000, 0};
sensor sHour = {0, 0, 200, 700, 1000, 0};

int eeAdress = 1;
int errEeAdr = 9;

int errCode = 0;

int man_torate_state = 0;
int man_back_state = 0;

void setup()
{
  SerialGPS.begin(4800);
  Serial.begin(9600);

  pinMode(HALL_0, INPUT);
  pinMode(HALL_1, INPUT);
  pinMode(HALL_2, INPUT);
  pinMode(HALL_3, INPUT);
  pinMode(HALL_MIN, INPUT);

  pinMode(motor_rotate, OUTPUT);
  pinMode(motor_back, OUTPUT);

  pinMode(man_rotate, INPUT_PULLUP);
  pinMode(man_back, INPUT_PULLUP);

  // get the time from EEPROM
  time_t fromEEPROMTime;
  EEPROM.get(eeAdress, fromEEPROMTime);
  savedTime = fromEEPROMTime;
  showedTime = fromEEPROMTime;

  int fromEEPROMError;
  EEPROM.get(errEeAdr, fromEEPROMError);
  errCode = fromEEPROMError;

  // save time to EEPROM
  pinMode(powerLossPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(powerLossPin), saveTime, RISING);

  isSummerTime(now());
  getGpsTime();

  syncGpsToRtc();

  diffInMin = calculateDifference(currentTime, savedTime);

  if (SET_TIME_START)
  {
    setTime(12, 15, 17, 16, 7, 2025); // pl. 2025.07.16. 12:15:17
    offset = 2;
    SET_TIME_START = false;
  }

  forward = false;
  backward = false;
  digitalWrite(motor_back, LOW);
  digitalWrite(motor_rotate, LOW);
}

void loop()
{
  currentTime = now();
  int sensor_hour = 0;

  s0.value = analogRead(HALL_0);
  s1.value = analogRead(HALL_1);
  s2.value = analogRead(HALL_2);
  s3.value = analogRead(HALL_3);
  sMin.value = analogRead(HALL_MIN);
  sHour.value = analogRead(HALL_HOUR);

  int prev_state = sMin.state;
  checkSensor(&sMin);

  man_torate_state = digitalRead(man_rotate);
  man_back_state = digitalRead(man_back);

  if (man_torate_state == LOW)
  {
    manual_rotate = true;

    forward = true;
    backward = false;
  }
  else if (man_back_state == LOW)
  {
    manual_rotate = true;

    forward = true;
    backward = true;
  }
  else if (manual_rotate)
  {

    if (sMin.state == 0 && prev_state == 1)
    {
      forward = false;
      backward = false;

      manual_rotate = false;
    }
  }

  if (year() < 2024)
  {
    isSummerTime(now());
    getGpsTime();
    syncGpsToRtc();
  }
  else if (!manual_rotate && errCode == 0)
  {
    isSummerTime(now());
    getGpsTime();
    syncGpsToRtc();

    sensor_hour = calculateHour(&s0, &s1, &s2, &s3);
    
    if (currentmin == 0 && (sensor_hour != hour() || sensor_hour != hour() - 12) && sHour.state == 1)
    {
      EEPROM.put(errEeAdr, 1);
    }

    if (sMin.state == 0 && prev_state == 1)
    {
      onMin();
      diffInMin = calculateDifference(currentTime, showedTime);
    }

    // rotation direction
    if (diffInMin > 0)
    {
      // a 13-ason kiadjuk hogy HIGH
      // várunk picit
      // a 12-esen kiadjuk hogy LOW
      // forgatja előre amíg nem lesz 0 a diffInMin
      rotation_counter++;
      forward = true;
      backward = false;
    }
    else if (diffInMin < 0)
    {
      // a 12-esen kiadjuk hogy HIGH
      // várunk picit
      // a 13-ason kiadjuk hogy HIGH
      // forgatja hátra (meghúzza a relét és ezzel visszafelé forgatja) amíg nem lesz 0 a diffInMin
      rotation_counter++;
      forward = true;
      backward = true;
    }
    else
    {
      forward = false;
      backward = false;
    }

    diffInMin = calculateDifference(currentTime, showedTime);
  }

  // monitoring

  if (currentsec != second() && monitorTime)
  {
    currentsec = second();

    printTimes();

    Serial.print(diffInMin);
    Serial.println();
  }

  if (currentsec != second() && monitorSensor)
  {
    currentsec = second();

    logSensor("s0", &s0);
    logSensor("s1", &s1);
    logSensor("s2", &s2);
    logSensor("s3", &s3);

    checkSensor(&sMin);
    logSensor("min", &sMin);

    checkSensor(&sHour);
    logSensor("hour", &sHour);

    Serial.print("\nÉrzékelők álltal észlelt idő: ");
    Serial.print(sensor_hour);
    Serial.println();
  }

  digitalWrite(motor_rotate, forward ? HIGH : LOW);
  digitalWrite(motor_back, backward ? HIGH : LOW);
      
  if (currentmin != minute())
  {
    currentmin = minute();
  }
  

  handleSerialCommand();
}