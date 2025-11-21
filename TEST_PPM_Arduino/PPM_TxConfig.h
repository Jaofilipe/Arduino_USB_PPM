#ifndef _PPM_TX_CONFIG_H_  // only include this file once 
#define _PPM_TX_CONFIG_H_

#include"Arduino.h"
/*
 *  Select your control mode by uncommenting the corresponding line
 */

//#define MODE_1 //RETA
//#define MODE_2 //RTEA
#define MODE_3 //AETR
//#define MODE_4 //ATER

/*
 * Define axis channel mapping based on selected mode
 */
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

extern uint16_t ppm[CHANNEL_NUMBER];
extern void setup_PPM(void);

#endif // _PPM_TX_CONFIG_H_
