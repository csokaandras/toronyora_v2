#include "../globals.h"
#include "time.h"

bool frissGps = false;

int calculateDifference(time_t from, time_t to)
{
  if (!turning || !manual_rotate)
  {
    int diff = ( from - to ) / 60;
    diff = diff % 720;
    /*
    if (diff > 360)
    {
      diff = ( diff - 360 );
    }
    if (diff < -360)
    {
      diff = ( diff + 360 );
    }
    */

    return diff;
  }

  return diffInMin;
}

void changeTime(time_t time, bool forward)
{
  if (forward)
  {
    showedTime += 60;
  }
  else
  {
    showedTime -= 60;
  }
}

void onMin()
{
  if (diffInMin > 0)
  {
    changeTime(showedTime, true);
  }
  else if (diffInMin < 0)
  {
    changeTime(showedTime, false);
  }
  Serial.print("Egy fordulat ideje: ");
  Serial.println(rotation_counter);
  rotation_counter = 0;
}

void saveTime()
{
  time_t saveTime = showedTime;
  EEPROM.put(eeAdress, saveTime);
}

void getGpsTime()
{
  frissGps = false;
  if (SerialGPS.available())
  {
    if (gps.encode(SerialGPS.read())) // process gps messages
    {
      // when TinyGPS reports new data...
      frissGps = true;
      unsigned long age;
      int Year;
      byte Month, Day, Hour, Minute, Second;
      gps.crack_datetime(&Year, &Month, &Day, &Hour, &Minute, &Second, NULL, &age);
      if (age < 500)
      {
        // set the Time to the latest GPS reading
        tmElements_t tm;

        tm.Second = Second;
        tm.Minute = Minute;
        tm.Hour = Hour + offset; // időzóna korrekció, ha szükséges
        tm.Day = Day;
        tm.Month = Month;
        tm.Year = CalendarYrToTm(Year); // konvertál 1970-alapú évvé

        gpsTime = makeTime(tm); // konvertál time_t típusúvá
      }
    }
  }
}

void syncGpsToRtc()
{
  if (frissGps && abs(gpsTime - currentTime) >= 30)
  {
    setTime(gpsTime);
  }
}

void isSummerTime(time_t t)
{
  int y = year(t);

  // Március utolsó vasárnapja 02:00
  tmElements_t marchLastSunday;
  marchLastSunday.Year = CalendarYrToTm(y);
  marchLastSunday.Month = 3;
  marchLastSunday.Day = 31;
  marchLastSunday.Hour = 2;
  marchLastSunday.Minute = 0;
  marchLastSunday.Second = 0;

  // Lépjünk visszafelé az utolsó vasárnapig
  for (int i = 0; i < 7; i++)
  {
    time_t temp = makeTime(marchLastSunday) - i * SECS_PER_DAY;
    if (weekday(temp) == 1)
    { // Sunday = 1
      marchLastSunday.Day -= i;
      break;
    }
  }

  time_t dstStart = makeTime(marchLastSunday);

  // Október utolsó vasárnapja 03:00
  tmElements_t octLastSunday;
  octLastSunday.Year = CalendarYrToTm(y);
  octLastSunday.Month = 10;
  octLastSunday.Day = 31;
  octLastSunday.Hour = 3;
  octLastSunday.Minute = 0;
  octLastSunday.Second = 0;

  for (int i = 0; i < 7; i++)
  {
    time_t temp = makeTime(octLastSunday) - i * SECS_PER_DAY;
    if (weekday(temp) == 1)
    { // Sunday = 1
      octLastSunday.Day -= i;
      break;
    }
  }

  time_t dstEnd = makeTime(octLastSunday);

  if (t >= dstStart && t < dstEnd)
  {
    offset = 2; // CEST time zone
  }else{
    offset = 1;
  }
}
