/*
 *   For USB Shield Libraries Download: 
 *   https://github.com/felis/USB_Host_Shield_2.0/archive/master.zip
 */

/*
 * PPM generator originally written by David Hasko
 * on https://code.google.com/p/generate-ppm-signal/ 
 */

//#include <PS3USB.h>
#include <SPI.h>

/*
 *  Select your control mode by uncommenting the corresponding line
 */

//#define MODE_1 //RETA
//#define MODE_2 //RTEA
#define MODE_3 //AETR
//#define MODE_4 //ATER

// Define axis channel mapping based on selected mode
#if defined(MODE_1)
  #define YAW_AXIS 0
  #define PITCH_AXIS 1
  #define THROTTLE_AXIS 2
  #define ROLL_AXIS 3
#ekif defined(MODE_2)
  #define YAW_AXIS 0
  #define THROTTLE_AXIS 1
  #define PITCH_AXIS 2
  #define ROLL_AXIS 3
#elif defined(MODE_3)
  #define ROLL_AXIS 0
  #define PITCH_AXIS 1
  #define THROTTLE_AXIS 2
  #define YAW_AXIS 3
#elif defined(MODE_4)
  #define ROLL_AXIS 0
  #define THROTTLE_AXIS 1   
  #define PITCH_AXIS 2
  #define YAW_AXIS 3
#else
    #error "Define Transmitter Axis Mapping"
#endif

/*
 *   Select your transmitter PPM settings by uncommenting the corresponding line
 */

#define FLYSKY_I6X                            //uncomment this line if you use Flysky i6x transmitter
//#define DEFAULT_PPM_TX                        //uncomment this line to use generic TX default ppm values

#if defined(FLYSKY_I6X)
#define CHANNEL_NUMBER 10                     //set the number of chanels
#define CHANNEL_DEFAULT_VALUE 1500            //set the default servo value
#define THROTTLE_DEFAULT_VALUE 1000           //set the default throttle value
#define PPM_FRAME_LEN 20000                   //set the PPM frame length in microseconds (1ms = 1000µs)
#define PPM_PULSE_LEN 400                     //set the ppm separation pulse length
#define PPM_PULSE_POLARITY 0                  //set polarity of the ppm pulses: 1 is positive, 0 is negative
#elif defined(DEFAULT_PPM_TX)
#define CHANNEL_NUMBER 8                     //set the number of chanels
#define CHANNEL_DEFAULT_VALUE 1500            //set the default servo value
#define THROTTLE_DEFAULT_VALUE 1000           //set the default throttle value
#define PPM_FRAME_LEN 22500                   //set the PPM frame length in microseconds (1ms = 1000µs)
#define PPM_PULSE_LEN 300                     //set the ppm separation pulse length
#define PPM_PULSE_POLARITY 1                  //set polarity of the ppm pulses: 1 is positive, 0 is negative
#else
    #error "Define Transmitter Type"
#endif

#define PPM_PIN 3                             //set PPM signal output pin on the arduino
#define SWITCH_PIN 16

//USB Usb;

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

  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start \n"));
    while (1); //halt
  }

  Serial.print(F("\r\n USB Library Started \n"));
  
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

  //Usb.Task();
  static int val = 1;
  
  ppm[0] = ppm[0] + val;
  if(ppm[0] >= 2000){ val = -1; }
  if(ppm[0] <= 1000){ val = 1; }
  delay(10);
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
