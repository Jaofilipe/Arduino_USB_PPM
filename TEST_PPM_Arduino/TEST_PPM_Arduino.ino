/*
 *   For USB Shield Libraries Download: 
 *   https://github.com/felis/USB_Host_Shield_2.0/archive/master.zip
 */

/*
 * PPM generator originally written by David Hasko
 * on https://code.google.com/p/generate-ppm-signal/ 
 */

#include "PPM_TxConfig.h"

/*
 * Define this to run a test ppm sequence in the main loop
 */
#define TEST_PPM_SEQ 

/*
 * Set PPM signal output pin on the arduino
 * Options:
 * 9  - uses Timer1 OC1A output pin
 * 10 - uses Timer1 OC1B output pin
 * other - needs to be handled manually in the code
 */
 #define PPM_PIN 10 

#if defined(PPM_PIN)            //check which Pin was defined for PPM output
  #if (PPM_PIN == 9)            // Pin 9 works best with Timer1 OC1A
    #define PPM_OC1A_USED
  #elif (PPM_PIN == 10)         // Pin 10 works best with Timer1 OC1B
    #define PPM_OC1B_USED
  #else
    #if (PPM_PIN <= 13) && (PPM_PIN <= 0) // other digital pins need to be handled manually
      #define PPM_MANUAL_USED 
    #else
      #error "PPM_PIN value is not valid. Choose 9, 10 or a digital pin between 0 and 13."
    #endif
  #endif
#else 
  #error "PPM_PIN is not defined in PPM_TxConfig.h"
#endif 

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

  cli();                          // stop interrupts
  TCCR1A = 0;                     // set TCCR1A register to 0
  TCCR1B = 0;                     // set TCCR1B register to 0 
  
  OCR1A = 100;                    // Timer 1 compare match register initial value
  
#if defined(PPM_OC1A_USED)          // if using OC1A pin ( pin 9 ) for PPM output 
                                     // On reset or startup OC1A register is always reset to “0”   
  #if PPM_PULSE_POLARITY == 0        // initialize the OC1A pin state according to PPM_PULSE_POLARITY
   TCCR1A |= (1 << COM1A1);          // set COM1A1 to enable "set" or "reset" on compare match       
   TCCR1A |= (1 << COM1A0);          // set COM1A0 so that OC1A is Set on compare match (set output to high level)
   TCCR1C |= (1 << FOC1A);           // Force Output Compare for OC1A making OC1A have value "1"
   TCCR1A = 0;                       // reset TCCR1A to 0 to avoid issues
  #endif
  
  TCCR1A |= (1 << COM1A0);           // set COM1A0 so that OC1A is Toggled on compare match (for PPM_PIN 9)

#elif defined(PPM_OC1B_USED)         // if using OC1B pin ( pin 10 ) for PPM output
                                     // On reset or startup OC1A register is always reset to “0”
  #if PPM_PULSE_POLARITY == 0        // initialize the OC1B pin state according to PPM_PULSE_POLARITY
   TCCR1A |= (1 << COM1B1);          // set COM1B1 to enable "set" or "reset" on compare match       
   TCCR1A |= (1 << COM1B0);          // set COM1B0 so that OC1B is Set on compare match (set output to high level)
   TCCR1C |= (1 << FOC1B);           // Force Output Compare for OC1B making OC1B have value "1"
   TCCR1A = 0;                       // reset TCCR1A to 0 to avoid issues
  #endif

  TCCR1A |= (1 << COM1B0);           // set COM1B0 so that OC1B is Toggled on compare match (for PPM_PIN 10)

#endif

  TCCR1B |= (1 << WGM12);         // turn on CTC mode
  TCCR1B |= (1 << CS11);          // clk/8 prescaler: 0,5 microseconds at 16mhz
  TIMSK1 |= (1 << OCIE1A);        // enable timer 1 interrupt on Output Compare A match 
  sei();                          // re-enable interrupts  

  pinMode(PPM_PIN, OUTPUT);       // IMPORTANT: When Using OC1A or OC1B outputs port definition MUST come last

#if defined(PPM_MANUAL_USED)                       // for pins other than 9 and 10  which need manual control
  digitalWrite(PPM_PIN, !PPM_PULSE_POLARITY);      // initialize the PPM output pin to idle state
#endif
  
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

/*
* Timer1 interrupt service routines for COMPA and COMPB
* This routine generates the PPM signal by toggling the output pin
*/

ISR(TIMER1_COMPA_vect){                       //timer1 interrupt service routine
  static boolean state = true;                //static variable to hold the state
  
  //TCNT1 = 0;                    //no need to reset the timer count in CTC mode
  
  if (state) {                                      //start of pulse

#if defined(PPM_MANUAL_USED)                        //manually control the PPM output pin                          
    digitalWrite(PPM_PIN, PPM_PULSE_POLARITY);      //start PPM signal pulse
#endif    
    
    OCR1A = PPM_PULSE_LEN * 2;                      //set the duration of the pulse
    state = false;                                  //next state will be the end of pulse 
  
  }else{                                     //end ppm pulse and calculate when to start the next pulse
    static byte cur_chan_numb;
    static unsigned int calc_rest;

#if defined(PPM_MANUAL_USED)                        //manually control the PPM output pin
    digitalWrite(PPM_PIN, !PPM_PULSE_POLARITY);     //reset the PPM signal pulse
#endif

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

ISR(TIMER1_COMPB_vect, ISR_ALIASOF(TIMER1_COMPA_vect)); //alias to use the same ISR for both COMPA and COMPB
