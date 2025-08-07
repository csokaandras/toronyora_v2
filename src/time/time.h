#include <TimeLib.h>

int calculateDifference(time_t from, time_t to);

void changeTime(time_t time, bool forward);

void onMin();

void saveTime();

void getGpsTime();

void syncGpsToRtc();

void isSummerTime(time_t t);

void SumWinTimeChange();