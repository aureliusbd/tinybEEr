
#include "tinybEEr_TCS34725.h"

tinybEEr_TCS34725 tcs = tinybEEr_TCS34725(&Wire, TCS34725_ADDRESS,TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);

float r, b, g;

void setup() 
{
  Serial.begin(9600);
  Serial.println("\tColor View Test!\t");

  while(!tcs.begin())
  {
    Serial.println("\tNo TCS34725 found ... check your connections\t");
    delay(1000);
  }
}

void loop() {
  uint16_t clear, red, green, blue;
  tcs.getRGBC(&red, &green, &blue, &clear);
  tcs.lock();  
  Serial.print("C:\t"); Serial.print(clear);
  Serial.print("\tR:\t"); Serial.print(red);
  Serial.print("\tG:\t"); Serial.print(green);
  Serial.print("\tB:\t"); Serial.print(blue);
  // Figure out some basic hex code for visualization
  long rgb = 0;
  rgb = ((long)red <<16L) | ((long)green <<8L) | (long)blue;
  Serial.println("\tHex:\t" + String(rgb, HEX) + "\t");
  Serial.println();
  delay(2000);
}

