#include <Wire.h>   //Lib for I2C
#include <Servo.h>  //Lib for Servo
#define accel_mode (0x53)   //I2C address for decive >from Datasheet<
byte values[6];   // Two bytes for each axis (0x32) to (0x37)
char output[512]; // space to store values for serial port
Servo myservo;  // create servo object to control a servo
int pos = 0;    // variable to store the servo position

void setup() {
  Wire.begin();
  Serial.begin(9600);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object

  // Initialisation 
  Wire.beginTransmission(accel_mode);
  Wire.write(0x2D); // Define mode: >Power_CTL(Read/Write)<
  Wire.write(0);    // Clears register
  Wire.endTransmission();

  Wire.beginTransmission(accel_mode);
  Wire.write(0x2D);
  Wire.write(16);   // Start Measure-Mode
  Wire.endTransmission();

  Wire.beginTransmission(accel_mode);
  Wire.write(0x2D);
  Wire.write(8);     // Disable Sleep-Mode
  Wire.endTransmission();

}

void loop() {
  int xyzregister = 0x32; //DATAX0
  int x,y,z;

  Wire.beginTransmission(accel_mode);
  Wire.write(xyzregister);
  Wire.endTransmission();

  Wire.beginTransmission(accel_mode);
  Wire.requestFrom(accel_mode, 6);  // Waiting for 6 bytes (x,y,z)
/*
  int i = 0;
  while(Wire.available()){
    values[i] = Wire.read();
    i++;
  }
  Wire.endTransmission();
*/

  for(int i = 0; i<6 && Wire.available(); i++){
    values[i] = Wire.read();
  }
  Wire.endTransmission();
  
  // Redefining x,y,z
  x = (((int)values[1]) << 8) | values[0];  //x starts at 0
  y = (((int)values[3]) << 8) | values[2];  //y starts at 2
  z = (((int)values[5]) << 8) | values[4];  //z starts at 4

  // Convert to String
  sprintf(output, "X: %d  |  Y: %d  |  Z: %d", x, y, z);
  Serial.print(output);
  
  Serial.write(10); // space


  if(x>25){
     for (pos=0; pos<=100; pos +=1){
      myservo.write(pos);
     }
  }
  else{
    for (pos = 100; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      myservo.write(pos); 
    }
  }
}
