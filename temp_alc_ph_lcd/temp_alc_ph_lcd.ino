#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// pH sensor
#define phPin A0
// unsigned long int avgVal;
// float b;

#define turbPin A1
float turbV;

// lcd display
LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display


float GetPhValue()
{
  static int buf[10];

  for(int index = 0; index < 10; ++index)
  { 
    buf[index] = analogRead(phPin);
    delay(10);
  }

  for(int i = 0; i < 9; ++i)
  {
    for(int j = i + 1; j < 10; ++j)
    {
      if(buf[i] > buf[j])
      {
        int temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }

  unsigned long int avgVal = 0;
  for(int i = 2; i < 8; ++i)
  {
    avgVal += buf[i];
  }
    
  float phValue = (float)avgVal * 3.3/1024/6;

  return phValue;  
}

float GetTurbValue()
{
  static float turbV = 0.0;
  for (int i = 0; i < 800; i++) {
    turbV += ((float)analogRead(turbPin)/1023)*3.3;
  }
  turbV = turbV/800; 

  return turbV;
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.init(); 
  lcd.backlight();
}

void loop()
{

  // pH sensor data and calibration
  static float phValue = GetPhValue();
  static float turbValue = GetTurbValue();

  Serial.print("\tpH_Volt:\t");
  Serial.print(phValue);
  Serial.print("\tturbV:\t");
  Serial.print(turbValue);
  Serial.println();

  

  lcd.setCursor(0,0);
  lcd.print("pH: ");
  lcd.print(phValue);
  lcd.setCursor(0,1);
  lcd.print("turb: ");
  lcd.print(turbValue);

  delay(10000);
  lcd.clear();
}
