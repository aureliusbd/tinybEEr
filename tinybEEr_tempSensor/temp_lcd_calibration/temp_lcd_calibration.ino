#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

int thermistorPin = A0;
int Vo;
float R1 = 10000;
float logR2, R2, T, Tstr;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

// calibration information (REALLY ROUGH CAL)
float a1 = 0.013, b1 = -0.879;
float a2 = -0.007, b2 = 0.528;
float a3 = 0.001, b3 = -0.321;
float error, T_cal; 

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.init(); 
  lcd.backlight();
}

void loop() {
  Vo = analogRead(thermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2)); // find steinhart coeficients
  T = T - 273.15; // convert to C
  T = ((T * 9.0)/ 5.0 + 32.0); // convert to F
  
  if (T <= 69){
    error = a1 * T + b1;
    T_cal = T * (error + 1);
  }

  if ((T > 69) && (T<=98)){
    error = a2 * T +b2;
    T_cal = T * (error + 1);
  }

  if (T > 98) {
    error = a3 * T + b3;
    T_cal = T * (error + 1);
  }

  lcd.setCursor(0,0);
  lcd.print("Temp:");
  lcd.print(T);
  lcd.print("F");
  lcd.setCursor(0,1);
  lcd.print("Temp_cal:");
  lcd.print(T_cal);
  lcd.print("F");
  delay(2000);
  lcd.clear();
  //Serial.println(T);
  Serial.println("error:\t");
  Serial.print(error);
  Serial.println("Temp:\t");
  Serial.print(T);
  Serial.println("T_cal:\t");
  Serial.print(T_cal);
}
