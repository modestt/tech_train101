/*
  Arduino Mega 4 Serial test
  
*/


void setup() {
  // initialize both serial ports:
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
}

void loop() {
  // read from port 1, send to port 0:
  
  if (Serial1.available()) {
    int inByte = Serial1.read();
    Serial.write("Serial Port1 => ");
    Serial.write(inByte);
    Serial.write("\n\r");
    Serial1.write("\n\rACK Serial Port1");
    }

      if (Serial2.available()) {
    int inByte = Serial2.read();
    Serial.write("Serial Port2 => ");
    Serial.write(inByte);
    Serial.write("\n\r");
     Serial2.write("\n\rACK Serial Port2");
    }

      if (Serial3.available()) {
    int inByte = Serial3.read();
    Serial.write("Serial Port3 => ");
    Serial.write(inByte);
    Serial.write("\n\r");
     Serial3.write("\n\rACK Serial Port3");
    }

 }
