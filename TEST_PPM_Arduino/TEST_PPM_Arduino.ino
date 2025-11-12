/*
 *   For USB Shield Libraries Download: 
 *   https://github.com/felis/USB_Host_Shield_2.0/archive/master.zip
 */

/*
 * PPM generator originally written by David Hasko
 * on https://code.google.com/p/generate-ppm-signal/ 
 */

#include "PPM_TxConfig.h"

#define TEST_PPM_SEQ   //define this to run a test ppm sequence in the main loop

#define PPM_PIN 3                             //set PPM signal output pin on the arduino
#define SWITCH_PIN 16

/*
 * this array holds the servo values for the ppm signal
 * change theese values in your code (usually servo values move between 1000 and 2000)
 */
int ppm[CHANNEL_NUMBER];

void setup() {

  Serial.begin(115200);                             //initialize serial communication

#if !defined(__MIPSEL__)
  while (!Serial);                                  // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif

  Serial.print(F("\r\n Serial Started \n"));
  
                                          //initiallize default ppm values
  for(int i=0; i<CHANNEL_NUMBER; i++){
    if ( i == THROTTLE_AXIS) {            //set default throttle value
      ppm[i] = THROTTLE_DEFAULT_VALUE;
    } else {
      ppm[i]= CHANNEL_DEFAULT_VALUE;
    }
  }

  pinMode(PPM_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);

  digitalWrite(PPM_PIN, !PPM_PULSE_POLARITY);  //set the PPM signal pin to the default state
  
  cli();                          //stop interrupts
  TCCR1A = 0;                     // set TCCR1 register to 0
  TCCR1B = 0;                     // same for TCCR1B 
  
  OCR1A = 100;                    // compare match register, change this
  TCCR1B |= (1 << WGM12);         // turn on CTC mode
  TCCR1B |= (1 << CS11);          // clk/8 prescaler: 0,5 microseconds at 16mhz
  TIMSK1 |= (1 << OCIE1A);        // enable timer 1 interrupt on Output Compare A match 
  sei();                          // re-enable interrupts  
  
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


ISR(TIMER1_COMPA_vect){                       //timer1 interrupt service routine
  static boolean state = true;                //static variable to hold the state
  
  TCNT1 = 0;
  
  if (state) {                                      //start of pulse
    digitalWrite(PPM_PIN, PPM_PULSE_POLARITY);      //start PPM signal pulse
    OCR1A = PPM_PULSE_LEN * 2;                      //set the duration of the pulse
    state = false;                                  //next state will be the end of pulse 
  
  }else{                                     //end ppm pulse and calculate when to start the next pulse
    static byte cur_chan_numb;
    static unsigned int calc_rest;
  
    digitalWrite(PPM_PIN, !PPM_PULSE_POLARITY);     //reset the PPM signal pulse
    state = true;                                   //next state will be the start of another ppm pulse

    if(cur_chan_numb >= CHANNEL_NUMBER){        //all channels were sent, now calculate the rest time of the frame
      cur_chan_numb = 0;
      calc_rest = calc_rest + PPM_PULSE_LEN;    //add the last ppm pulse 
      OCR1A = (PPM_FRAME_LEN - calc_rest) * 2;  //set the duration of the sync/padding/rest time
      calc_rest = 0;
    }else{                                    //send next channel pulse
      OCR1A = (ppm[cur_chan_numb] - PPM_PULSE_LEN) * 2;          //set the duration of the pulse based on the ppm value
      calc_rest = calc_rest + ppm[cur_chan_numb];               //accumulate the pulse lengths for rest time calculation
      cur_chan_numb++;                                          //go to the next channel
    }     
  }
}
