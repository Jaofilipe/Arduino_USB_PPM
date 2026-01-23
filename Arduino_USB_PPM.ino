#include "ARDUINO_USB_PPM.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

USB Usb;

LiquidCrystal_I2C MyLCD(0x27, 20, 4);

ThrustMasterPPM_Display TMaster_Joy( ppm, &Usb, &MyLCD);

bool conn_status = true;    //flag to clear lcd on first usb disconnection

void setup() {

  pinMode(MENU_ENTER_PIN, INPUT_PULLUP); //pin for ok menu button

  MyLCD.init();
  MyLCD.backlight();
  MyLCD.setCursor(0, 0);
  MyLCD.print("    Initializing");

  Serial.begin(9600);                             //initialize serial communication

#if !defined(__MIPSEL__)
  while (!Serial);                                  // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif

Serial.println(F("\r\nThrustMaster T.16000M FCS Joystick Example Started"));
  
  setup_PPM();

  Serial.println(F("\r\n PPM Started \n"));

  MyLCD.setCursor(0, 2);
  MyLCD.print("    PPM  Started");

if (Usb.Init() == -1) {
  Serial.print(F("\r\n Check USB Board, Init Failed"));
  MyLCD.setCursor(0, 1);
  MyLCD.print("  USB Init Failed!");
  MyLCD.setCursor(0, 2);
  MyLCD.print("Press Reset to Retry");
  while (1); // Halt

} else {

  Serial.println(F("\r\n USB Board Init Success"));
  MyLCD.setCursor(0, 1);
  MyLCD.print("  USB Init Success");
}

  delay(800);
  MyLCD.clear();
 
  conn_status = true;    //flag to clear lcd on first usb disconnection
}

void loop() {

label_loop_begin:           //label marker for jump to always retry connecting to usb
  
  Usb.Task();

  if (!TMaster_Joy.connected()) {  //if usb is not connected or lost conn. 
   if (conn_status == true) {      //and usb connection was present before
  
    Serial.println("No Connection to Joystick");  //Joystick not connected on serial port
    MyLCD.clear();                                //clear lcd to display conn. lost message
    MyLCD.setCursor(0, 1);
    MyLCD.print("No Conn. to Joystick");
    failsafe_PPM();                               //set ppm values to failsafe ppm
    conn_status = false;                          //reset usb connection flag
   }
   delay(10);
   goto label_loop_begin;  //bypass remaining code with this jump to the beginning
  
  }else{                            //if usb is now connected 
    if (conn_status == false) {     // and usb connection status was lost 

      MyLCD.clear();                //clear lcd text for new info        
      conn_status = true;           //set usb connection flag
    }
  }
  
  evaluate_menu(&MyLCD);
  handle_display();

}

void handle_display(){
  
    switch (display_states)
  {
  case Display_Idle:
    printIdle();
    break;
  case Display_All_Channels:
    printFirstChannels();
    break;
  case Display_Joy_Axis:
    printJoyAxis();
    break;
  case Display_Joy_Buttons:
    printJoyButtons();
    break;
  case Display_Failsafe_Values:
    printFailsafe();
    break;
  
  default:
    break;
  }

}

void evaluate_menu(LiquidCrystal_I2C *display){

  static uint16_t horizontal = 0;
  static uint16_t vertical = 0;
  static bool button = false;

  vertical  = analogRead(MENU_VERT_ANALOG)>>2;
  horizontal   = analogRead(MENU_HORI_ANALOG)>>2;
  button = digitalRead(MENU_ENTER_PIN);

#if defined(USM_PPM_DEBUG_MENU)
  Serial.print("Horizontal: ");
  Serial.print(horizontal);
  Serial.print("  Vertical: ");
  Serial.print(vertical);
  Serial.print("  botao: ");
  Serial.println(button);
#endif

  union menu_buttons {
    struct{
        uint8_t up : 1;
        uint8_t down : 1;
        uint8_t right : 1;
        uint8_t left : 1;
        uint8_t ok : 1;
    } __attribute__((packed));
    uint8_t menukeys : 5;
} __attribute__((packed));

menu_buttons new_menu;
static menu_buttons old_menu;
menu_buttons menu_state_pressed;

new_menu.up =    (vertical >= 255-10) ? 1:0;
new_menu.down =  (vertical <= (0+10)) ? 1:0;
new_menu.left =  (horizontal >= 255-10 ) ? 1:0;
new_menu.right = (horizontal <= (0+10) ) ? 1:0;
new_menu.ok = button;

menu_state_pressed.menukeys = old_menu.menukeys ^ new_menu.menukeys;

old_menu.menukeys = new_menu.menukeys;

  switch (display_states)
  {
  case Display_Idle:
    if (menu_state_pressed.up && new_menu.up)
    {
      display_states = Display_Failsafe_Values;
      display->clear();
    } else if (menu_state_pressed.down && new_menu.down){
      display_states = Display_All_Channels;
      display->clear();
    } else {}
    
    break;
  case Display_All_Channels:
    if (menu_state_pressed.up && new_menu.up){
      display_states = Display_Idle;
      display->clear();
    } else if (menu_state_pressed.down && new_menu.down){
      display_states = Display_Joy_Axis;
      display->clear();
    } else {}
    break;
  case Display_Joy_Axis:
    if (menu_state_pressed.up && new_menu.up){
      display_states = Display_All_Channels;
      display->clear();
    } else if (menu_state_pressed.down && new_menu.down){
      display_states = Display_Joy_Buttons;
      display->clear();
    } else {}
    break;
  case Display_Joy_Buttons:
    if (menu_state_pressed.up && new_menu.up){
      display_states = Display_Joy_Axis;
      display->clear();
    } else if (menu_state_pressed.down && new_menu.down){
      display_states = Display_Failsafe_Values;
      display->clear();
    } else {}
    break;
  case Display_Failsafe_Values:
    if (menu_state_pressed.up && new_menu.up){
      display_states = Display_Joy_Buttons;
      display->clear();
    } else if (menu_state_pressed.down && new_menu.down){
      display_states = Display_Idle;
      display->clear();
    } else {}
    break;
  
  
  default:
    break;
  }
}

