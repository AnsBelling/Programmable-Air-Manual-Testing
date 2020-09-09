// Programmable-Air Orig. Author: tinkrmind
// https://github.com/orgs/Programmable-Air

#include "programmable_air.h" // programmable-air library
#include <Adafruit_NeoPixel.h> // neo-pixel library for the neopixels on the board, used to indicate inflation/suction

#define DEBUG 1

int state = UN_KNOWN;

int atmospheric_pressure = 508; // should be around 508
int threshold = 8;

void setup() {
  initializePins();
}

void loop() {
  showPressure();

  // If blue button is pressed start sucking air in / deflating soft robot
  if (readBtn(BLUE) && state != SUCKING) {
    // switch on pumps to 50% power
    switchOnPump(1, 100);
    switchOffPump(2);
    suck(); // deflate / air suction function
    state = SUCKING;
  }
  // if red button is pressed, start inflating soft robot
  else if (readBtn(RED) && state != BLOWING) {
    // switch on pumps to 50% power
    switchOnPump(2, 100);
    switchOffPump(1);
    blow(); //inflate function
    state = BLOWING;
  }

  // if neither button is pressed, vent the valves on the slave board but inflate briefly in case the suction function has been used
  else if (!readBtn(BLUE) && !readBtn(RED) && state != VENTING) {
    switchOnPump(1, 100);
    blow();
    delayWhileReadingPressure(250);
    switchOffPumps();
    vent();
    state = VENTING;
  }

  delayWhileReadingPressure(200);
}
