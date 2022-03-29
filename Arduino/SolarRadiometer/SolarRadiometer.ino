#include <Servo.h>
#include <C:\Users\w63x712\Documents\MSU Courses\Remote Sensing\SolarRadiometerProject\Arduino\SolarRadiometer\SolarPosition.h>

// Make sure to copy the Time and SolarPosition libraries in the same directory. Modify the line above

// Test positions
SolarPosition Home(45.668305, -111.059107);  // Nelson Story Tower
SolarPosition Cobleigh(45.666596, -111.045824); // Cobleigh Hall Rooftop (Final Test)

// Create a fixed UNIX initial time. 
// IMPORTANT: The initial hour MUST be provided as a reference for Arduino. COMPILE RIGHT BEFORE USING THE CODE.
// Also, align the device to the East or until there is no shadow around the tube. 
int someS = 0;  //second
int someM = 56;  //minute
int someH = 7 + 6; //hour. +6 (MST) to get UTM time
int someD = 23; //day
int someMo = 3; //month
int someY = 2022; //year
tmElements_t someTime = {someS, someM, someH, 0, someD, someMo, CalendarYrToTm(someY) };
time_t initialTime = makeTime(someTime);

// Servo variables
Servo servobase, servoled;
float azimuth, zenith;

// Voltage reader variables
const float arduinoVCC = 5.01;    //Arduino voltage
const int alanogPin = A0;         //the pin connecting the voltage. 
const int inputResolution =1023;  //works with most Arduino boards


void setup() {  
  // Configure servo pins
  servobase.attach(5);
  servoled.attach(6);

  // Set initial servo positions
  servobase.write(0);  // Look EAST
  servoled.write(0);   // Look up (Zenith = 0) 
  
  Serial.begin(9600);
  Serial.println("Initializing Solar Radiometer........");
  delay(500);  

  // set Time clock 
  setTime(initialTime);
  // set the Time library time service as the time provider
  SolarPosition::setTimeProvider(now);

  // Align the tube to the calculated azimuth and zenith angles.
  // Stop here during 20 seconds, during which period you should align the device with the Sun
  // making sure there is no shadow around the tube
  // Calculate solar angle (zenith and azimuth).
  azimuth = 180 - getAzimuth(Cobleigh, initialTime);
  zenith = getZenith(Cobleigh, initialTime);
  // Move servos to the calculated position.
  servobase.write(azimuth);  
  servoled.write(zenith);
  Serial.println(F("You have 20 seconds to align the device with the Sun..."));
  delay(20000);
}

void loop() {
  /*
   * Main loop. Refreshes every minute.
   */

  // Step 1: Calculate solar angle (zenith and azimuth).
  azimuth = 180 - getAzimuth(Home, initialTime);
  zenith = getZenith(Home, initialTime);
  Serial.print(F("The sun was at a zenith of "));
  Serial.print(zenith, 4);
  Serial.print(F(" and an azimuth (servo angle) of "));
  Serial.println(azimuth, 4);
  digitalClockDisplay();

  // Step 2: Move servos to the calculated position.
  Serial.println(F("Moving to corresponding position..."));
  servobase.write(azimuth);  
  servoled.write(zenith);

  // Step 3: Perform grid search around the current position to find the maximum voltage lecture.  
  Serial.println(F("Performing grid search..."));
  float maxV = gridSearch();
  Serial.print("Maximum Average Voltage (V): \t");
  Serial.println(maxV);

  // Step 4: Save voltage measurement in a CSV file.
  // Use Tools->ArduSpreadSheet and save the output.
 
  delay(60000); 

  // Step 5: Update time
  someS = second();  
  someM = minute();  
  someH = hour(); 
  someTime = {someS, someM, someH, 0, someD, someMo, CalendarYrToTm(someY)};
  initialTime = makeTime(someTime);
}


float gridSearch(){
  // Moves the zenith nd azimuth angles of the servo +/-5° searching the maximum voltage lecture
  float Vr;
  float maxV = 0;
  delay(1000);
  for(int ang1 = azimuth - 3; ang1 <= azimuth + 3; ang1 += 1){
    for(int ang2 = zenith - 3; ang2 <= zenith + 3; ang2 += 1){
      // Move to the new position
      servobase.write(ang1);  
      servoled.write(ang2);
      delay(100); // Pause just to ensure stability
      // Read voltage
      Vr = getVoltageAverage();
      // Update maximum voltage if necessary
      if(Vr >= maxV){
        maxV = Vr; 
      }
    }
  }
  // Note: This process takes ~8sec
  return maxV;  
}


float getZenith(SolarPosition Position, time_t time_eval){  
  return 90 - Position.getSolarPosition(time_eval).elevation;  // Convert elevation to zenith
}

float getAzimuth(SolarPosition Position, time_t time_eval){  
  return Position.getSolarPosition(time_eval).azimuth - 90;  // Calculate azimuth and subtract 90° bc the radiometer starts facing East
}


float getVoltageAverage(){
  //Take 10 voltage samples with an interval of 5ms, then calculate the average.
  float voltage = 0; int A0Value;
  for(int i=0; i < 10; i++){
    A0Value = analogRead(alanogPin);
    voltage += A0Value * (arduinoVCC / (float)inputResolution); 
    delay(10);
  }      
  return voltage / 10;
}


void digitalClockDisplay(){
  // Digital clock display of the time
  Serial.print(someH);
  printDigits(someM);
  printDigits(someS);
  Serial.print(" ");
  Serial.print(someD);
  Serial.print(" ");
  Serial.print(someMo);
  Serial.print(" ");
  Serial.print(someY); 
  Serial.println(); 
}

void printDigits(int digits){
  // Utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
