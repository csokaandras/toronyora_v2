#include <TimeLib.h>

// kiszamolja a ketto megadott ido kozott a kulonbseget
int calculateDifference(time_t from, time_t to);    

// valtoztatja az idot a forgasiranynak megfeleloen
void changeTime(time_t time, bool forward);

// nezi hogy megtett-e egy fordulatot a percmutato (egy motorfordulat)
void onMin();

// atkonvertalja az idot es elmenti az EEPROM-ba
void saveTime();

// Kiolvassa a GPS idot
void getGpsTime();

// bemasolja a GPS idot az RTC idobe ha tul nagy az elteres
void syncGpsToRtc();

// megnezi hogy nyari vagy teli idoszamitas es modositja az offsetet
void isSummerTime(time_t t);

// ez a felso fuggveny egy reszet csinalta volna de megse kellett
void SumWinTimeChange();