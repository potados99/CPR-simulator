#include "Display.h"

Display myDisp(3, 2);

void smoothen(double& val);
void lowerSensitivity(double& val);
void levelShift(double& val);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
   while (!Serial) {
     ; // wait for serial port to connect. Needed for native USB port only
   }

  pinMode(A7, INPUT);


   
}

void loop() {
  // put your main code here, to run repeatedly:
  double readVal = analogRead(A7);

  smoothen(readVal);
  //lowerSensitivity(readVal);
  levelShift(readVal);

  myDisp.show(readVal, 2);
  Serial.println(readVal);
}

void smoothen(double& val) {
  static const int numReadings = 50;

  static int readings[numReadings];      // the readings from the analog input
  static int readIndex = 0;              // the index of the current reading
  static int total = 0;                  // the running total
  static int average = 0;                // the average

  // subtract the last reading:
   total = total - readings[readIndex];
   
   // read from the sensor:
   readings[readIndex] = (val > 5) ? val : 0;
   
   // add the reading to the total:
   total = total + readings[readIndex];
   
   // advance to the next position in the array:
   readIndex = (readIndex + 1) % numReadings;

   // calculate the average:
   average = total / numReadings;
   val = average;
}

void lowerSensitivity(double& val) {
  static double lastVal = 0;

  if (fabs(lastVal - val) >= 20 ) lastVal = val;
  else val = lastVal;
}

void levelShift(double& val) {
  val = val / 63L;
}
