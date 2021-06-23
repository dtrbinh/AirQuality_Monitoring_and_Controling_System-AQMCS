// TouchScreen_Calibr_native for MCUFRIEND UNO Display Shields
// adapted by David Prentice
// for Adafruit's <TouchScreen.h> Resistive Touch Screen Library
// from Henning Karlsen's original UTouch_Calibration program.
// Many Thanks.
#include <UTFTGLUE.h>              //use GLUE class and constructor
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#define PORTRAIT  0
#define LANDSCAPE 1
int x,y;
#define TOUCH_ORIENTATION  PORTRAIT
#define TITLE "TouchScreen.h GFX Calibration"

#include <Adafruit_GFX.h>
// MCUFRIEND UNO shield shares pins with the TFT.
#if defined(ESP32)
int XP = 27, YP = 4, XM = 15, YM = 14;  //most common configuration
#else
int XP = 6, YP = A1, XM = A2, YM = 7;  //most common configuration
//int XP = 7, YP = A2, XM = A1, YM = 6;  //most common configuration
#endif
#include <TouchScreen.h>         //Adafruit Library
TouchScreen ts(XP, YP, XM, YM, 300);   //re-initialised after diagnose
TSPoint tp;                            //global point
//#include "TouchScreen_kbv.h"         //my hacked version
//TouchScreen_kbv ts(XP, YP, XM, YM, 300);   //re-initialised after diagnose
//TSPoint_kbv tp;                            //global point
#include <TouchScreen.h>
UTFTGLUE myGLCD(0,A2,A1,A3,A4,A0);
#define WHITE 0xFFFF
#define RED   0xF800
#define GRAY  0x8410
#define BLACK 0x0000
void setup(){ 
  Serial.begin(115200);
// Setup the LCD
  myGLCD.InitLCD();
  myGLCD.setFont(BigFont);
  //myTouch.InitTouch();
  //myTouch.setPrecision(PREC_MEDIUM);
  #define BLACK   0x0000
  #define BLUE    0x001F
  #define RED     0xF800
  #define GREEN   0x07E0
  #define CYAN    0x07FF
  #define MAGENTA 0xF81F
  #define YELLOW  0xFFE0
  #define WHITE   0xFFFF
  #define GREY    0x8410
  #define ORANGE  0xE880
  #define TS_MINX 125
  #define TS_MINY 85
  #define TS_MAXX 965
  #define TS_MAXY 905
  
} 
void readResistiveTouch()
{
    tp = ts.getPoint();
    pinMode(YP, OUTPUT);      //restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);  //because TFT control pins
    digitalWrite(XM, HIGH);
    Serial.println("tp.x=" + String(tp.x) + ", tp.y=" + String(tp.y) + ", tp.z =" + String(tp.z));
}
void loop()
{
   myGLCD.clrScr();
   myGLCD.fillScreen(BLUE);
   readResistiveTouch();
   Serial.println("tp.x=" + String(x) + ", tp.y=" + String(y) + ", tp.z =" + String(tp.z));
   delay(3000);
}
