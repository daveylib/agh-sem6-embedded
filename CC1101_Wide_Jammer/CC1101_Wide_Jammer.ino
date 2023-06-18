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

  int state = radio.begin(433.5, 4.8, 1.587, 58, 10, 16);  // freq, br, freqDev, rxBw, pwr, preambleLen
  if (state == RADIOLIB_ERR_NONE)
    Serial.println("OK");
}

const char *data = "hello casdo oansod inasoin doasin doanis doinas ";

void loop() { 
  for (int ch = 0; ch < 25; ch++) {
    radio.setFrequency(433.5 + 0.05 * ch);
    radio.transmit(data);
  }
}
