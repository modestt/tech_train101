/*
  Serial Event example

*/

String inputString = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup() {
  // initialize serial:
  Serial.begin(9600);
  Serial1.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

  pinMode(13, OUTPUT) ;
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    delay(10);
    digitalWrite(13, LOW);
    Serial.println(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent1() {
  while (Serial1.available()) {
    
    char inChar = (char)Serial1.read();
     // add it to the inputString:
    inputString += inChar;
      if (inChar == '\r') {stringComplete = true; }
  }
}
