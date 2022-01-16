// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Digital IO pin connected to the button. This will be driven with a
// pull-up resistor so the switch pulls the pin to ground momentarily.
// On a high -> low transition the button press logic will execute.
#define BUTTON_PIN   2

#define PIXEL_PIN    6  // Digital IO pin connected to the NeoPixels.

int LED_NUMBER_VIENNA = 60;
int MAX_NUMBER_LED = 120;

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(MAX_NUMBER_LED, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

boolean oldState = HIGH;
int     mode     = 0;    // Currently-active animation mode, 0-9

RTC_DS1307 rtc;



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
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  strip.begin(); // Initialize NeoPixel strip object (REQUIRED)
  strip.show();  // Initialize all pixels to 'off'
}
void loop () {
  strip.clear();
 DateTime now = rtc.now();
 DateTime currentYearFirstDay = DateTime(now.year(), 1, 1, 0, 0, 0);
 int dayOfYear = divideAndRoundUp(now.unixtime() - currentYearFirstDay.unixtime(), 86400L);
 float sunset = getSunsetByDayOfYear(dayOfYear);
 float sunrise = getSunriseByDayOfYear(dayOfYear);
 bool hasRolledOver = false;
 int numberOfLightUpPixels = round(decimalmap(sunset - sunrise,0,24,0,MAX_NUMBER_LED));
 float currentHour = now.hour();
 float currentTimeInDecimalFormat = currentHour + (now.minute() / (float)60) - 1;
 int startPixelLitUp = LED_NUMBER_VIENNA + decimalmap(currentTimeInDecimalFormat - sunrise, 0, 24, 0, MAX_NUMBER_LED);
 int endPixelLitUp = startPixelLitUp - numberOfLightUpPixels;
 if(endPixelLitUp < 0) {
   endPixelLitUp = MAX_NUMBER_LED + endPixelLitUp;
   hasRolledOver = true;
 }
 if(startPixelLitUp > MAX_NUMBER_LED) {
   startPixelLitUp = startPixelLitUp - MAX_NUMBER_LED;
   hasRolledOver = true;
 }
 strip.setPixelColor(1, 255,255,255); // Set pixel 'c' to value 'color'
 Serial.print("Sunrise is today at: ");
 Serial.println(getSunriseByDayOfYear(dayOfYear), DEC);
 Serial.print("Light up pixels are: ");
 Serial.println(numberOfLightUpPixels, DEC);
 Serial.print("Time in decimal format: ");
 Serial.println(currentTimeInDecimalFormat, DEC);
 Serial.print("Start pixel is: ");
 Serial.println(startPixelLitUp, DEC);
 Serial.print("End pixel is: ");
 Serial.println(endPixelLitUp, DEC);
 Serial.println("Pixel start is left pixel, pixel end is right pixel!");
 Serial.print("Range that is lit up is: ");
    strip.clear();         //   Set all pixels in RAM to 0 (off)
 if(hasRolledOver) {
   Serial.print("0-");
   Serial.print(startPixelLitUp);
   Serial.print(" and ");
   Serial.print(endPixelLitUp);
   Serial.println("-150");

 } else {
   Serial.print(startPixelLitUp);
   Serial.print("-");
   Serial.println(endPixelLitUp);

 }
  for(int a=0; a<MAX_NUMBER_LED; a++) { 
    if(hasRolledOver) {
      if(a < startPixelLitUp || a > endPixelLitUp) {
        strip.setPixelColor(a, 255,255,255); // Set pixel 'c' to value 'color'
      }
    } else {
      if(a > endPixelLitUp && a < startPixelLitUp) {
        strip.setPixelColor(a, 255,255,255); // Set pixel 'c' to value 'color'
      }
    }

  }
   strip.show(); // Update strip with new contents
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
  if(dayOfYear <= 15) {
    return 6.7f;
  }
  if(dayOfYear <= 26) {
    return 6.583f;
  }
  if(dayOfYear <= 34) {
    return 6.333f;
  }
  if(dayOfYear <= 41) {
    return 6.167f;
  }
  if(dayOfYear <= 47) {
    return 6.00f;
  }
  if(dayOfYear <= 53) {
    return 5.833f;
  }
  if(dayOfYear <= 58) {
    return 5.666f;
  }
  if(dayOfYear <= 63) {
    return 5.500f;
  }
  if(dayOfYear <= 68) {
    return 5.333f;
  }
  if(dayOfYear <= 73) {
    return 5.167f;
  }
  if(dayOfYear <= 78) {
    return 5.000f;
  }
  if(dayOfYear <= 82) {
    return 4.833f;
  }
  if(dayOfYear <= 88) {
    return 4.666f;
  }
  if(dayOfYear <= 93) {
    return 4.500f;
  }
  if(dayOfYear <= 97) {
    return 4.333f;
  }
  if(dayOfYear <= 103) {
    return 4.167f;
  }
  if(dayOfYear <= 108) {
    return 4.000f;
  }
  if(dayOfYear <= 113) {
    return 3.833f;
  }
  if(dayOfYear <= 119) {
    return 3.666f;
  }
  if(dayOfYear <= 125) {
    return 3.500f;
  }
  if(dayOfYear <= 131) {
    return 3.333f;
  }
  if(dayOfYear <= 139) {
    return 3.167f;
  }
  if(dayOfYear <= 149) {
    return 3.000f;
  }
  if(dayOfYear <= 171) {
    return 2.883f;
  }
  if(dayOfYear <= 185) {
    return 3.000f;
  }
  if(dayOfYear <= 197) {
    return 3.167f;
  }
  if(dayOfYear <= 206) {
    return 3.333f;
  }
  if(dayOfYear <= 214) {
    return 3.500f;
  }
  if(dayOfYear <= 222) {
    return 3.666f;
  }
  if(dayOfYear <= 229) {
    return 3.833f;
  }
  if(dayOfYear <= 236) {
    return 4.000f;
  }
  if(dayOfYear <= 244) {
    return 4.167f;
  }
  if(dayOfYear <= 250) {
    return 4.333f;
  }
  if(dayOfYear <= 258) {
    return 4.500f;
  }
  if(dayOfYear <= 265) {
    return 4.666f;
  }
  if(dayOfYear <= 272) {
    return 4.833;
  }
  if(dayOfYear <= 279) {
    return 5.000f;
  }
  if(dayOfYear <= 286) {
    return 5.167f;
  }
  if(dayOfYear <= 293) {
    return 5.333f;
  }
  if(dayOfYear <= 299) {
    return 5.500f;
  }
  if(dayOfYear <= 306) {
    return 5.666f;
  }
  if(dayOfYear <= 312) {
    return 5.833f;
  }
  if(dayOfYear <= 319) {
    return 6.000f;
  }      
  if(dayOfYear <= 325) {
    return 6.167f;
  }  
  if(dayOfYear <= 333) {
    return 6.333f;
  }  
  if(dayOfYear <= 341) {
    return 6.500f;
  }  
  if(dayOfYear <= 352) {
    return 6.666f;
  }
  return 6.750f; 
}

