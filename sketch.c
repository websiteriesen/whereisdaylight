// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
void setup () {
 while (!Serial); // for Leonardo/Micro/Zero
 Serial.begin(57600);
 if (! rtc.begin()) {
   Serial.println("Couldn't find RTC");
   while (1);
 }
 if (! rtc.isrunning()) {
   Serial.println("RTC is NOT running!");
   // following line sets the RTC to the date & time this sketch was compiled
   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
   // This line sets the RTC with an explicit date & time, for example to set
   // January 21, 2014 at 3am you would call:
   // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
 }
}
void loop () {
 DateTime now = rtc.now();
 DateTime currentYearFirstDay = DateTime(2022, 1, 1, 0, 0, 0);
 int dayOfYear = divideAndRoundUp(now.unixtime() - currentYearFirstDay.unixtime(), 86400L);
 Serial.print("Sunrise is today at: ");
 Serial.println(getSunriseByDayOfYear(dayOfYear), DEC);
 Serial.println();
 Serial.println();
 delay(3000);
}

int divideAndRoundUp(float x, float y)
{
   int a = (x -1)/y +1;

   return a;
}

float getSunriseByDayOfYear(int dayOfYear) {
  if(dayOfYear > 10) {
    return 8.00;
  }
  return 10.00;
}












 /*Serial.print(now.year(), DEC);
 Serial.print('/');
 Serial.print(now.month(), DEC);
 Serial.print('/');
 Serial.print(now.day(), DEC);
 Serial.print(" (");
 Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
 Serial.print(") ");
 Serial.print(now.hour(), DEC);
 Serial.print(':');
 Serial.print(now.minute(), DEC);
 Serial.print(':');
 Serial.print(now.second(), DEC);
 Serial.println();
 Serial.print("Date of first this year: ");
 Serial.print(currentYearFirstDay.year(), DEC);
 Serial.print("Date of  first this year is valid:");
 if(currentYearFirstDay.isValid()) {
 Serial.print("YES");
 } else {
   Serial.print("NO");
 }
 Serial.print(" days this year: ");
 Serial.print(divideAndRoundUp(now.unixtime() - currentYearFirstDay.unixtime(), 86400L));
 Serial.println("d");
 // days since first day of year
 Serial.println(""); */
