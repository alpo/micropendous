/*
  HCMS Display 
 Language: Arduino/Wiring
 
 Displays a string on an Avago HCMS-297x display
 Anything you send in the serial port is displayed
 when the Arduino receives a newline or carriage return.
 
 String library based on the Wiring String library:
 http://wiring.org.co/learning/reference/String.html
 
 created 12 Jun. 2008
 modified 13 Nov. 2008
 by Tom Igoe
 
 */
#include <LedDisplay.h>

// Define pins for the LED display. 
// You can change these, just re-wire your board:
#define dataPin 6              // connects to the display's data in
#define registerSelect 7       // the display's register select pin 
#define clockPin 8             // the display's clock pin
#define enable 9               // the display's chip enable pin
#define reset 10               // the display's reset pin

#define displayLength 8        // number of bytes needed to pad the string

// create am instance of the LED display:
LedDisplay myDisplay = LedDisplay(dataPin, registerSelect, clockPin, 
enable, reset, displayLength);

int brightness = 15;        // screen brightness
char myString[] = {
  'p','r','i','n','t','i','n','g'};
void setup() {
  Serial.begin(9600);

  // initialize the display library:
  myDisplay.begin();
  myDisplay.setString("Printing");
  myDisplay.home();
  myDisplay.setBrightness(brightness);
}

void loop() {

  for (int thisPosition = 0; thisPosition < 8; thisPosition++) {
    for (int thisChar = ' '; thisChar < 'z'; thisChar++) {
      myDisplay.write(thisChar);
      myDisplay.setCursor(thisPosition);
      delay(3);
    }
    myDisplay.write(myString[thisPosition]);
    delay(10);
  }
  delay(500);
  myDisplay.clear();
  myDisplay.home();
}
