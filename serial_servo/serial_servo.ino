#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 
int pos = 0;    // variable to store the servo position 

String inputString = "";
String initString = "inithighfive";
String finString = "endhighfive";
String zeroString = "returnzero";
boolean stringComplete = false;
 
void setup() 
{ 
  Serial.begin(9600);
  myservo.attach(8);  // attaches the servo on pin 8 to the servo object 
  inputString.reserve(20);
} 
 
 
void loop() 
{ 
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      stringComplete = true;
    }
    else {
      inputString += inChar;
    }
  }
    
  if (stringComplete) {
    Serial.println(inputString);
    if (inputString.equals(initString)) {
      Serial.println("Initializing High Five!");
      for (pos = 90; pos >= 0; pos--) {
        myservo.write(pos);
        delay(10);
      }
    }
    else if (inputString.equals(finString)) {
      Serial.println("High Five Completed!");
      for (pos = 0; pos <= 90; pos++) {
        myservo.write(pos);
        delay(10);
      }
    }
    else if (inputString.equals(zeroString)) {
      pos = myservo.read();
      if (pos > 90){
        for (pos; pos >= 90; pos--) {
          myservo.write(pos);
          delay(10);
        }
      }
      else {
        for (pos; pos <= 90; pos++) {
          myservo.write(pos);
          delay(10);
        }
      }
    } 
    else {
      Serial.println("No High Five :(");
    }    
    inputString = "";
    stringComplete = false; 
  } 
}

