/* Fire Ancestor piezo sensors */

const int threshold = 1;  // threshold value to decide when the detected sound is a knock or not

int sensor0Reading = A0; // the piezo is connected to analog pin 0
int sensor1Reading = A1; // the piezo is connected to analog pin 1

byte noteON = 144;//note on command

void setup() {
  Serial.begin(9600);       // use the serial port
}

void loop() {
  // read the sensor and store it in the variable sensorReading:
  sensor0Reading = analogRead(A0);
  sensor1Reading = analogRead(A1);
  
  if (sensor0Reading > threshold) {
    Serial.write(sensor0Reading);
  }
  
  if (sensor1Reading > threshold) {
    Serial.write(sensor1Reading);
  }
  
  delay(10);  // delay to avoid overloading the serial port buffer
}
