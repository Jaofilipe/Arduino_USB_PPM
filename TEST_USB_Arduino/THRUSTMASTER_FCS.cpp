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

        for (uint8_t i = 0; i < sizeof(TMAxisData); i++){
                if ((TmJoyData.Axis.RZaxis != oldAxisState.RZaxis ) || (TmJoyData.Axis.Slider != oldAxisState.Slider ) || (TmJoyData.Axis.Xaxis != oldAxisState.Xaxis ) || (TmJoyData.Axis.Yaxis != oldAxisState.Yaxis ) ) {
                        OnGamePadChanged(&TmJoyData.Axis);
                        break;
                }
        }

        if (TmJoyData.Dpad.dpad_data != oldDpadState.dpad_data ) {
                OnHatSwitch(TmJoyData.Dpad.dpad_data);
                oldDpadState.dpad_data = TmJoyData.Dpad.dpad_data;
        }

        buttonClickState.all_buttons = (TmJoyData.Buttons.all_buttons ^ oldButtonState.all_buttons);

        // Calling Button Event Handler for every button changed
        if (buttonClickState.all_buttons) {
                for (uint8_t i = 0; i < 0x10; i++) {
                        uint16_t mask = (0x0001 << i);

                        if ((mask & buttonClickState.all_buttons) > 0) {
                                if ((TmJoyData.Buttons.all_buttons & mask) > 0){
                                        OnButtonDn(i + 1);
                                }else {
                                        OnButtonUp(i + 1);
                                }
                        }
                }
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
