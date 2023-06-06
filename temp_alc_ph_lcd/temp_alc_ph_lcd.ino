#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// thermistor
#define thermistorPin A0
#define R1 10000
// int Vo;
// float R1 = 10000;
// float logR2, R2, T, Tstr;
float thermVal;
float c1 = 1.009249522e-03;
float c2 = 2.378405444e-04;
float c3 = 2.019202697e-07;

// calibration information for thermistor (REALLY ROUGH CAL)
float a1 = 0.013;
float b1 = -0.879;
float a2 = -0.007;
float b2 = 0.528;
float a3 = 0.001;
float b3 = -0.321;
// float error, T_cal; 

// alcohol sensor
#define alcPin A1
float alc;

// pH sensor
#define phPin A3
// unsigned long int avgVal;
// float b;

// lcd display
LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

/* Retrieve Thermistor value and calibrate */
float GetThermistorValue()
{
  static float error, T_cal;

  static int Vo = analogRead(thermistorPin);
  static float R2 = R1 * (1023.0 / (float)Vo - 1.0);
  static float logR2 = log(R2);
  static float T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2)); // find steinhart coeficients
  T = T - 273.15; // convert to C
  T = ((T * 9.0) / 5.0 + 32.0); // convert to F
  
  if (T <= 69)
  {
    error = a1 * T + b1;
    T_cal = T * (error + 1);
  }
  else if ((T > 69) && (T <= 98)) 
  {
    error = a2 * T +b2;
    T_cal = T * (error + 1);
  }
  else // (T > 98) 
  {
    error = a3 * T + b3;
    T_cal = T * (error + 1);
  }

  return T_cal;  
}

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
    
  float phValue = (float)avgVal * 5.0/1024/6;
  phValue = 3.5 * phValue;

  return phValue  
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
  //thermistor data and calibration
  thermVal = GetThermistorValue();

  // Vo = analogRead(thermistorPin);
  // R2 = R1 * (1023.0 / (float)Vo - 1.0);
  // logR2 = log(R2);
  // T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2)); // find steinhart coeficients
  // T = T - 273.15; // convert to C
  // T = ((T * 9.0) / 5.0 + 32.0); // convert to F
  
  // if (T <= 69)
  // {
  //   error = a1 * T + b1;
  //   T_cal = T * (error + 1);
  // }

  // if ((T > 69) && (T <= 98)) 
  // {
  //   error = a2 * T +b2;
  //   T_cal = T * (error + 1);
  // }

  // if (T > 98) {
  //   error = a3 * T + b3;
  //   T_cal = T * (error + 1);
  // }

  // alcohol data
  alc = analogRead(alcPin);

  // pH sensor data and calibration
  phValue = GetPhValue();
  // for(int index = 0; index < 10; ++index)
  // { 
  //   buf[index] = analogRead(phPin);
  //   delay(10);
  // }
  // for(int i = 0; i < 9; ++i)
  // {
  //   for(int j = i + 1; j < 10; ++j)
  //   {
  //     if(buf[i] > buf[j])
  //     {
  //       temp = buf[i];
  //       buf[i] = buf[j];
  //       buf[j] = temp;
  //     }
  //   }
  // }
  // avgVal = 0;
  // for(int i = 2; i < 8; ++i)
  //   avgVal += buf[i];
  // float phValue = (float)avgVal * 5.0/1024/6;
  // phValue = 3.5 * phValue;

  Serial.print("    pH:");
  Serial.print(phValue, 2);
  Serial.println(" ");
  
  lcd.setCursor(0, 0);
  lcd.print("T:");
  // lcd.print(T_cal);
  lcd.print(thermVal);
  lcd.print("F");
  lcd.setCursor(0, 1);
  lcd.print("alc:");
  lcd.print(alc);
  lcd.print("ppm");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("pH:");
  lcd.print(phValue);
  delay(1000);
  lcd.clear();
}