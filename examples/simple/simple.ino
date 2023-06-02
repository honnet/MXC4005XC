#include "MXC4005XC.h"

// example with stm32duino:
#define my_SCL 13 // PB6
#define my_SDA 14 // PB7


void setup() {
  Serial.begin(115200);
  Serial.print("Starting...\n");

  uint8_t transceived = 0;
  do {
    transceived = MXC4005XC_Enable(my_SDA, my_SCL);
    delay(300);
  } while (transceived == 0);
}


void loop() {
  float data[4] = {0};  // unit: g

  MXC4005XC_GetData(data);

  for (uint8_t i = 0; i < 3; i++) {
    Serial.print( data[i] );
    Serial.print('\t');
  }

//  Serial.print( data[3] ); // temperature in Celcius
  Serial.print('\n');

  delay(10);
}

