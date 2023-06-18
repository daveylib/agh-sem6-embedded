#include <Arduino.h>
#include <RadioLib.h>

#define LED      13

#define PIN_CS   10
#define PIN_GDO0  4
#define PIN_GDO2  5

#define PIN_TRANSMIT 8

CC1101 radio = new Module(PIN_CS, PIN_GDO0, RADIOLIB_NC, PIN_GDO2);

short keyToRead = 0;
bool receivedKeys = false;

byte firstKey[255] = { 0 };
byte secondKey[255] = { 0 };

byte sync[] = { 0xFF, 0xFF };  // sync word to set

void printArray(byte *arr, short size) {
  for (int i = 0; i < size; i++) {
    Serial.print(arr[i], HEX);
    Serial.print(" ");
  }
  
  Serial.println();
}

void errorLog(const char *msg, short state) {
  Serial.println(msg);
  
  Serial.print("Status code: ");
  Serial.println(state);
  
  for(;;) {}
}

void setup() {
  pinMode(PIN_TRANSMIT, OUTPUT);
  digitalWrite(PIN_TRANSMIT, LOW);
  
  Serial.begin(115200);

  Serial.println("Embedded system: FSK receiver");

  Serial.println("\nEnabling jammer...");
  digitalWrite(PIN_TRANSMIT, HIGH);

  Serial.println("\nInitializing radio...");

//  int state = radio.begin(434.32, 2.87, 16, 58, 10, 16);  // freq, br, freqDev, rxBw, pwr, preambleLen
  short state = radio.begin(434.32, 2.87, 16);
//  if (state != RADIOLIB_ERR_NONE)
//    errorLog("Error occurred while initializing radio", state);

  state = radio.setEncoding(RADIOLIB_ENCODING_NRZ);
//  if (state != RADIOLIB_ERR_NONE)
//    errorLog("Error occurred while setting encoding", state);

  state = radio.setSyncWord(sync, 2);
//  if (state != RADIOLIB_ERR_NONE)
//    errorLog("Error occurred while setting sync word", state);

  state = radio.setPreambleLength(16);
//  if (state != RADIOLIB_ERR_NONE)
//    errorLog("Error occurred while setting length of preamble", state);

  state = radio.setCrcFiltering(false);
//  if (state != RADIOLIB_ERR_NONE)
//    errorLog("Error occurred while disabling CRC filtering", state);

  state = radio.fixedPacketLengthMode(sizeof(firstKey));
//  if (state != RADIOLIB_ERR_NONE)
//    errorLog("Error occurred while setting fixed packet length", state);

  Serial.println("Finished setuping radio");

  Serial.println("\nStarting receiving");
}

void loop() {
  if (receivedKeys) {
    delay(1000);
    return;
  }

  short state;
  bool isCorrectKey = false;

  if (keyToRead == 0) {
    state = radio.receive(firstKey, sizeof(firstKey));

    if (firstKey[0] == 0x2A and firstKey[1] == 0xAA)
      isCorrectKey = true;
  } else if (keyToRead == 1) {
    state = radio.receive(secondKey, sizeof(secondKey));
    
    if (secondKey[0] == 0x2A and secondKey[1] == 0xAA)
      isCorrectKey = true;
  }
  
  if (state == RADIOLIB_ERR_NONE and isCorrectKey) {
    Serial.print("\nReceived the key no. ");
    Serial.println(keyToRead);
    
    if (keyToRead == 0) {
      printArray(firstKey, sizeof(firstKey));
    } else if (keyToRead == 1) {
      printArray(secondKey, sizeof(secondKey));

      receivedKeys = true;

      Serial.println("\nDisabling jammer...");
      digitalWrite(PIN_TRANSMIT, LOW);

      Serial.println("\nTransmitting the first key...");
      delay(1 * 1000);

      radio.transmit(firstKey, sizeof(firstKey));

      Serial.println("\nTransmitting the second key in 10s...");
      delay(10 * 1000);

      radio.transmit(secondKey, sizeof(secondKey));
    }

    if (keyToRead % 2 == 0) {
      state = radio.setFrequency(433.60);
      
//      if (state != RADIOLIB_ERR_NONE)
//        errorLog("Error occurred while setting the second frequency", state);
    } else {
      state = radio.setFrequency(434.32);
      
//      if (state != RADIOLIB_ERR_NONE)
//        errorLog("Error occurred while setting the second frequency", state);
    }

    keyToRead += 1;
  }
}
