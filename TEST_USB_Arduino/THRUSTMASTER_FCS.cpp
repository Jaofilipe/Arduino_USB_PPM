/*
 *   For USB Shield Libraries Download: 
 *   https://github.com/felis/USB_Host_Shield_2.0/archive/master.zip
 */

#include "THRUSTMASTER_FCS.h"

void THRUSTMASTER_FCS::ParseHIDData(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf) {
        
        if (HIDUniversal::VID != THRUSTMASTER_VID || HIDUniversal::PID != THRUSTMASTER_FCS_PID){ // Make sure the right device is actually connected
            return;
        }
        
#ifdef DEBUG_JOYSTICK
        if(len && buf)  {
                Serial.print("\r\n");
                for (uint8_t i = 0; i < len; i++) {
                 Serial.print(buf[i], HEX);
                 Serial.print(" ");
                }
        }
#endif
        memcpy(&TmJoyData, buf, min(len, MFK_CASTUINT8T sizeof(TmJoyData)));

        static TMButtonsData oldButtonState;

        if (TmJoyData.Buttons.all_buttons != oldButtonState.all_buttons) { // Check if anything has changed
                buttonClickState.all_buttons = TmJoyData.Buttons.all_buttons & ~oldButtonState.all_buttons; // Update click state variable
                oldButtonState.all_buttons = TmJoyData.Buttons.all_buttons;
        }
}

uint8_t THRUSTMASTER_FCS::OnInitSuccessful() { // Called by the HIDUniversal library on success
            if (HIDUniversal::VID != THRUSTMASTER_VID || HIDUniversal::PID != THRUSTMASTER_FCS_PID) {// Make sure the right device is actually connected
                return 1;
            }
            return 0;
        };

bool THRUSTMASTER_FCS::connected() {
        return HIDUniversal::isReady() && HIDUniversal::VID == THRUSTMASTER_VID && HIDUniversal::PID == THRUSTMASTER_FCS_PID;
};

// See: https://github.com/torvalds/linux/blob/master/drivers/hid/hid-steelseries.c
/*
void THRUSTMASTER_FCS::setLeds(uint16_t leds) {
        uint8_t buf[3];
        buf[0] = 0x40; // Report ID
        buf[1] = leds & 0xFF;
        buf[2] = (leds >> 8) & 0x7F;
        pUsb->outTransfer(bAddress, epInfo[epInterruptOutIndex].epAddr, sizeof(buf), buf);
}
*/
