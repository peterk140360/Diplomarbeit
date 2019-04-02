#include "ADXL345.h"

#include <Wire.h>
#include <SPI.h>

#include <RF24.h>
#include <nRF24L01.h>


RF24 radio(9, 10);    // declarate CE and CSN (SPI Bus)
const byte address[6] = "00001";  // pipe address for communication

ADXL345 adxl_foot;   // variable adxl_foot is an instance of the ADXL345 library
ADXL345 adxl_knee;   // 2nd ADXL - mounted on the knee

int16_t xF, yF, zF;   // Value for readings from Foot-mounted ADXL
int16_t xK, yK, zK;   // from Knee-mounted ADXL
int16_t rawXF, rawYF, rawZF;   
int16_t rawXK, rawYK, rawZK;




int16_t Value[5];  // Array which will be transferred

void setup() {
  Serial.begin(115200);
  
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);    // simply to prevent power supply issus
  radio.stopListening();            // stop listening, so we can transmit
  
  adxl_foot.powerOn();
  adxl_knee.powerOn();
}


void loop() { 
  adxl_foot.readAccel(&xF, &yF, &zF); // read the accelerometer values from accelerometer on foot
  adxl_knee.readAccel(&xK, &yK, &zK); // Values from knee for hip movement

  
//Offset from calibration test
  rawXF = xF + 168;
  rawYF = yF + 116;
  rawZF = zF + 10;

  rawXK = xK + 65;
  rawYK = yK - 92;
  rawZK = zK - 5;
  // two Axis for foot movement (left, right, back, forth)
  
// Set boundaries, so value is between +255 and -255
  if (rawXF < -255) 
    rawXF = -255; 
  else if (rawXF > 255) 
    rawXF = 255;
    
  if (rawYF < -255) 
    rawYF = -255; 
  else if (rawYK > 255) 
    rawYF = 255;

  // one Axis for knee movement 
  if (rawZF < -255) 
    rawZF = -255; 
  else if (rawZK > 255) 
    rawZF = 255;

// Two Axis for Hip movement (back and forth)
  if (rawXK < -255) 
    rawXK = -255; 
  else if (rawXK > 255) 
    rawXK = 255;

  if (rawYK < -255) 
    rawYK = -255; 
  else if (rawYK > 255) 
    rawYK = 255;


// store in arry
  Value[0] = rawXF;
  Value[1] = rawYF;
  Value[2] = rawZF;
  Value[3] = rawXK;
  Value[4] = rawYK;

// transfer array
  radio.write(Value, sizeof(Value));


// Visualization for better understanding
  Serial.print(" mappedRawX = "); Serial.print(rawXF);   // raw data with offset
  Serial.print(" mappedRawY = "); Serial.println(rawYF); // raw data with 
}
