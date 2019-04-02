/*************************************************************
  Visualizationsketch for ESP8266 & MPU6050
 *************************************************************/

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include <Wire.h>
#include <MPU6050_tockn.h>



// Auth Token should be the same in the Blynk App
char auth[] = "99cb76c5a85148cfaeb6f1781a2d572d";

// Mobile hotspot credentials
char ssid[] = "IO-06";
char pass[] = "12234567";


// Define X,Y,Z Variables for transmitting to Blynk App
#define XValue V0
#define YValue V1
#define ZValue V2

//Define MPU 
MPU6050 mpu6050(Wire);

//Define terminal
WidgetTerminal terminal(V4);


void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  while (Blynk.connect() == false) {        // Be patient. 
                                            // Wait for Blynk to come online  
  }
  
  Blynk.notify("Device Started");           // Notification to smartphone 
  
  Blynk.begin(auth, ssid, pass);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);            // Calibrate MPU

  
  terminal.clear();
  terminal.println(F("Blynk v" BLYNK_VERSION ": Device started"));
  terminal.println(F("----------------------------"));
  terminal.println(F("prothetic leg"));
  terminal.flush();
}

void loop()
{
  Blynk.run();
  mpu6050.update();

  // Store Values in Variables
  int16_t AngleX = mpu6050.getAngleX();
  int16_t AngleY = mpu6050.getAngleY();
  int16_t AngleZ = mpu6050.getAngleZ(); 

  // Transmit to Blynk App
  Blynk.virtualWrite(XValue, AngleX);
  Blynk.virtualWrite(YValue, AngleY);
  Blynk.virtualWrite(ZValue, AngleZ);
}

BLYNK_WRITE(V5)
{
  int state = param.asInt();
  
  if (state){
    mpu6050.calcGyroOffsets();
    terminal.println("Don't move");
  }
  terminal.flush();
}
