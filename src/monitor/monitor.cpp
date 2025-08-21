#include <Arduino.h>
#include <TimeLib.h>

#include "monitor.h"


void checkSensor(sensor *s)
{
  if ((s->value >= s->LL && s->value < s->LH) || (s->value >= s->HL && s->value < s->HH))
  {
    s->state = 1;
  }
  else if (s->value < s->HL && s->value >= s->LH)
  {
    s->state = 0;
  }
  else
  {
    s->state = -1;
  }
}

int calculateHour(sensor *s0, sensor *s1, sensor *s2, sensor *s3)
{
  int hour = 0;

  checkSensor(s0);
  checkSensor(s1);
  checkSensor(s2);
  checkSensor(s3);

  if (s0->state == 1)
  {
    hour += 1;
  }
  if (s1->state == 1)
  {
    hour += 2;
  }
  if (s2->state == 1)
  {
    hour += 4;
  }
  if (s3->state == 1)
  {
    hour += 8;
  }
  // Serial.println(hour);
  return hour;
}

void logSensor(String text, sensor *s)
{
  if (second() % 1 == 0)
  {
    
    Serial.print(text + " ");
    Serial.print(s->value);
    Serial.print(" - ");
    Serial.print(s->state);
    Serial.println();
  }
}

void printDigits(int digits)
{
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void showTime(time_t t, String text)
{
  Serial.print(text);

  Serial.print(year(t));
  Serial.print("-");
  printDigits(month(t));
  Serial.print("-");
  printDigits(day(t));
  Serial.print(" ");
  printDigits(hour(t));
  Serial.print(":");
  printDigits(minute(t));
  Serial.print(":");
  printDigits(second(t));
  Serial.println();
}

void printTimes()
{
  Serial.println();
  Serial.print("saved   ");
  showTime(savedTime, "Mentett idő:        ");
  Serial.print("showed  ");
  showTime(showedTime, "Mechanika akt. idő: ");
  Serial.print("current ");
  showTime(currentTime, "A pontos idő:       ");
  Serial.print("gps     ");
  showTime(gpsTime, "A gps idő:          ");
}

void PrintVT100()
{
  Serial.println();
  Serial.println("          ▞                                                                                                                   ▞                ");
  Serial.println("┏━━━┓  ┏━━━━┓ ━━┳━━ ┏┓  ┏┓ ┏━━━━┓ ┏┓   ┰ ┏━━━━┓ ┏━━━━┓ ━━┳━━ ┏━━━━┓ ┏━━━━┓        ━━┳━━ ┏━━━━┓ ┏━━━━┓ ┏━━━━┓ ┏┓   ┰  ┓   ┏ ┏━━━━┓ ┏━━━━┓ ┏━━━━┓");
  Serial.println("┃   ┃  ┃    ┃   ┃   ┃┗┓┏┛┃ ┃    ┃ ┃┗┓  ┃ ┃    ┃ ┗┓       ┃   ┃    ┃ ┃    ┃          ┃   ┃    ┃ ┃    ┃ ┃    ┃ ┃┗┓  ┃  ┗┓ ┏┛ ┃    ┃ ┃    ┃ ┃    ┃");
  Serial.println("┣━━━┻┓ ┣━━━━┫   ┃   ┃ ┗┛ ┃ ┃    ┃ ┃ ┗┓ ┃ ┃    ┃  ┗━━┓    ┃   ┃    ┃ ┣━━━┳┛          ┃   ┃    ┃ ┣━━━┳┛ ┃    ┃ ┃ ┗┓ ┃   ┗┳┛  ┃    ┃ ┣━━━┳┛ ┣━━━━┫");
  Serial.println("┃    ┃ ┃    ┃   ┃   ┃    ┃ ┃    ┃ ┃  ┗┓┃ ┃    ┃     ┗┓   ┃   ┃    ┃ ┃   ┗┓          ┃   ┃    ┃ ┃   ┗┓ ┃    ┃ ┃  ┗┓┃    ┃   ┃    ┃ ┃   ┗┓ ┃    ┃");
  Serial.println("┗━━━━┛ ┚    ┖   ┸   ┸    ┸ ┗━━━━┛ ┸   ┗┛ ┗━━━━┛ ┗━━━━┛   ┸   ┗━━━━┛ ┸    ┸          ┸   ┗━━━━┛ ┸    ┸ ┗━━━━┛ ┸   ┗┛    ┸   ┗━━━━┛ ┸    ┸ ┚    ┖");
  Serial.println("☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲☴☱☲");
  Serial.println("                                                              Developed By: Csóka András");
  Serial.println("                                                               Designed By: Csóka Antal");
  Serial.println();
  printTimes();
  int hour = calculateHour(&s0, &s1, &s2, &s3);
  Serial.print("A mechanika órája: ");
  Serial.println(hour);
}
