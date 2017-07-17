/* Fire Ancestor piezo sensors */

const int threshold = 1;  // threshold value to decide when the detected sound is a knock or not

int sensor0Reading;
int sensor1Reading;

bool arduinoDebug = true;

void setup() {
  Serial.begin(9600);       // use the serial port
//  pinMode(2, INPUT_PULLUP);  
}

void loop() {
  // read the sensor and store it in the variable sensorReading:
  sensor0Reading = analogRead(A0);
  sensor1Reading = analogRead(A1);
  
  if (sensor0Reading > threshold) {
    Serial.write(0);
    Serial.write(sensor0Reading+10);
    
    if (arduinoDebug) {
      Serial.print("A0: ");
      Serial.println(sensor0Reading);      
    }
  }
  
  if (sensor1Reading > threshold) {
    Serial.write(1);
    Serial.write(sensor1Reading+10);
    
    if (arduinoDebug) {
      Serial.print("A1: ");
      Serial.println(sensor1Reading);
    }
  }
    
  delay(5);  // delay to avoid overloading the serial port buffer
}
