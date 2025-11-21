#ifndef __THRUSTMASTER_FCS_h__
#define __THRUSTMASTER_FCS_h__

#include <hiduniversal.h>

//#define DEBUG_JOYSTICK

#define THRUSTMASTER_VID     0x044F
#define THRUSTMASTER_FCS_PID 0xB10A

enum DPADEnum {
        DPAD_UP = 0x0,
        DPAD_UP_RIGHT = 0x1,
        DPAD_RIGHT = 0x2,
        DPAD_RIGHT_DOWN = 0x3,
        DPAD_DOWN = 0x4,
        DPAD_DOWN_LEFT = 0x5,
        DPAD_LEFT = 0x6,
        DPAD_LEFT_UP = 0x7,
        DPAD_OFF = 0xF,
};

union TMButtonsData {
        struct {
                uint8_t trigger : 1;
                uint8_t thumb_bottom : 1;
                uint8_t thumb_left : 1;
                uint8_t thumb_right : 1;
                
                uint8_t left_front_one : 1;
                uint8_t left_back_one : 1;
                uint8_t left_front_two : 1;
                uint8_t left_back_two : 1;
                uint8_t left_front_three : 1;
                uint8_t left_back_three : 1;

                uint8_t right_front_one : 1;
                uint8_t right_back_one : 1;
                uint8_t right_front_two : 1;
                uint8_t right_back_two : 1;
                uint8_t right_front_three : 1;
                uint8_t right_back_three : 1;

                uint8_t dpad : 4;
                
        } __attribute__((packed));
        uint32_t all_buttons : 20;
} __attribute__((packed));

struct TMJoyData {
        union TMButtonsData Buttons;
        uint16_t Xaxis : 16;  // 0–0x3FFF Range [0:1023] i.e. 14 bits
        uint16_t Yaxis : 16;  // 0–0x3FFF Range [0:1023] i.e. 14 bits
        uint8_t  RZaxis : 8;             // 0-0x00FF Range [0:254]  8 bits
        uint8_t  Slider : 8;         // 0-0x00FF Range [0:254]  8 bits
} __attribute__((packed));

class THRUSTMASTER_FCS : public HIDUniversal {

public:
        THRUSTMASTER_FCS(USB *p) : HIDUniversal(p) {};

        bool connected();
        TMJoyData TmJoyData;
        TMButtonsData buttonClickState;

private:
        void ParseHIDData(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf); // Called by the HIDUniversal library
        uint8_t OnInitSuccessful();    //bool to get the status of initialization
};

#endif
