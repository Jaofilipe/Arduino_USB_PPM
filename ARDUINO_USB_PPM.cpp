
#include "ARDUINO_USB_PPM.h"


ThrustMasterPPM::ThrustMasterPPM(uint16_t *ppm, USB *usb ) : ppm_array(ppm), THRUSTMASTER_FCS(usb), throttle_safety(true) {

        //do something when initializing
};

void ThrustMasterPPM::OnGamePadChanged(const TMAxisData *axis){

        ppm_array[YAW_AXIS]      = map(axis->RZaxis,0x00,0xFFFF,1000,2000); 
        ppm_array[ROLL_AXIS]     = map(axis->Xaxis ,0x00,0xFFFF,1000,2000);
        ppm_array[THROTTLE_AXIS] = ((throttle_safety) ? (map(axis->Slider,0x00,0xFF,1000,2000)):(1000));
        ppm_array[PITCH_AXIS]    = map(axis->Yaxis ,0x00,0xFF,1000,2000); 

#if defined(USM_PPM_DEBUG)
        Serial.print(F("X Axis: "));
        Serial.println(axis->Xaxis, HEX);
        Serial.print(F("CH "));
        Serial.print(ROLL_AXIS);
        Serial.print(F(": Roll: "));
        Serial.println(ppm_array[ROLL_AXIS]);

        Serial.print(F("Y Axis: "));
        Serial.println(axis->Yaxis, HEX);
        Serial.print(F("CH "));
        Serial.print(PITCH_AXIS);
        Serial.print(F(": Pitch: "));
        Serial.println(ppm_array[PITCH_AXIS]);
        
        Serial.print(F("RZ Axis: "));
        Serial.println(axis->RZaxis, HEX);
        Serial.print(F("CH "));
        Serial.print(YAW_AXIS);
        Serial.print(F(": Yaw: "));
        Serial.println(ppm_array[YAW_AXIS]);

        Serial.print(F("Slider: "));
        Serial.println(axis->RZaxis, HEX);
        Serial.print(F("CH "));
        Serial.print(THROTTLE_AXIS);
        Serial.print(F(": Throttle: "));
        Serial.println(ppm_array[THROTTLE_AXIS]);
#endif
}

void ThrustMasterPPM::OnHatSwitch(uint8_t hat){

#if defined(USM_PPM_DEBUG)
    Serial.println(F("Hat Switch: "));
#endif
        switch (hat)
        {
        case DPAD_UP:
#if defined(USM_PPM_DEBUG)
                Serial.println(F("Up"));
#endif
                break;
        case DPAD_UP_RIGHT:
#if defined(USM_PPM_DEBUG)
                Serial.println(F("UP & right"));
#endif
                break;
        case DPAD_RIGHT:
#if defined(USM_PPM_DEBUG)
                Serial.println(F("Right"));
#endif
                break;
        case DPAD_RIGHT_DOWN:
#if defined(USM_PPM_DEBUG)
                Serial.println(F("Right & down"));
#endif
                break;
        case DPAD_DOWN:
#if defined(USM_PPM_DEBUG)
                Serial.println(F("Down"));
#endif                
                break;
        case DPAD_DOWN_LEFT:
#if defined(USM_PPM_DEBUG)
                Serial.println(F("Down & left"));
#endif
                break;
        case DPAD_LEFT:
#if defined(USM_PPM_DEBUG)
                Serial.println(F("Left"));
#endif
                break;
        case DPAD_LEFT_UP:
#if defined(USM_PPM_DEBUG)
                Serial.println(F("Left & up"));
#endif
                break;
        case DPAD_OFF:
#if defined(USM_PPM_DEBUG)

#endif
                break;
        
        default:
#if defined(USM_PPM_DEBUG)
                Serial.print(F("Unknown state: "));
                Serial.println(TmJoyData.Dpad.dpad_data, HEX);
#endif
                break;
        }
}

