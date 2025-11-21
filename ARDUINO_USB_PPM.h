#ifndef _ARDUINO_USB_PPM_H_
#define _ARDUINO_USB_PPM_H_

#include "TEST_PPM_Arduino/PPM_TxConfig.h"
#include "TEST_USB_Arduino/THRUSTMASTER_FCS.h"
#include <SPI.h>

#define USM_PPM_DEBUG

class ThrustMasterPPM : public THRUSTMASTER_FCS {

        uint16_t *ppm_array;
        bool throttle_safety;
        
        void OnGamePadChanged(const TMAxisData *axis);
        void OnHatSwitch(uint8_t hat);
        void OnButtonUp(uint8_t but_id);
        void OnButtonDn(uint8_t but_id);
public:
        ThrustMasterPPM(uint16_t *ppm, USB *usb );
};


#endif
