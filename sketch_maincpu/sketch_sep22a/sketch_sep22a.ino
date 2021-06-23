float t,h,gas,CO,flame;
#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
DFRobotDFPlayerMini myDFPlayer;
#include <SimpleKalmanFilter.h>
SimpleKalmanFilter bo_loc(2, 2, 0.001);
#include<DHT.h>
#include "SharpGP2Y10.h"           
const int DHTPIN = 35;       //Đọc dữ liệu từ DHT11 ở chân 6 digtal trên mạch Arduino
const int DHTTYPE = DHT11;  //Khai báo loại cảm biến
String  gui,X,Y,Z,Q,W,O,str1,str2,str3,str4,str5;
DHT dht(DHTPIN, DHTTYPE);
int voPin = A13;
int ledPin = 39;
float dustDensity = 0.0;
SharpGP2Y10 dustSensor(voPin, ledPin);
#include <URTouch.h>
#include <URTouchCD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include <TouchScreen.h>
#include <UTFTGLUE.h>              //use GLUE class and constructor
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#define MINPRESSURE 200
#define MAXPRESSURE 1000
const int XP = 8, XM = A2, YP = A3, YM = 9; //ID=0x9341
const int TS_LEFT = 94, TS_RT = 877, TS_TOP = 83, TS_BOT = 898;//Portrait
//const int TS_LEFT = 83, TS_RT = 898, TS_TOP = 877, TS_BOT = 94;//Landscape

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Adafruit_GFX_Button on_btn, off_btn;

int pixel_x, pixel_y;     //Touch_getXY() updates global vars
UTFTGLUE myGLCD(0,A2,A1,A3,A4,A0); //all dummy args
//URTouch  myTouch( 6, 5, 4, 3, 2);
//UTFT myGLCD(ITDB32S,38,39,40,41);
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
#define RGB(r, g, b) (((r&0xF8)<<8)|((g&0xFC)<<3)|(b>>3))
#define GREY      RGB(127, 127, 127)
#define DARKGREY  RGB(64, 64, 64)
#define TURQUOISE RGB(0, 128, 128)
#define PINK      RGB(255, 128, 192)
#define OLIVE     RGB(128, 128, 0)
#define PURPLE    RGB(128, 0, 128)
#define AZURE     RGB(0, 128, 255)
#define ORANGE    RGB(255,128,64)

void Delay(float delayTime, void (func1)(), void (func2)(), void func3()){
    unsigned long endTime = millis() + delayTime;
    while(millis() < endTime)
    {
      func1();
      func2();
      func3();
      //while(millis() < endTime){};  //Xóa dòng này nếu muốn lặp hàm func, giữ nếu muốn chạy hàm func 1 lần.
    }
}
 
bool Touch_getXY(void)
{
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);      //restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);   //because TFT control pins
    digitalWrite(XM, HIGH);
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
        pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width()); //.kbv makes sense to me
        pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
    }else{if((pixel_x>1) and (pixel_x<62) and(pixel_y>0) and (pixel_y<100)){
      pixel_x = 0;pixel_x = 0;
    }
    
    }
          
    return pressed;
    
}