float getSunsetByDayOfYear(int dayOfYear) {
  if(dayOfYear <= 15) {
    return 15.45f;
  }
  if(dayOfYear <= 26) {
    return 15.75f;
  }
  if(dayOfYear <= 34) {
    return 15.933f;
  }
  if(dayOfYear <= 41) {
    return 16.133f;
  }
  if(dayOfYear <= 47) {
    return 16.283f;
  }
  if(dayOfYear <= 53) {
    return 16.45f;
  }
  if(dayOfYear <= 58) {
    return 16.583f;
  }
  if(dayOfYear <= 63) {
    return 16.716f;
  }
  if(dayOfYear <= 68) {
    return 16.833f;
  }
  if(dayOfYear <= 73) {
    return 16.966f;
  }
  if(dayOfYear <= 78) {
    return 17.083f;
  }
  if(dayOfYear <= 82) {
    return 17.183f;
  }
  if(dayOfYear <= 88) {
    return 17.333f;
  }
  if(dayOfYear <= 93) {
    return 17.450f;
  }
  if(dayOfYear <= 97) {
    return 17.550f;
  }
  if(dayOfYear <= 103) {
    return 17.700f;
  }
  if(dayOfYear <= 108) {
    return 17.816f;
  }
  if(dayOfYear <= 113) {
    return 17.933f;
  }
  if(dayOfYear <= 119) {
    return 18.083f;
  }
  if(dayOfYear <= 125) {
    return 18.216f;
  }
  if(dayOfYear <= 131) {
    return 18.35f;
  }
  if(dayOfYear <= 139) {
    return 18.533f;
  }
  if(dayOfYear <= 149) {
    return 18.716f;
  }
  if(dayOfYear <= 171) {
    return 18.966f;
  }
  if(dayOfYear <= 185) {
    return 18.95f;
  }
  if(dayOfYear <= 197) {
    return 18.816f;
  }
  if(dayOfYear <= 206) {
    return 18.666f;
  }
  if(dayOfYear <= 214) {
    return 18.483f;
  }
  if(dayOfYear <= 222) {
    return 18.283f;
  }
  if(dayOfYear <= 229) {
    return 18.066f;
  }
  if(dayOfYear <= 236) {
    return 17.85f;
  }
  if(dayOfYear <= 244) {
    return 17.6f;
  }
  if(dayOfYear <= 250) {
    return 17.383f;
  }
  if(dayOfYear <= 258) {
    return 17.116f;
  }
  if(dayOfYear <= 265) {
    return 16.866;
  }
  if(dayOfYear <= 272) {
    return 16.633;
  }
  if(dayOfYear <= 279) {
    return 16.383f;
  }
  if(dayOfYear <= 286) {
    return 16.15f;
  }
  if(dayOfYear <= 293) {
    return 15.933f;
  }
  if(dayOfYear <= 299) {
    return 15.75f;
  }
  if(dayOfYear <= 306) {
    return 15.566f;
  }
  if(dayOfYear <= 312) {
    return 15.416;
  }
  if(dayOfYear <= 319) {
    return 15.266f;
  }      
  if(dayOfYear <= 325) {
    return 15.166f;
  }  
  if(dayOfYear <= 333) {
    return 15.05f;
  }  
  if(dayOfYear <= 341) {
    return 15.000f;
  }  
  if(dayOfYear <= 352) {
    return 15.016f;
  }
  return 15.15f; 
}

float decimalmap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
