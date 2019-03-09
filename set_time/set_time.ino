
#include <Wire.h>
#include "ds3231.h"

#define BUFF_MAX 128

uint8_t time[8];
char recv[BUFF_MAX];

void setup() {
  Serial.begin(9600);
  Wire.begin();
  DS3231_init(DS3231_CONTROL_INTCN);
  memset(recv, 0, BUFF_MAX);
  /* Modify the string below for the current time.
     The format is TssmmhhWDDMMYYYY
     (the letter T and then seconds, minutes, hours,
     weekday 1-7 starting with Sunday, day, month, year). */
  String currentTime = "T004618305032019";
  char buff[BUFF_MAX];
  currentTime.toCharArray(buff, BUFF_MAX);
  set_time(buff);
}


void loop() {
  char in;
  char buff[BUFF_MAX];
  unsigned long now = millis();
  struct ts t;
  DS3231_get(&t);
  snprintf(buff, BUFF_MAX, "%d.%02d.%02d %02d:%02d:%02d", t.year,
           t.mon, t.mday, t.hour, t.min, t.sec);
  Serial.println(buff);
  delay(1000);
}

void set_time(char *time) {
  uint8_t i;
  uint8_t reg_val;
  char buff[BUFF_MAX];
  struct ts t;

  //T355720619112011
  t.sec = inp2toi(time, 1);
  t.min = inp2toi(time, 3);
  t.hour = inp2toi(time, 5);
  t.wday = time[7] - 48;
  t.mday = inp2toi(time, 8);
  t.mon = inp2toi(time, 10);
  t.year = inp2toi(time, 12) * 100 + inp2toi(time, 14);
  DS3231_set(t);
  Serial.println("OK");
}
