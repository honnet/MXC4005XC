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
  float data[4] = {0};   // units g (x,y,z) and celcius
  char buffer[27] = {0}; // example: -1.00, -1.00, -1.00, -100.0
  uint8_t index = 0;

  MXC4005XC_GetData(data);

  for (uint8_t i = 0; i < 3; i++) {
   // Serial.print() uses double by default, which takes a lot of space.
   // sprintf (with %f) avoids it:
    index += sprintf( &buffer[index], "%.2f\t", data[i] );
  }

//  index += sprintf( &buffer[index], "%.1f\t", data[3] ); // temperature

  buffer[index-1] = '\n'; // overwrite last tab
  Serial.print(buffer);

  delay(10);
}

