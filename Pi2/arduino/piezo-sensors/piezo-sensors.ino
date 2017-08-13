/* Fire Ancestor piezo sensors */

const int threshold = 1;  // threshold value to decide when the detected sound is a knock or not

int sensorsCount = 16;
int sensorReadings[16];
int offset = 10;

bool arduinoDebug = true;

void setup() {
  Serial.begin(9600);       // use the serial port
}

void loop() {
  // read the sensor and store it in the array sensorReadings:
  sensorReadings[0] = analogRead(A0);
  sensorReadings[1] = analogRead(A1);
  sensorReadings[2] = analogRead(A2);
  sensorReadings[3] = analogRead(A3);
  sensorReadings[4] = analogRead(A4);
  sensorReadings[5] = analogRead(A5);
  sensorReadings[6] = analogRead(A6);
  sensorReadings[7] = analogRead(A7);
  sensorReadings[8] = analogRead(A8);
  sensorReadings[9] = analogRead(A9);
  sensorReadings[10] = analogRead(A10);
  sensorReadings[11] = analogRead(A11);
  sensorReadings[12] = analogRead(A12);
  sensorReadings[13] = analogRead(A13);
  sensorReadings[14] = analogRead(A14);
  sensorReadings[15] = analogRead(A15);

  for (int i = 0; i < sensorsCount; i++) {
    if (sensorReadings[i] > threshold) {
      Serial.write(i);
      Serial.write(sensorReadings[i]+offset);

      if (arduinoDebug) {
        Serial.print(i);
        Serial.println(sensorReadings[i]+offset);      
      }
    }
  }
  
  delay(5);  // delay to avoid overloading the serial port buffer
}
