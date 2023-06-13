#include "GPS.h"

void setup() {
  // put your setup code here, to run once:
//  Serial.begin(57600);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }
//  latitude = 0;
//  longitude = 0;
}

void loop() {
  Serial.print("LAT="); Serial.println(get_long(), 6);
  Serial.print("LONG="); Serial.println(get_lati(), 6);
  Serial.print("Alti="); Serial.println(get_alt(), 6);
}
