#ifndef _ARDUINO_USB_PPM_H_
#define _ARDUINO_USB_PPM_H_

#include "TEST_PPM_Arduino/PPM_TxConfig.h"
#include "TEST_USB_Arduino/THRUSTMASTER_FCS.h"
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define USM_PPM_DEBUG_BUTTONS
#define USM_PPM_DEBUG_AXIS

class ThrustMasterPPM : public THRUSTMASTER_FCS {
        public:
        int16_t crude_ppm[CHANNEL_NUMBER];
        uint16_t *ppm_array;
        bool throttle_safety;
        ThrustMasterPPM(uint16_t *ppm, USB *usb );
        
        private:
                void OnGamePadChanged(const TMAxisData *axis);
                void OnHatSwitch(uint8_t hat);
                void OnButtonUp(uint8_t but_id);
                void OnButtonDn(uint8_t but_id);

                virtual void handleHatUp(void __attribute__((unused))){
                        return;        
                };
                virtual void handleHatDown(void __attribute__((unused))){
                        return;        
                };
                virtual void handleHatLeft(void __attribute__((unused))){
                        return;        
                };
                virtual void handleHatRight(void __attribute__((unused))){
                        return;        
                };

};

// states of the main display information menu

#define Display_Max_State 4

enum DisplayStates{
        Display_Idle = (uint8_t)0,
        Display_First_Channels,
        Display_Last_Channels,
        Display_Joy_Axis,
        Display_Joy_Buttons,
        Display_Failsafe_Values,
};

extern uint8_t display_states;

// Class to handle printing information into a display, ignore if not needed or change sparingly

class ThrustMasterPPM_Display : public ThrustMasterPPM {
        public:
        LiquidCrystal_I2C *Lcd_I2C;
        ThrustMasterPPM_Display(uint16_t *ppm, USB *usb, LiquidCrystal_I2C *display);
        
        private:
                void handleHatUp(void);
                void handleHatDown(void);
                void handleHatLeft(void);
                void handleHatRight(void);

};


#endif
