
#include "ARDUINO_USB_PPM.h"

uint8_t display_states = Display_Idle;

ThrustMasterPPM::ThrustMasterPPM(uint16_t *ppm, USB *usb ) : THRUSTMASTER_FCS(usb) , ppm_array(ppm),  throttle_safety(true){

        //do something when initializing
};

void ThrustMasterPPM::OnGamePadChanged(const TMAxisData *axis){

        ppm_array[YAW_AXIS]      = map(axis->RZaxis,0x00,0xFF,1000,2000); 
        ppm_array[ROLL_AXIS]     = map(axis->Xaxis ,0x00,0x3FFF,1000,2000);
        ppm_array[THROTTLE_AXIS] = ((!throttle_safety) ? (map(axis->Slider,0x00,0xFF,1000,2000)):(1000));
        ppm_array[PITCH_AXIS]    = map(axis->Yaxis ,0x00,0x3FFF,1000,2000); 

#if defined(USM_PPM_DEBUG_AXIS)
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

#if defined(USM_PPM_DEBUG_BUTTONS)
    Serial.println(F("Hat Switch: "));
#endif
        switch (hat)
        {
        case DPAD_UP:
#if defined(USM_PPM_DEBUG_BUTTONS)
                Serial.println(F("Up"));
#endif
                handleHatUp();
                break;
        case DPAD_UP_RIGHT:
#if defined(USM_PPM_DEBUG_BUTTONS)
                Serial.println(F("UP & right"));
#endif
                break;
        case DPAD_RIGHT:
#if defined(USM_PPM_DEBUG_BUTTONS)
                Serial.println(F("Right"));
#endif
                handleHatRight();
                break;
        case DPAD_RIGHT_DOWN:
#if defined(USM_PPM_DEBUG_BUTTONS)
                Serial.println(F("Right & down"));
#endif
                break;
        case DPAD_DOWN:
#if defined(USM_PPM_DEBUG_BUTTONS)
                Serial.println(F("Down"));
#endif                
                handleHatDown();
                break;
        case DPAD_DOWN_LEFT:
#if defined(USM_PPM_DEBUG_BUTTONS)
                Serial.println(F("Down & left"));
#endif
                break;
        case DPAD_LEFT:
#if defined(USM_PPM_DEBUG_BUTTONS)
                Serial.println(F("Left"));
#endif
                handleHatLeft();
                break;
        case DPAD_LEFT_UP:
#if defined(USM_PPM_DEBUG_BUTTONS)
                Serial.println(F("Left & up"));
#endif
                break;
        case DPAD_OFF:
#if defined(USM_PPM_DEBUG_BUTTONS)

#endif
                break;
        
        default:
#if defined(USM_PPM_DEBUG_BUTTONS)
                Serial.print(F("Unknown state: "));
                Serial.println(TmJoyData.Dpad.dpad_data, HEX);
#endif
                break;
        }
}

void ThrustMasterPPM::OnButtonUp(uint8_t but_id){

#if defined(USM_PPM_DEBUG_BUTTONS)
    Serial.print(F("Button Up id: "));
    Serial.println(but_id, HEX);
#endif

    switch (but_id) {
        case TRIGGER:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.println(F("Button Up: TRIGGER"));
#endif
            break;
        case THUMB_BOTTOM:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.println(F("Button Up: THUMB_BOTTOM"));
#endif
            break;
        case THUMB_LEFT:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.println(F("Button Up: THUMB_LEFT"));
#endif
            break;
        case THUMB_RIGHT:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.println(F("Button Up: THUMB_RIGHT"));
#endif
            break;
        case LEFT_FRONT_OUTSIDE:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.println(F("Button Up: LEFT_FRONT_OUTSIDE"));
#endif
            break;
        case LEFT_FRONT_MIDDLE:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.println(F("Button Up: LEFT_FRONT_MIDDLE"));
#endif
            break;
        case LEFT_FRONT_INSIDE:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.println(F("Button Up: LEFT_FRONT_INSIDE"));
#endif
            break;
        case LEFT_BACK_INSIDE:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.println(F("Button Up: LEFT_BACK_INSIDE"));
#endif
            break;
        case LEFT_BACK_MIDDLE:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.println(F("Button Up: LEFT_BACK_MIDDLE"));
#endif
            break;
        case LEFT_BACK_OUTSIDE:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.println(F("Button Up: LEFT_BACK_OUTSIDE"));
#endif
            break;
        case RIGHT_FRONT_OUTSIDE:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.println(F("Button Up: RIGHT_FRONT_OUTSIDE"));
#endif
            break;
        case RIGHT_FRONT_MIDDLE:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.println(F("Button Up: RIGHT_FRONT_MIDDLE"));
#endif
            break;
        case RIGHT_FRONT_INSIDE:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.println(F("Button Up: RIGHT_FRONT_INSIDE"));
#endif
            break;
        case RIGHT_BACK_INSIDE:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.println(F("Button Up: RIGHT_BACK_INSIDE"));
#endif
            break;
        case RIGHT_BACK_MIDDLE:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.println(F("Button Up: RIGHT_BACK_MIDDLE"));
#endif
            break;
        case RIGHT_BACK_OUTSIDE:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.println(F("Button Up: RIGHT_BACK_OUTSIDE"));
#endif
            break;
        default:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.print(F("Unknown Button Up: "));
            Serial.println(but_id, HEX);
#endif
            break;
    }
}

