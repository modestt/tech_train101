

  #include <Arduino.h>
  #include <stdint.h>
 
bool  result = false;  // whether the string is complete
unsigned long int milliSec = 0 ;
char milliSecStr[50]  ;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  milliSec = millis() ;
  
}

void loop() {
 
 delay(1000);
 milliSec = millis() ;
 //sprintf(milliSecStr,%d,millSec);
 itoa(milliSec,milliSecStr,10) ;
 strcat(milliSecStr,"\r\n");
 Serial.print(milliSecStr) ;
 
}

