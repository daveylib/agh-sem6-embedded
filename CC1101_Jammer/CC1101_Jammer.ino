#include <Arduino.h>
#include <RadioLib.h>

#define LED        13

#define PIN_CS1    10
#define PIN_GDO0   4
#define PIN_GDO2   5

#define CC1101_GDO0 PIN_GDO0

CC1101 radio = new Module(PIN_CS1, PIN_GDO0, RADIOLIB_NC, PIN_GDO2);

void setup() {
  Serial.begin(115200);

  Serial.print("Initializing ... ");

  int state = radio.begin(434.32, 4.8, 1.587, 58, 10, 16);  // freq, br, freqDev, rxBw, pwr, preambleLen
  if (state == RADIOLIB_ERR_NONE)
    Serial.println("OK");
}

const char *data = "hello casdo oansod inasoin doasin doanis doinas ";

void loop() {
  Serial.print("Transmitting ...");

  int state = radio.transmit(data);

  if (state == RADIOLIB_ERR_NONE) {
    Serial.println("OK");
  } else if (state == RADIOLIB_ERR_PACKET_TOO_LONG) {
    Serial.println("TOO LONG");
  } else {
    Serial.print("ERROR");
    Serial.println(state);
  }

  delay(1);
}
