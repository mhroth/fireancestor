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
    Serial.print("0 = ");
    Serial.println(sensor0Reading);
    MIDImessage(noteON, 60, 1);//turn note on
    delay(300);//hold note for 300ms
    MIDImessage(noteON, 60, 0);//turn note off (note on with velocity 0)
  }
  
  if (sensor1Reading > threshold) {
    Serial.print("1 = ");
    Serial.println(sensor1Reading);
    MIDImessage(noteON, 61, 1);//turn note on
    delay(300);//hold note for 300ms
    MIDImessage(noteON, 61, 0);//turn note off (note on with velocity 0)
  }
  
  delay(10);  // delay to avoid overloading the serial port buffer
}

//send MIDI message
void MIDImessage(byte command, byte data1, byte data2) {
  Serial.write(command);
  Serial.write(data1);
  Serial.write(data2);
}
