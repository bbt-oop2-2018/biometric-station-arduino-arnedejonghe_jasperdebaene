
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
  
  //Start Temperaturesensor
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
  //End Temperaturesensor


  //Start Pulsesensor
  pinMode(LED13,OUTPUT);         

  //End Pulsesensor 

  //Start Accelerometer
  lcd.begin(16, 2);
  accel.init();
  //End Accelerometer



}


char* buttons[] = {"Left", "Up", "Down", "Right", "Select", "Undefined"};

#define LEFT_BUTTON     0
#define UP_BUTTON       1
#define DN_BUTTON       2
#define RIGHT_BUTTON    3
#define SEL_BUTTON      4
#define UNDEFINED       5

int buttonindex;



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

  // First event
  if(myTimer1(delayTime, currentMillis) == 1){

   

    // Heart beat leading edge detected.
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
      //digitalWrite(LED_BUILTIN, HIGH);
    }

    // Heart beat trailing edge detected.
    if(reading < LowerThreshold && IgnoreReading == true){
      IgnoreReading = false;
      //digitalWrite(LED_BUILTIN, LOW);
    }  

    // Calculate Beats Per Minute.
    BPM = (1.0/PulseInterval) * 60.0 * 1000;
  }

  
 
   //End Pulsesensor

   
  
  


//Start Accelerometer

   if (accel.available())
  {
  
    accel.read();
    
    // accel.read() will update two sets of variables. 
    // * int's x, y, and z will store the signed 12-bit values 
    //   read out of the accelerometer.
    // * floats cx, cy, and cz will store the calculated 
    //   acceleration from those 12-bit values. These variables 
    //   are in units of g's.
    // Check the two function declarations below for an example
    // of how to use these variables.
    printCalculatedAccels();
    //printAccels(); // Uncomment to print digital readings
    
    // The library also supports the portrait/landscape detection
    //  of the MMA8452Q. Check out this function declaration for
    //  an example of how to use that.
    printOrientation();
    
    Serial.println("]"); 
    delay(100); 
 

}
//End Accelerometer

    //Start LCD screen
//    lcd.clear();
//    lcd.print(temperature);
//    lcd.print(" ");
//    lcd.print((char)223);
//    lcd.print("C ");
//    lcd.print(" PPM: ");
//    lcd.println(Signal);
//    lcd.print("a: ");
//    lcd.print(accel.x, 3);
//    lcd.print("\t");
//    lcd.print(accel.y, 3);
//    lcd.print("\t");
//    lcd.print(accel.z, 3);
//    lcd.print("\t");
    //End LCD screen



//Start Buttons
   //int buttonValue = 1023;
    int buttonValue = analogRead(A0);
    lcd.clear();
    lcd.print("Press Any Button");
   
//    while (buttonValue > 1015) 
//    {
//        buttonValue = analogRead(A0);
//        //Serial.println(buttonValue);
//    }

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
//        lcd.print("acceleration: ");
//        lcd.setCursor(0, 2);
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
    //lcd.clear();
    //lcd.print(buttons[buttonindex]);
    //lcd.print("  pressed");
    // wait for key to be releasted

//    while (buttonValue < 1000) 
//    {
//        buttonValue = analogRead(A0);
//        //delay(100000);
//    };  // sit in this loop till key unpressed
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

// This function demonstrates how to use the accel.cx, accel.cy,
//  and accel.cz variables.
// Before using these variables you must call the accel.read()
//  function!
void printCalculatedAccels()
{ 
  Serial.print(accel.cx, 3);
  Serial.print("\t");
  Serial.print(accel.cy, 3);
  Serial.print("\t");
  Serial.print(accel.cz, 3);
  Serial.print("\t");
}

// This function demonstrates how to use the accel.readPL()
// function, which reads the portrait/landscape status of the
// sensor.
void printOrientation()
{
  // accel.readPL() will return a byte containing information
  // about the orientation of the sensor. It will be either
  // PORTRAIT_U, PORTRAIT_D, LANDSCAPE_R, LANDSCAPE_L, or
  // LOCKOUT.
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


// First event timer
int myTimer1(long delayTime, long currentMillis){
  if(currentMillis - previousMillis >= delayTime){previousMillis = currentMillis;return 1;}
  else{return 0;}
}

