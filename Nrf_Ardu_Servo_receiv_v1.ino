

#include <SPI.h>
#include <Servo.h>

#include <RF24.h>
#include <nRF24L01.h>


RF24 radio(9, 10);    // declarate CE and CSN (SPI Bus)
const byte address[6] = "00001";  // pipe address for communication

Servo servoFootBaF;   // create servo object to control a servo on Foot; Sideways
Servo servoFootS;     // foot Back and Forth
Servo servoKnee;      // knee
Servo servoHipBaF;    // hip Back and Forth
Servo servoHipS;      // hip Sideways




int16_t mappedXF, mappedYF, mappedZF;
int16_t mappedXK, mappedYK;
 
int Value[5];  // Array for received values


void setup() {
  Serial.begin(115200);
  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);    // simply to prevent power supply issus
  radio.startListening();            // Start listening, so we can hear
  

  servoFootBaF.attach(3);
  servoFootS.attach(5);
  servoKnee.attach(6);
  servoHipBaF.attach(4);
  servoHipS.attach(7);
}


void loop() {
  if (radio.available()) {  //when data is received, map it
    radio.read(Value, sizeof(Value));

// Boundary values are from tests with prototyp
    mappedXF = map(Value[0], -255, 255, 67, 116);  // foot back and forth
    mappedYF = map(Value[1], -255, 255, 68, 93);   // foot sideways
    mappedZF = map(Value[2], -255, 255, 95, 157);  // knee back and forth
    mappedXK = map(Value[3], -255, 255, 31, 130);  // hip back and forth
    mappedYK = map(Value[4], -255, 255, 70, 121);  // hip sideways


// Output 
    servoFootBaF.write(mappedXF);
    delay(1);
    servoFootS.write(180 - mappedYF);
    delay(1);
    servoKnee.write(mappedZF);
    delay(1);
    servoHipBaF.write(mappedXK);
    delay(1);
    servoHipS.write(180 - mappedYK);
    delay(1);

//Serial monitor for better understanding
    Serial.print(" X-Axis = "); Serial.println(mappedXF);
    Serial.print(" Z-Axis = "); Serial.println(mappedYF);
    Serial.println("sucsessfully reveived");
  }
}
