#include <Firebase.h>
#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
String gui;
#define WIFI_SSID "Hoang Huy"                                                                                
#define WIFI_PASSWORD "0905547819" 

#define FIREBASE_HOST"hethonggiamsatvacanhbao.firebaseio.com"  //Firebase Project URL
#define FIREBASE_AUTH"PYJQmGJQdFQrvE9DXGg9jVTRuDmTupOmmm6WxUS3"  //Firebase Database Secret                                 

void setup() 
{
  Serial.begin(115200);                                                  
  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) { //Thoát ra khỏi vòng 
        delay(500);
        }
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  }
void loop() 
{
 
    //Nhận dữ liệu từ arduino
   if (Serial.available()) {
    gui = Serial.readString();
    delay(1);
  String str1=gui.substring(gui.indexOf("O")+1, gui.indexOf("X"));          
  String str2=gui.substring(gui.indexOf("X")+1, gui.indexOf("Y"));          
  String str3=gui.substring(gui.indexOf("Y")+1, gui.indexOf("Z"));          
  String str4=gui.substring(gui.indexOf("Z")+1, gui.indexOf("W"));          
  String str5=gui.substring(gui.indexOf("W")+1, gui.indexOf("Q"));          
     
  Firebase.setString("/Bộ phận giám sát","ChấtLượngKhôngKhí");
  Firebase.setString("/Nhiệt độ", str1);
  Firebase.setString("/Độ ẩm", str2);
  Firebase.setString("/Nồng độ gas", str3);
  Firebase.setString("/Nồng độ CO", str4);
  Firebase.setString("/Nồng độ bụi", str5);
  Firebase.setString("/State","Online!!!");
  
  } else { Firebase.setString("/State","Mất kết nối!"); }
  //debug 
    //Serial.println("Da ket noi WiFi");
    //Serial.println("Dia chi IP cua ESP8266: ");
    //Serial.println(WiFi.localIP());
    //Serial.println();
    //Serial.println(str1);
    //Serial.println(str2);
    //Serial.println(str3);
    //Serial.println(str4);
    //Serial.println(str5); 
      
delay(700);
      }
