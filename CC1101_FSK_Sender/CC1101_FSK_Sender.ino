#include <RadioLib.h>

#define PIN_CS1    10
#define PIN_GDO0   4
#define PIN_GDO2   5

#define CC1101_GDO0 PIN_GDO0

CC1101 radio = new Module(PIN_CS1, PIN_GDO0, RADIOLIB_NC, PIN_GDO2);

void setup() {
  Serial.begin(115200);

  Serial.print(F("[CC1101] Initializing ... "));
  int state = radio.begin(434.32, 2.87, 16);
  
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  radio.setOutputPower(10);

  radio.setSyncWord(0x21, 0x69, 0, false);

  Serial.print(F("[CC1101] Sending packet ... "));
  transmissionState = radio.transmit("Hello World from the CC1101 module! Hello World from the CC1101 module! Hello World from the CC1101 module! Hello World from the CC1101 module! Hello World from the CC1101 module! Hello World from the CC1101 module! Hello World from the CC1101 module!");
  
  if (transmissionState == RADIOLIB_ERR_NONE) {
    Serial.println(F("transmission finished!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(transmissionState);
  }

  radio.finishTransmit();
}

void loop() {
   delay(1000);
}