void setup() {
  Serial.begin(115200); //Giao tiếp máy tính
  Serial1.begin(9600);//Giao tiếp DFplayer
  myDFPlayer.begin(Serial1); myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  myDFPlayer.volume(0);//0-30
  Serial2.begin(115200); //Giao tiếp nodeMCU
 
  dht.begin();         // Khởi động cảm biến
  pinMode(27,OUTPUT);
  pinMode(31,OUTPUT);
  myGLCD.InitLCD();
  myGLCD.setFont(BigFont);
  //Home
  myGLCD.clrScr();
  myGLCD.setColor(MAGENTA);myGLCD.setBackColor(CYAN);
  myGLCD.fillScreen(CYAN);
  myGLCD.setTextSize(2);
  myGLCD.print("SAN PHAM CUOC THI KHKT",30,10);
  myGLCD.print("CAP TINH 2018-2019",60,30);
  myGLCD.print(" Nhom tac gia : ",LEFT,50 );
  myGLCD.print("  DO TRAN BINH",LEFT,70);
  myGLCD.print("  TA HOANG AI VI",LEFT,90);
  myGLCD.print(" Lop 11Ly-THPT Chuyen", LEFT, 120);
  myGLCD.print(" Le Khiet", LEFT, 140);
  myGLCD.setColor(RED);
  myGLCD.print("Tu chuyen trang sau 5s...",LEFT,170);delay(5000);
  //Giamsat
  myGLCD.clrScr();
  myGLCD.setTextSize(2);
  myGLCD.fillScreen(BLACK);
  myGLCD.setColor(CYAN);
  myGLCD.setBackColor(BLACK);
  myGLCD.print("CHAT LUONG KHONG KHI",40,10);
  myGLCD.setColor(WHITE);
  myGLCD.print("Nhiet do    :", LEFT, 40);myGLCD.print("C",250,40);
  myGLCD.print("Do am       :", LEFT, 60);myGLCD.print("%",250,60);  
  myGLCD.print("Nong do CO  :", LEFT, 80);myGLCD.print("ppm",250,80);
  myGLCD.print("Nong do gas :", LEFT, 100);myGLCD.print("ppm",250,100);
  myGLCD.print("Nong do bui :", LEFT, 120);myGLCD.print("ug/m3",250,120);
  myGLCD.print("Lua        :", LEFT, 140);myGLCD.print("dvi",250,140);
//Tạo nút nhấn 
  myGLCD.setTextSize(3);
  myGLCD.setBackColor(GREEN);
  myGLCD.setColor(GREEN);
  myGLCD.fillRect(1,180,100,239);
  myGLCD.setColor(RED);
  myGLCD.print("HOME", 15,200);
  
  myGLCD.setTextSize(2);
  myGLCD.setBackColor(GREEN);
  myGLCD.setColor(GREEN);
  myGLCD.fillRect(220,180,319,239);
  myGLCD.setColor(RED);
  myGLCD.print("SEND",245,190);myGLCD.print("DATA",245,210) ;

  myGLCD.setTextSize(2);
  myGLCD.setBackColor(GREEN);
  myGLCD.setColor(GREEN);
  myGLCD.fillRect(120,180,200,239);
  myGLCD.setColor(RED);
  myGLCD.print("GET",140,190);myGLCD.print("DATA",140,210); 
}

void thongtin()
{ 
  h = dht.readHumidity();    //Đọc độ ẩm
  t = dht.readTemperature(); //Đọc nhiệt độ
  //-----------------------------------
  gas = analogRead(A11);   //Cảm biến gas ở chân A11

  //-----------------------------------
  CO = analogRead(A6);   //Cảm biến CO ở chân A6
 
  //-----------------------------------
  flame = analogRead(A15);
  //-----------------------------------
   dustDensity = 1000*dustSensor.getDustDensity();    //Cảm biến bụi
   dustDensity = bo_loc.updateEstimate(dustDensity); 
    String temp = String(t);
   String hum = String(h);
   String val1 = String(gas);
   String val2 = String(CO);
   String dust = String(dustDensity);
   String fire = String(flame);

    gui = "O"+temp+"X"+hum+"Y"+val1+"Z"+val2+"W"+dust+"Q"  ;Serial2.print(gui);
}

void giamsat()
{
  
// Clear the screen and draw the frame
   String temp = String(t);
   String hum = String(h);
   String val1 = String(gas);
   String val2 = String(CO);
   String dust = String(dustDensity);
   String fire = String(flame);
  myGLCD.setTextSize(2);myGLCD.setColor(WHITE);myGLCD.setBackColor(BLACK);
  myGLCD.print(temp,160,40); 
  myGLCD.print(hum,160,60); 
  myGLCD.print(val2,160,80); 
  myGLCD.print(val1,160,100); 
  myGLCD.print(dust,160,120); 
  myGLCD.print(fire,160,140);
  
}
void HomeScreen()
{ 
  setup();
  if((pixel_x>1) and (pixel_x<62) and (pixel_y>215) and (pixel_y<320)){ctrlScreen();};
  //delay(500);
  }
  
