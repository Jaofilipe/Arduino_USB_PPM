

/*
 * Define this to run a test ppm sequence in the main loop
 */
#define TEST_PPM_SEQ 

#include "PPM_TxConfig.h"

void setup() {

  Serial.begin(115200);                             //initialize serial communication

#if !defined(__MIPSEL__)
  while (!Serial);                                  // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif

  Serial.print(F("\r\n Serial Started \n"));
  setup_PPM();
  Serial.print(F("\r\n PPM Started \n"));
}

void loop() {

#if defined(TEST_PPM_SEQ)                    //test ppm signal generation by changing the first channel value between 1000 and 2000

  static int val = 1;
  
  ppm[0] = ppm[0] + val;
  if(ppm[0] >= 2000){ val = -1; }
  if(ppm[0] <= 1000){ val = 1; }
  delay(10);

#endif

}
