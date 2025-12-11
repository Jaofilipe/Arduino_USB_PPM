#include "ARDUINO_USB_PPM.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

USB Usb;

LiquidCrystal_I2C MyLCD(0x27, 20, 4);

ThrustMasterPPM_Display TMaster_Joy( ppm, &Usb, &MyLCD);

void setup() {

  MyLCD.init();
  MyLCD.backlight();
  MyLCD.setCursor(0, 0);
  MyLCD.print("    Initializing");

  Serial.begin(9600);                             //initialize serial communication

#if !defined(__MIPSEL__)
  while (!Serial);                                  // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif

Serial.println(F("\r\nThrustMaster T.16000M FCS Joystick Example Started"));
delay(200);

if (Usb.Init() == -1) {
  Serial.print(F("\r\n Check USB Board, Init Failed"));
  MyLCD.setCursor(0, 1);
  MyLCD.print("  USB Init Failed!");
  MyLCD.setCursor(0, 2);
  MyLCD.print("Press Reset to Retry");
  while (1); // Halt

} else {

  Serial.print(F("\r\n USB Board Init Success"));
  MyLCD.setCursor(0, 1);
  MyLCD.print("  USB Init Success");
}

  setup_PPM();

  delay(200);
  Serial.print(F("\r\n PPM Started \n"));

  MyLCD.setCursor(0, 2);
  MyLCD.print("    PPM  Started");
  delay(800);
  MyLCD.clear();


/*
  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  pinMode(4,INPUT_PULLUP);
  pinMode(5,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), handleUp, LOW);
  attachInterrupt(digitalPinToInterrupt(3), handleDown, LOW);
  attachInterrupt(digitalPinToInterrupt(4), handleOk, LOW);
  attachInterrupt(digitalPinToInterrupt(5), handleBack, LOW); */
    
}

void loop() {

  bool conn_status = true;    //flag to clear lcd on first usb disconnection
label_loop_begin:           //label marker for jump to always retry connecting to usb
  
  Usb.Task();

  if (!TMaster_Joy.connected()) {  //if usb is not connected or lost conn. 
   if (conn_status == true) {      //and usb connection was present before
  
    Serial.println("No Connection to Joystick");  //Joystick not connected on serial port
    MyLCD.clear();                                //clear lcd to display conn. lost message
    MyLCD.setCursor(0, 1);
    MyLCD.print("No Conn. to Joystick");
    conn_status = false;                          //reset usb connection flag
   }
    goto label_loop_begin;  //bypass remaining code with this jump to the beginning
  
  }else{                            //if usb is now connected 
    if (conn_status == false) {     // and usb connection status was lost 

      MyLCD.clear();                //clear lcd text for new info        
      conn_status = true;           //set usb connection flag
    }
  }
  
  switch (display_states)
  {
  case Display_Idle:
    printIdle();
    break;
  case Display_First_Channels:
    printFirstChannels();
    break;
  case Display_Last_Channels:
    printLastChannels();
    break;
  case Display_Joy_Axis:
    printJoyAxis();
    break;
  case Display_Joy_Buttons:
    printJoyButtons();
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