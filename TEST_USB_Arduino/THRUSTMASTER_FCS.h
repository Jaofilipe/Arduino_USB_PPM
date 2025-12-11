#ifndef __THRUSTMASTER_FCS_h__
#define __THRUSTMASTER_FCS_h__

#include <hiduniversal.h>

//#define DEBUG_JOYSTICK

#define THRUSTMASTER_VID     0x044F
#define THRUSTMASTER_FCS_PID 0xB10A

enum DPADEnum {
        DPAD_UP         = 0x0,
        DPAD_UP_RIGHT   = 0x1,
        DPAD_RIGHT      = 0x2,
        DPAD_RIGHT_DOWN = 0x3,
        DPAD_DOWN       = 0x4,
        DPAD_DOWN_LEFT  = 0x5,
        DPAD_LEFT       = 0x6,
        DPAD_LEFT_UP    = 0x7,
        DPAD_OFF        = 0xF,
};

enum ButtonsEnum{
        TRIGGER             = 0x00,
        THUMB_BOTTOM        = 0x01,
        THUMB_LEFT          = 0x02,
        THUMB_RIGHT         = 0x03,
        LEFT_FRONT_OUTSIDE  = 0x04,
        LEFT_FRONT_MIDDLE   = 0x05,
        LEFT_FRONT_INSIDE   = 0x06,
        LEFT_BACK_INSIDE    = 0x07,
        LEFT_BACK_MIDDLE    = 0x08,
        LEFT_BACK_OUTSIDE   = 0x09,
        RIGHT_FRONT_OUTSIDE = 0x0A,
        RIGHT_FRONT_MIDDLE  = 0x0B,
        RIGHT_FRONT_INSIDE  = 0x0C,
        RIGHT_BACK_INSIDE   = 0x0D,
        RIGHT_BACK_MIDDLE   = 0x0E,
        RIGHT_BACK_OUTSIDE  = 0x0F,
};

union TMButtonsData {
        struct {
                uint8_t trigger         : 1;
                uint8_t thumb_bottom    : 1;
                uint8_t thumb_left      : 1;
                uint8_t thumb_right     : 1;
                
                uint8_t left_front_outside      : 1;
                uint8_t left_front_middle       : 1;
                uint8_t left_front_inside       : 1;
                uint8_t left_back_inside        : 1;
                uint8_t left_back_middle        : 1;
                uint8_t left_back_outside       : 1;

                uint8_t right_front_outside     : 1;
                uint8_t right_front_middle      : 1;
                uint8_t right_front_inside      : 1;
                uint8_t right_back_inside       : 1;
                uint8_t right_back_middle       : 1;
                uint8_t right_back_outside      : 1;
                
        } __attribute__((packed));
        uint32_t all_buttons : 16;
} __attribute__((packed));

union TMDpadData {
        uint8_t dpad_data : 4;
} __attribute__((packed));

struct TMAxisData {
        uint16_t Xaxis : 16;    // 0–0x3FFF Range [0:1023] i.e. 14 bits
        uint16_t Yaxis : 16;    // 0–0x3FFF Range [0:1023] i.e. 14 bits
        uint8_t  RZaxis : 8;    // 0-0x00FF Range [0:254]  8 bits
        uint8_t  Slider : 8;    // 0-0x00FF Range [0:254]  8 bits
} __attribute__((packed));

struct TMJoyData {
        TMButtonsData Buttons;
        TMDpadData Dpad;
        TMAxisData Axis;
} __attribute__((packed));

class THRUSTMASTER_FCS : public HIDUniversal {

public:
        THRUSTMASTER_FCS(USB *p) : HIDUniversal(p) {};

        bool connected();               //function to return joystick is connected to usb board

        TMJoyData TmJoyData;              //register to store current joystick values
        TMButtonsData buttonClickState;  //flag register to store "button state changed" flags 

        TMButtonsData oldButtonState;
        TMDpadData oldDpadState;
        TMAxisData oldAxisState;

        virtual void OnGamePadChanged(const TMAxisData *axis __attribute__((unused))) {
                return;
        };
        virtual void OnHatSwitch(uint8_t hat __attribute__((unused))) {
                return;
        };
        virtual void OnButtonUp(uint8_t but_id __attribute__((unused))) {
                return;
        };
        virtual void OnButtonDn(uint8_t but_id __attribute__((unused))) {
                return;
        };

private:
        void ParseHIDData(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf); // Called by the HIDUniversal library
        uint8_t OnInitSuccessful();    //bool to get the status of initialization
};

#endif
