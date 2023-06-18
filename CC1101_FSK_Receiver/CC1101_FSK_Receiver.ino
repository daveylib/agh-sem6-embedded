#include <Arduino.h>
#include <RadioLib.h>

#define LED        13

#define PIN_CS    10
#define PIN_GDO0   4
#define PIN_GDO2   5

CC1101 radio = new Module(PIN_CS, PIN_GDO0, RADIOLIB_NC, PIN_GDO2);

byte buff[255] = {0};

byte sync[] = { 0x21, 0x69 };

void print_array(byte *arr, int size) {
  for (int i = 0; i < size; i++) {
    Serial.print(arr[i], HEX);
    Serial.print(" ");
  }
  
  Serial.println();
}

void errorLog(const char *msg, int state) {
  Serial.println(msg);
  
  Serial.print("Status code: ");
  Serial.println(state);
  
  for(;;) {}
}

void setup() {
  Serial.begin(115200);

  // RXRadio init
  Serial.println("Initializing radio...");

//   int state = radio.begin(434.3, 2.87, 16, 58, 10, 16);  // freq, br, freqDev, rxBw, pwr, preambleLen
  int state = radio.begin(434.32, 2.87, 16);
  if (state != RADIOLIB_ERR_NONE)
    errorLog("Error occurred while initializing radio", state);

  state = radio.setEncoding(RADIOLIB_ENCODING_NRZ);
  if (state != RADIOLIB_ERR_NONE)
    errorLog("Error occurred while setting encoding", state);

  state = radio.setSyncWord(sync, 2);
  if (state != RADIOLIB_ERR_NONE)
    errorLog("Error occurred while setting sync word", state);

  state = radio.setPreambleLength(16);
  if (state != RADIOLIB_ERR_NONE)
    errorLog("Error occurred while setting length of preamble", state);

  state = radio.setCrcFiltering(false);
  if (state != RADIOLIB_ERR_NONE)
    errorLog("Error occurred while disabling CRC filtering", state);

  state = radio.fixedPacketLengthMode(sizeof(buff));
  if (state != RADIOLIB_ERR_NONE)
    errorLog("Error occurred while setting fixed packet length", state);
    
  // Finish setup
  Serial.println("Finished setuping radio");
}

void loop() {
  int state = radio.receive(buff, sizeof(buff));
  
  if (state == RADIOLIB_ERR_NONE) {
    if (buff[0] == 0x2A and buff[1] == 0xAA) {
      Serial.println("\nReceived a new key!");
      print_array(buff, sizeof(buff));
    }
  }
}
