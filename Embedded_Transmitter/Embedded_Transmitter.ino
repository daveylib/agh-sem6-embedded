#include <Arduino.h>
#include <RadioLib.h>

#define LED      13

#define PIN_CS   10
#define PIN_GDO0  4
#define PIN_GDO2  5

#define PIN_TRANSMIT 8

CC1101 radio = new Module(PIN_CS, PIN_GDO0, RADIOLIB_NC, PIN_GDO2);

void errorLog(const char *msg, int state) {
  Serial.println(msg);
  
  Serial.print("Status code: ");
  Serial.println(state);
  
  for(;;) {}
}

void setup() {
  pinMode(PIN_TRANSMIT, INPUT);
  Serial.begin(115200);

  Serial.println("Embedded system: FSK transmitter");

  Serial.println("Initializing radio...");

  int state = radio.begin(434.32, 4.8, 1.587, 58, 10, 16);  // freq, br, freqDev, rxBw, pwr, preambleLen
  if (state != RADIOLIB_ERR_NONE)
    errorLog("Error occurred while initializing radio", state);

  Serial.println("Finished setuping radio");

  Serial.println("\nStarting transmitting");
}

bool isFirstFreq = true;

void loop() {
  if (digitalRead(PIN_TRANSMIT) == HIGH) {    
    if (isFirstFreq)
      radio.setFrequency(434.32);
    else
      radio.setFrequency(433.60);
  
    isFirstFreq = !isFirstFreq;
    
    int state = radio.transmit("Hello, world");
    if (state != RADIOLIB_ERR_NONE)
      errorLog("Error occurred while starting transmission", state); 
  }
}
