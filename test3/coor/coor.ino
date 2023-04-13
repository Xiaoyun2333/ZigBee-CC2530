#include <U8g2lib.h>
#include <Wire.h>

String inputString;
String message;

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

char recv_state = 0;

void page0(void) {
  u8g2.setFont(u8g2_font_timR10_tf);
  u8g2.setFontPosTop();
}
void page1(void) {
  u8g2.setFont(u8g2_font_timR10_tf);
  u8g2.setFontPosTop();
  u8g2.setCursor(0,0);
  u8g2.print("Wrong identity!");
  u8g2.setCursor(0,30);
  u8g2.print("Please try again!X_X");
}
void page2(void) { 
  u8g2.setFont(u8g2_font_timR10_tf);
  u8g2.setFontPosTop();
  u8g2.setCursor(0,0);
  u8g2.print("Hello Xiaoyun^_^");
  u8g2.setCursor(0,20);
  u8g2.print("ID                      Balance");
  u8g2.setCursor(0,40);
  u8g2.print(message);
}
void setup() {
  Serial.begin(38400);
  Serial3.begin(38400);
  u8g2.setI2CAddress(0x3C*2);
  u8g2.begin();
  u8g2.enableUTF8Print();
  message = "";
}

void loop() {
  if(recv_state == 0){
    u8g2.clearDisplay();
  }
  else{
  u8g2.firstPage();  
  do {
    if(message == "\r\n") page1();
    else page2();
  } while( u8g2.nextPage() );
  }
}
void serialEvent()
{ 
    while(Serial.available())
    { 
        //读取缓冲区数据，一次读取一个字符，并移除
       char inChar = (char)Serial.read(); 
        //延时1s等待完成下一个读取
       delayMicroseconds(1000);
       //连接字符串并存储到变量inputString中
       inputString +=inChar; 
     } 
     Serial3.print(inputString);
     inputString="";
} 

//串口3监听函数，一旦缓冲区接收到数据就执行此函数
void serialEvent3()
{ 
     while(Serial3.available())
     {
         char inChar = (char)Serial3.read(); 
         delayMicroseconds(1000); 
         inputString +=inChar; 
     }
      message = inputString;
      // Serial.print(message);
      inputString=""; 
      recv_state = 1;
}