void printIdle(void){

    MyLCD.setCursor(0, YAW_AXIS);
    MyLCD.print(YAW_AXIS+1);
    MyLCD.print(" Y ");
    MyLCD.print(ppm[YAW_AXIS]);
    MyLCD.print(" ");

    MyLCD.setCursor(0, PITCH_AXIS);
    MyLCD.print(PITCH_AXIS+1);
    MyLCD.print(" P ");
    MyLCD.print(ppm[PITCH_AXIS]);
    MyLCD.print(" ");

    MyLCD.setCursor(0, ROLL_AXIS);
    MyLCD.print(ROLL_AXIS+1);
    MyLCD.print(" R ");
    MyLCD.print(ppm[ROLL_AXIS]);
    MyLCD.print(" ");
  
    MyLCD.setCursor(0, THROTTLE_AXIS);
    MyLCD.print(THROTTLE_AXIS+1);
    MyLCD.print(" T ");
    MyLCD.print(ppm[THROTTLE_AXIS]);
    MyLCD.print(" ");
    MyLCD.setCursor(10, THROTTLE_AXIS);
    MyLCD.print(TMaster_Joy.throttle_safety?"Safe: ON  ":"Safe: OFF ");

}

void printFirstChannels(void){
  for (uint8_t i = 0; i < 8; i++)
  {
    MyLCD.setCursor(((i>>2)*10), i%4); // 10 char spacing by using most sig. bit and 2 columns by i%4 
    MyLCD.print("Ch0");
    MyLCD.print(i+1);
    MyLCD.print(" ");
    MyLCD.print(ppm[i]);
    MyLCD.print(" ");
  }
}
void printLastChannels(void){
  char buffer[3];   // 2 digits + null terminator
  for (uint8_t i = 0; i < (CHANNEL_NUMBER % 8); i++)
  {
    MyLCD.setCursor(((i>>2)*10), i%4);
    MyLCD.print("Ch");
    sprintf(buffer, "%02d", i+1+8);
    MyLCD.print(buffer);
    MyLCD.print("  ");
    MyLCD.print(ppm[i]);
  }
}

void printJoyAxis(){
    char buffer[6];   // 5 digits + null terminator

    MyLCD.setCursor(0, 0);
    MyLCD.print("X Axis ");
    sprintf(buffer, "%05d", TMaster_Joy.TmJoyData.Axis.Xaxis);
    MyLCD.print(buffer);

    MyLCD.setCursor(0, 1);
    MyLCD.print("Y Axis ");
    sprintf(buffer, "%05d", TMaster_Joy.TmJoyData.Axis.Yaxis);
    MyLCD.print(buffer);

    MyLCD.setCursor(0, 2);
    MyLCD.print("RZ Axis  ");
    sprintf(buffer, "%03d", TMaster_Joy.TmJoyData.Axis.RZaxis);
    MyLCD.print(buffer);

    MyLCD.setCursor(0, 3);
    MyLCD.print("Slider   ");
    sprintf(buffer, "%03d",TMaster_Joy.TmJoyData.Axis.Slider);
    MyLCD.print(buffer);
}

void printJoyButtons(){
  char buffer[3];   // 2 digits + null terminator

  for (uint8_t i = 0; i < 16; i++)
  {
      MyLCD.setCursor(((i>>2)*5), i%4); // 5 char spacing by using most sig. bit and n columns by i%4 
      sprintf(buffer, "%02d",(i+1));
      MyLCD.print(buffer);
      MyLCD.print("-");
      MyLCD.print(((TMaster_Joy.TmJoyData.Buttons.all_buttons>>i) & 1u));
  }
}

void printFailsafe(){
    MyLCD.setCursor(10,0);
    MyLCD.print("Failsafe");
    MyLCD.setCursor(0,0);
  for (uint8_t i = 0; i < 4; i++){
    MyLCD.setCursor(0,i);
    MyLCD.print(i+1);
    MyLCD.print("-");
    MyLCD.print(ppm[i]);
    MyLCD.print(" ");
  }
  for (uint8_t i = 4; i < 6; i++){
    MyLCD.setCursor(7,i-2);
    MyLCD.print(i+1);
    MyLCD.print("-");
    MyLCD.print(ppm[i]);
    MyLCD.print(" ");
  }
  for (uint8_t i = 6; i < 8; i++){
    MyLCD.setCursor(14,i-4);
    MyLCD.print(i+1);
    MyLCD.print("-");
    MyLCD.print(ppm[i]);
    MyLCD.print(" ");
  }
}