void ctrlScreen()
{ //myGLCD.clrScr();
  myGLCD.setTextSize(2);
  myGLCD.fillScreen(BLACK);
  myGLCD.setColor(CYAN);
  myGLCD.setBackColor(BLACK);
  myGLCD.print("CHAT LUONG KHONG KHI",40,10);
  myGLCD.setColor(WHITE);
  myGLCD.print("Nhiet do    :", LEFT, 40);myGLCD.print("C",250,40);
  myGLCD.print("Do am       :", LEFT, 60);myGLCD.print("%",250,60);  
  myGLCD.print("Nong do CO  :", LEFT, 80);myGLCD.print("ppm",250,80);
  myGLCD.print("Nong do gas :", LEFT, 100);myGLCD.print("ppm",250,100);
  myGLCD.print("Nong do bui :", LEFT, 120);myGLCD.print("ug/m3",250,120);
  myGLCD.print("Lua        :", LEFT, 140);myGLCD.print("dvi",250,140);
//Tạo nút nhấn 
  myGLCD.setTextSize(3);
  myGLCD.setBackColor(GREEN);
  myGLCD.setColor(GREEN);
  myGLCD.fillRect(1,180,100,239);
  myGLCD.setColor(RED);
  myGLCD.print("HOME", 15,200);
  
  myGLCD.setTextSize(2);
  myGLCD.setBackColor(GREEN);
  myGLCD.setColor(GREEN);
  myGLCD.fillRect(220,180,319,239);
  myGLCD.setColor(RED);
  myGLCD.print("SEND",245,190);myGLCD.print("DATA",245,210) ;

  myGLCD.setTextSize(2);
  myGLCD.setBackColor(GREEN);
  myGLCD.setColor(GREEN);
  myGLCD.fillRect(120,180,200,239);
  myGLCD.setColor(RED);
  myGLCD.print("GET",140,190);myGLCD.print("DATA",140,210);
  String temp = String(t);
   String hum = String(h);
   String val1 = String(gas);
   String val2 = String(CO);
   String dust = String(dustDensity);
   String fire = String(flame);
  myGLCD.setTextSize(2);myGLCD.setColor(WHITE);myGLCD.setBackColor(BLACK);
  myGLCD.print(temp,160,40); 
  myGLCD.print(hum,160,60); 
  myGLCD.print(val2,160,80); 
  myGLCD.print(val1,160,100); 
  myGLCD.print(dust,160,120); 
  myGLCD.print(fire,160,140);
  //delay(1000);
}

void loop() 
{
  myDFPlayer.volume(20);//0-30
  thongtin();
    //---Hàm cảnh báo-------------------------  
if ((dustDensity>70)|| (CO>170) || (gas>400)) {
    digitalWrite(31,LOW); //|| ((CO>170)and(CO<300)) || ((gas>400)and(gas<700)) 
    myDFPlayer.play(1);
    Delay(15000,thongtin,giamsat,ctrlScreen);}
  else{digitalWrite(31,HIGH);thongtin();} //state off 
                    
if((dustDensity>200) || (CO>300) || (gas>700)) {
    digitalWrite(27,LOW);//|| (CO>300) || (gas>700)
    myDFPlayer.play(2);
    Delay(20000,thongtin,giamsat,ctrlScreen); }
  else{digitalWrite(27,HIGH);thongtin();} //state off

   //Serial.println(gui);
      bool down = Touch_getXY();
  on_btn.press(down && on_btn.contains(pixel_x, pixel_y));
  off_btn.press(down && off_btn.contains(pixel_x, pixel_y));
  if((pixel_x>1) and (pixel_x<62)){ 
    if ((pixel_y>0) and (pixel_y<100)){HomeScreen();}
    if ((pixel_y>120) and (pixel_y<200)){giamsat();}
    if ((pixel_y>215) and (pixel_y<320)){ctrlScreen();}
  }

 Serial2.print(gui);Serial.println(gui);
}
