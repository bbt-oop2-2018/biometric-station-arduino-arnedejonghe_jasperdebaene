//Author: Arne De Jonghe

#include <LiquidCrystal.h>
#include <Wire.h> 
#include "SparkFunTMP102.h" 
#include <SparkFun_MMA8452Q.h>

MMA8452Q accel;
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
TMP102 sensor0(0x48);

const int ALERT_PIN = A3;
int PulseSensorPurplePin = 2;       
int LED13 = 13;  
int Signal;                
int Threshold = 550;            
int LowerThreshold = 490;
int reading = 0;
float BPM = 0.0;
bool IgnoreReading = false;
bool FirstPulseDetected = false;
unsigned long FirstPulseTime = 0;
unsigned long SecondPulseTime = 0;
unsigned long PulseInterval = 0;
const unsigned long delayTime = 10;
const unsigned long delayTime2 = 1000;
const unsigned long baudRate = 9600;
unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;


void setup() {
  
  //Start Temperaturesensor setup
  Serial.begin(9600); 
  pinMode(ALERT_PIN,INPUT);  
  sensor0.begin();  
  sensor0.setFault(0);  
  sensor0.setAlertPolarity(1); 
  sensor0.setAlertMode(0);  
  sensor0.setConversionRate(2);
  sensor0.setExtendedMode(0);
  sensor0.setHighTempF(85.0); 
  sensor0.setLowTempF(84.0);  
  //End Temperaturesensor setup

  //Start Accelerometer setup
  lcd.begin(16, 2);
  accel.init();
  //End Accelerometer setup



}


//Button values
char* buttons[] = {"Left", "Up", "Down", "Right", "Select", "Undefined"};

#define LEFT_BUTTON     0
#define UP_BUTTON       1
#define DN_BUTTON       2
#define RIGHT_BUTTON    3
#define SEL_BUTTON      4
#define UNDEFINED       5

int buttonindex;
//Button values


void loop() {

  //Start Temperaturesensor
  float temperature;
  boolean alertPinState, alertRegisterState;
  sensor0.wakeup();
  temperature = sensor0.readTempC();

  alertPinState = digitalRead(ALERT_PIN); 
  alertRegisterState = sensor0.alert();  
  
  sensor0.sleep();

  Serial.print("[");
  Serial.print(temperature);
  Serial.print(" | ");
  //End Temperaturesensor


   //Start Pulsesensor
   Signal = analogRead(PulseSensorPurplePin);    
                                    
   Serial.print(Signal);
   Serial.print(" | ");     

   unsigned long currentMillis = millis();

   if(myTimer1(delayTime, currentMillis) == 1){

    if(Signal > Threshold && IgnoreReading == false){
      if(FirstPulseDetected == false){
        FirstPulseTime = millis();
        FirstPulseDetected = true;
      }
      else{
        SecondPulseTime = millis();
        PulseInterval = SecondPulseTime - FirstPulseTime;
        FirstPulseTime = SecondPulseTime;
      }
      IgnoreReading = true;
     
    }
   
    if(reading < LowerThreshold && IgnoreReading == true){
      IgnoreReading = false;
    
    }  

    BPM = (1.0/PulseInterval) * 60.0 * 1000;
   }
   //End Pulsesensor

   
  
  


   //Start Accelerometer

   if (accel.available()){
  
    accel.read();
    printCalculatedAccels();
    printOrientation();
    Serial.println("]"); 
    delay(100); 
}
    //End Accelerometer


   //Start Buttons
    int buttonValue = analogRead(A0);
    lcd.clear();
    lcd.print("Bio Station");
    lcd.setCursor(0, 2);
    lcd.print("Arne en Jasper");

    if (buttonValue > 845 && buttonValue < 865)  // left button
    {
        buttonindex = LEFT_BUTTON;
        lcd.clear();
        lcd.print("Heart Pulse: ");
        lcd.setCursor(15, 0);
          if(Signal > Threshold){                         
           digitalWrite(LED13,HIGH);
           lcd.print((char)219);
          } else {
           digitalWrite(LED13,LOW);        
           lcd.print("-");       
          }
        lcd.setCursor(0, 2);
        lcd.print("BPM: ");
        lcd.print(BPM);
     
        
    }
    else if (buttonValue > 915 && buttonValue < 949)  // UP button
    {
        buttonindex = UP_BUTTON;
            lcd.clear();
            lcd.print("Temperature:");
            lcd.setCursor(0, 2);
            lcd.print(temperature);
            lcd.print(" ");
            lcd.print((char)223);
            lcd.print("C");
            
    }
    else if (buttonValue > 895 && buttonValue < 910) // down button
    {
        buttonindex = DN_BUTTON;
    }
    else if (buttonValue > 810 && buttonValue < 820) // right button
    {
        buttonindex = RIGHT_BUTTON;
        lcd.clear();
        lcd.print("X:");
        lcd.print(accel.cx, 3);
        lcd.print("g ");
        lcd.print("Y:");
        lcd.print(accel.cy, 3);
        lcd.print("g");
        lcd.setCursor(0, 2);
        lcd.print("Z:");
        lcd.print(accel.cz, 3);
        lcd.print("g");
    }
    else if (buttonValue > 605 && buttonValue < 620) // select button
    {
        buttonindex = SEL_BUTTON;
    }
    else buttonindex = UNDEFINED;
    //End Buttons

    
}


void printAccels()
{
  Serial.print(accel.x, 3);
  Serial.print("\t");
  Serial.print(accel.y, 3);
  Serial.print("\t");
  Serial.print(accel.z, 3);
  Serial.print("\t");
}


void printCalculatedAccels()
{ 
  Serial.print(accel.cx, 3);
  Serial.print("\t");
  Serial.print(accel.cy, 3);
  Serial.print("\t");
  Serial.print(accel.cz, 3);
  Serial.print("\t");
}


void printOrientation()
{
  byte pl = accel.readPL();
  switch (pl)
  {
  case PORTRAIT_U:
    Serial.print("Portrait Up");
    break;
  case PORTRAIT_D:
    Serial.print("Portrait Down");
    break;
  case LANDSCAPE_R:
    Serial.print("Landscape Right");
    break;
  case LANDSCAPE_L:
    Serial.print("Landscape Left");
    break;
  case LOCKOUT:
    Serial.print("Flat");
    break;
  }
}


int myTimer1(long delayTime, long currentMillis){
  if(currentMillis - previousMillis >= delayTime){previousMillis = currentMillis;return 1;}
  else{return 0;}
}

