#include <Arduino.h>
#include <math.h>
float ang = 0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  if(Serial.available())
  {
    if(Serial.read() == 'r')
    {
      Serial.println(5000.0*(1.0+sin(ang)),2);
      ang+=M_PI/100;
    }
  }
}