void ThrustMasterPPM::OnButtonDn(uint8_t but_id){
   
#if defined(USM_PPM_DEBUG_BUTTONS)
    Serial.print(F("Button Down id: "));
    Serial.println(but_id, HEX);
#endif

    switch (but_id) {
        case TRIGGER:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.println(F("Button Down: TRIGGER"));
#endif
            break;
        case THUMB_BOTTOM:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.println(F("Button Down: THUMB_BOTTOM"));
#endif
            break;
        case THUMB_LEFT:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.println(F("Button Down: THUMB_LEFT"));
#endif
            break;
        case THUMB_RIGHT:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.println(F("Button Down: THUMB_RIGHT"));
#endif
            break;
        case LEFT_FRONT_OUTSIDE:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.println(F("Button Down: LEFT_FRONT_OUTSIDE"));
#endif
                throttle_safety = false;
            break;
        case LEFT_FRONT_MIDDLE:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.println(F("Button Down: LEFT_FRONT_MIDDLE"));
#endif
                throttle_safety = true;
            break;
        case LEFT_FRONT_INSIDE:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.println(F("Button Down: LEFT_FRONT_INSIDE"));
#endif
            break;
        case LEFT_BACK_INSIDE:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.println(F("Button Down: LEFT_BACK_INSIDE"));
#endif
            break;
        case LEFT_BACK_MIDDLE:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.println(F("Button Down: LEFT_BACK_MIDDLE"));
#endif
            break;
        case LEFT_BACK_OUTSIDE:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.println(F("Button Down: LEFT_BACK_OUTSIDE"));
#endif
            break;
        case RIGHT_FRONT_OUTSIDE:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.println(F("Button Down: RIGHT_FRONT_OUTSIDE"));
#endif
            break;
        case RIGHT_FRONT_MIDDLE:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.println(F("Button Down: RIGHT_FRONT_MIDDLE"));
#endif
            break;
        case RIGHT_FRONT_INSIDE:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.println(F("Button Down: RIGHT_FRONT_INSIDE"));
#endif
            break;
        case RIGHT_BACK_INSIDE:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.println(F("Button Down: RIGHT_BACK_INSIDE"));
#endif
            break;
        case RIGHT_BACK_MIDDLE:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.println(F("Button Down: RIGHT_BACK_MIDDLE"));
#endif
            break;
        case RIGHT_BACK_OUTSIDE:
#if defined(USM_PPM_DEBUG_BUTTONS)
            Serial.println(F("Button Down: RIGHT_BACK_OUTSIDE"));
#endif
            break;
        default:
#if defined(USM_PPM_DEBUG_BUTTONS )
            Serial.print(F("Unknown Button Down: "));
            Serial.println(but_id, HEX);
#endif
            break;
    }
}

ThrustMasterPPM_Display::ThrustMasterPPM_Display(uint16_t *ppm, USB *usb, LiquidCrystal_I2C *display ) : ThrustMasterPPM(ppm, usb), Lcd_I2C(display){

        //do something when initializing
};

void ThrustMasterPPM_Display::handleHatUp(void){
  display_states = (display_states <= 0 )?(Display_Max_State):(display_states-1);
  Lcd_I2C->clear();
}

void ThrustMasterPPM_Display::handleHatDown(void){
  display_states = (display_states >= Display_Max_State )?(0):(display_states+1);
  Lcd_I2C->clear();
}

void ThrustMasterPPM_Display::handleHatLeft(void){
  display_states = Display_Idle;
  Lcd_I2C->clear(); 
}

void ThrustMasterPPM_Display::handleHatRight(void){

}