void ThrustMasterPPM::OnButtonUp(uint8_t but_id){

#if defined(USM_PPM_DEBUG)
    Serial.print(F("Button Up id: "));
    Serial.println(but_id, HEX);
#endif

    switch (but_id) {
        case TRIGGER:
#if defined(USM_PPM_DEBUG)
            Serial.println(F("Button Up: TRIGGER"));
#endif
            break;
        case THUMB_BOTTOM:
#if defined(USM_PPM_DEBUG)
            Serial.println(F("Button Up: THUMB_BOTTOM"));
#endif
            break;
        case THUMB_LEFT:
#if defined(USM_PPM_DEBUG)
            Serial.println(F("Button Up: THUMB_LEFT"));
#endif
            break;
        case THUMB_RIGHT:
#if defined(USM_PPM_DEBUG)
            Serial.println(F("Button Up: THUMB_RIGHT"));
#endif
            break;
        case LEFT_FRONT_ONE:
#if defined(USM_PPM_DEBUG)
            Serial.println(F("Button Up: LEFT_FRONT_ONE"));
#endif
            break;
        case LEFT_BACK_ONE:
#if defined(USM_PPM_DEBUG)
            Serial.println(F("Button Up: LEFT_BACK_ONE"));
#endif
            break;
        case LEFT_FRONT_TWO:
#if defined(USM_PPM_DEBUG)
            Serial.println(F("Button Up: LEFT_FRONT_TWO"));
#endif
            break;
        case LEFT_BACK_TWO:
#if defined(USM_PPM_DEBUG)
            Serial.println(F("Button Up: LEFT_BACK_TWO"));
#endif
            break;
        case LEFT_FRONT_THREE:
#if defined(USM_PPM_DEBUG)
            Serial.println(F("Button Up: LEFT_FRONT_THREE"));
#endif
            break;
        case LEFT_BACK_THREE:
#if defined(USM_PPM_DEBUG)
            Serial.println(F("Button Up: LEFT_BACK_THREE"));
#endif
            break;
        case RIGHT_FRONT_ONE:
#if defined(USM_PPM_DEBUG)
            Serial.println(F("Button Up: RIGHT_FRONT_ONE"));
#endif
            break;
        case RIGHT_BACK_ONE:
#if defined(USM_PPM_DEBUG)
            Serial.println(F("Button Up: RIGHT_BACK_ONE"));
#endif
            break;
        case RIGHT_FRONT_TWO:
#if defined(USM_PPM_DEBUG)
            Serial.println(F("Button Up: RIGHT_FRONT_TWO"));
#endif
            break;
        case RIGHT_BACK_TWO:
#if defined(USM_PPM_DEBUG)
            Serial.println(F("Button Up: RIGHT_BACK_TWO"));
#endif
            break;
        case RIGHT_FRONT_THREE:
#if defined(USM_PPM_DEBUG)
            Serial.println(F("Button Up: RIGHT_FRONT_THREE"));
#endif
            break;
        case RIGHT_BACK_THREE:
#if defined(USM_PPM_DEBUG)
            Serial.println(F("Button Up: RIGHT_BACK_THREE"));
#endif
            break;
        default:
#if defined(USM_PPM_DEBUG)
            Serial.print(F("Unknown Button Up: "));
            Serial.println(but_id, HEX);
#endif
            break;
    }
}

void ThrustMasterPPM::OnButtonDn(uint8_t but_id){
   
#if defined(USM_PPM_DEBUG)
    Serial.print(F("Button Down id: "));
    Serial.println(but_id, HEX);
#endif

    switch (but_id) {
        case TRIGGER:
#if defined(USM_PPM_DEBUG)
            Serial.println(F("Button Down: TRIGGER"));
#endif
            break;
        case THUMB_BOTTOM:
#if defined(USM_PPM_DEBUG)
            Serial.println(F("Button Down: THUMB_BOTTOM"));
#endif
            break;
        case THUMB_LEFT:
#if defined(USM_PPM_DEBUG)
            Serial.println(F("Button Down: THUMB_LEFT"));
#endif
            break;
        case THUMB_RIGHT:
#if defined(USM_PPM_DEBUG)
            Serial.println(F("Button Down: THUMB_RIGHT"));
#endif
            break;
        case LEFT_FRONT_ONE:
#if defined(USM_PPM_DEBUG)
            Serial.println(F("Button Down: LEFT_FRONT_ONE"));
#endif
                throttle_safety = false;
            break;
        case LEFT_BACK_ONE:
#if defined(USM_PPM_DEBUG)
            Serial.println(F("Button Down: LEFT_BACK_ONE"));
#endif
                throttle_safety = true;
            break;
        case LEFT_FRONT_TWO:
#if defined(USM_PPM_DEBUG)
            Serial.println(F("Button Down: LEFT_FRONT_TWO"));
#endif
            break;
        case LEFT_BACK_TWO:
#if defined(USM_PPM_DEBUG)
            Serial.println(F("Button Down: LEFT_BACK_TWO"));
#endif
            break;
        case LEFT_FRONT_THREE:
#if defined(USM_PPM_DEBUG)
            Serial.println(F("Button Down: LEFT_FRONT_THREE"));
#endif
            break;
        case LEFT_BACK_THREE:
#if defined(USM_PPM_DEBUG)
            Serial.println(F("Button Down: LEFT_BACK_THREE"));
#endif
            break;
        case RIGHT_FRONT_ONE:
#if defined(USM_PPM_DEBUG)
            Serial.println(F("Button Down: RIGHT_FRONT_ONE"));
#endif
            break;
        case RIGHT_BACK_ONE:
#if defined(USM_PPM_DEBUG)
            Serial.println(F("Button Down: RIGHT_BACK_ONE"));
#endif
            break;
        case RIGHT_FRONT_TWO:
#if defined(USM_PPM_DEBUG)
            Serial.println(F("Button Down: RIGHT_FRONT_TWO"));
#endif
            break;
        case RIGHT_BACK_TWO:
#if defined(USM_PPM_DEBUG)
            Serial.println(F("Button Down: RIGHT_BACK_TWO"));
#endif
            break;
        case RIGHT_FRONT_THREE:
#if defined(USM_PPM_DEBUG)
            Serial.println(F("Button Down: RIGHT_FRONT_THREE"));
#endif
            break;
        case RIGHT_BACK_THREE:
#if defined(USM_PPM_DEBUG)
            Serial.println(F("Button Down: RIGHT_BACK_THREE"));
#endif
            break;
        default:
#if defined(USM_PPM_DEBUG)
            Serial.print(F("Unknown Button Down: "));
            Serial.println(but_id, HEX);
#endif
            break;
    }
}
