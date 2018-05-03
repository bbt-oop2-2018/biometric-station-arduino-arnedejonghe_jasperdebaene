#include <LiquidCrystal.h>
#include <Wire.h> 
#include "SparkFunTMP102.h" 
#include <SparkFun_MMA8452Q.h>

MMA8452Q accel;
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
TMP102 sensor0(0x48);

const int ALERT_PIN = A3;
int PulseSensorPurplePin = 0;       
int LED13 = 13;  
int Signal;                
int Threshold = 550;            


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


void loop() {

  //Start Temperaturesensor
  float temperature;
  boolean alertPinState, alertRegisterState;
  sensor0.wakeup();
  temperature = sensor0.readTempC();

  alertPinState = digitalRead(ALERT_PIN); 
  alertRegisterState = sensor0.alert();  
  
  sensor0.sleep();

//  Serial.print("Temperature: ");
//  Serial.print(temperature);
//  
//  Serial.print("\tAlert Pin: ");
//  Serial.print(alertPinState);
//  
//  Serial.print("\tAlert Register: ");
//  Serial.println(alertRegisterState);
//  
//  delay(1000); 
  //End Temperaturesensor


   //Start Pulsesensor
   Signal = analogRead(PulseSensorPurplePin);                                        
  // Serial.println(Signal);                   
  
   if(Signal > Threshold){                         
     digitalWrite(LED13,HIGH);
   } else {
     digitalWrite(LED13,LOW);               
   }
   //End Pulsesensor

    //Start LCD screen
    lcd.clear();
    lcd.print(temperature);
    lcd.print(" ");
    lcd.print((char)223);
    lcd.print("C");
    //End LCD screen
  
     delay(10);


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
    
    Serial.println(); 
 

}
//End Accelerometer
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
