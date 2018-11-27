
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_IS31FL3731.h>
#include "ds3231.h"
#include <Time.h>
#include <TimeLib.h>

#define BUFF_MAX 128

uint8_t time[8];
char recv[BUFF_MAX];
unsigned int recv_size = 0;
unsigned long prev, interval = 5000;

// If you're using the full breakout...
//Adafruit_IS31FL3731 matrix = Adafruit_IS31FL3731();
// If you're using the FeatherWing version
Adafruit_IS31FL3731_Wing matrix = Adafruit_IS31FL3731_Wing();

time_t get_time_t(char *date_string)
{
  tmElements_t tm;
  char ds[20];

  strncpy(ds, date_string, 20);

  tm.Second = atoi(&ds[17]);
  tm.Minute = atoi(&ds[14]);
  tm.Hour = atoi(&ds[11]);
  tm.Day = atoi(&ds[8]);
  tm.Month = atoi(&ds[5]);
  tm.Year = atoi(&ds[0]) - 1970;
  return (makeTime(tm));
}

void setup() {
  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);
  Serial.begin(9600);
  Serial.println("ISSI manual animation test");
  if (! matrix.begin()) {
    Serial.println("IS31 not found");
    while (1);
  }
  Serial.println("IS31 Found!");
  matrix.setRotation(0);
  matrix.setTextSize(1);
  matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
  matrix.clear();
  matrix.setTextColor(100);

  Wire.begin();
  DS3231_init(DS3231_CONTROL_INTCN);
  memset(recv, 0, BUFF_MAX);
}


void loop() {
  char in;
  char buff[BUFF_MAX];
  unsigned long now = millis();
  struct ts t;
  DS3231_get(&t);
  snprintf(buff, BUFF_MAX, "%d.%02d.%02d %02d:%02d:%02d", t.year,
           t.mon, t.mday, t.hour, t.min, t.sec);
  time_t currentTime = get_time_t(buff);
  snprintf(buff, BUFF_MAX, "2017/12/25 00:00:01");
  time_t xmas = get_time_t(buff);
  Serial.println(currentTime);
  Serial.println(xmas);

  int daysLeft = ((xmas - currentTime) / 86400) + 1;
  Serial.println(daysLeft);

  matrix.setRotation(0);
  matrix.setTextSize(1);
  matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
  matrix.clear();
  matrix.setTextColor(100);
  snprintf(buff, BUFF_MAX, "%d days left!", daysLeft);
  
  for (int8_t x = 16; x >= -80; x--) {
    matrix.clear();
    matrix.setCursor(x, 0);
    matrix.print(buff);
    delay(50);
  }

  digitalWrite(6, LOW);
  delay(100);
}
