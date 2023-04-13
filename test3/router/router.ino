#include <SPI.h>
#include <MFRC522.h>
#include <U8glib.h>

MFRC522 rfid(53, 48);
String RFID_CardUID;
String inputString;

byte set[1]={0};
byte money[1]={0};

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);//库的使用

void page1(void) {
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);//与上面一行的字体不同
  u8g.drawStr( 0, 10, "");
  u8g.drawStr( 0, 60, "");
}
void page2(void) {
  // 设置字体  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);//与上面一行的字体不同
  u8g.drawStr( 0, 10, "YuanXiPeng");
  u8g.drawStr( 0, 60, "2020070756");
}
boolean MFRC522_WriteCard(MFRC522 *_name, byte _block, byte *_buffer, byte _length){
  MFRC522::MIFARE_Key _key;
  for(byte i = 0; i < 6; i++)
    _key.keyByte[i] = 0xFF;
  MFRC522::StatusCode _status;
  _status = _name->PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, _block, &_key, &(_name->uid));
  if(_status != MFRC522::STATUS_OK){
    return false;
  }
  _status = _name->MIFARE_Write(_block, _buffer, _length);
  if(_status != MFRC522::STATUS_OK){
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(_name->GetStatusCodeName(_status));
    return false;
  }
  return true;
}

boolean MFRC522_ReadCard(MFRC522 *_name, byte _block, byte *_buffer, byte _length){
  MFRC522::MIFARE_Key _key;
  for(byte i = 0; i < 6; i++)
    _key.keyByte[i] = 0xFF;
  MFRC522::StatusCode _status;
  _status = _name->PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, _block, &_key, &(_name->uid));
  if(_status != MFRC522::STATUS_OK){
    return false;
  }
  if(_length < 18){
    byte _Read_buffer[18];
    byte _Read_buffer_length = 18;
    _status = _name->MIFARE_Read(_block, _Read_buffer, &_Read_buffer_length);
    if(_status != MFRC522::STATUS_OK){
      return false;
    }
    for(byte _i = 0; _i < _length; _i++)
      _buffer[_i] = _Read_buffer[_i];
  }
  else{
    _status = _name->MIFARE_Read(_block, _buffer, &_length);
    if(_status != MFRC522::STATUS_OK){
      return false;
    }
  }
  return true;
}
String MFRC522_ReadCardUID(MFRC522 *_name){
  String _CardUID = "";
  for (byte _i = 0; _i < _name->uid.size; _i++){
    if(_name->uid.uidByte[_i] < 0x10)
      _CardUID += "0";
    _CardUID += String(_name->uid.uidByte[_i], HEX) + String(" ");
  }
  return _CardUID;
}
boolean MFRC522_IsNewCard(MFRC522 *_name){
  if(!_name->PICC_IsNewCardPresent())
    return false;
  if(!_name->PICC_ReadCardSerial())
    return false;
  return true;
}

void setup() {
  Serial.begin(38400);
  Serial3.begin(38400);
  SPI.begin();
  rfid.PCD_Init();
  RFID_CardUID = "";
}

void loop() {
  // if (Serial.available()) {      // If anything comes in Serial (USB),
  //   Serial3.write(Serial.read());   // read it and send it out Serial1 (pins 0 & 1)
  // }

  // if (Serial3.available()) {     // If anything comes in Serial1 (pins 0 & 1)
  //   Serial.write(Serial3.read());   // read it and send it out Serial (USB)
  // }

if(MFRC522_IsNewCard(&rfid)){
  RFID_CardUID = MFRC522_ReadCardUID(&rfid); 
  MFRC522_ReadCard(&rfid, 1, money, 1);
  
  
  
  if(RFID_CardUID == "9a 6c 4d 80 "){
    Serial.println("The magnetic card ID number is correct, welcome to use! :)");
    if(money[0] == 0){
    Serial.println("Insufficient balance, payment failed! #_#");
    }
    else{
      set[0] = money[0] - 1;
       MFRC522_WriteCard(&rfid, 1, set, 16);
    } 
  }
  else{
    Serial.println("The magnetic card ID number is wrong, please use the correct magnetic card!X_X");
  } 
  delayMicroseconds(1100);
  MFRC522_ReadCard(&rfid, 1, money, 1);
  if(RFID_CardUID == "9a 6c 4d 80 "){
    Serial3.println(String(RFID_CardUID +"   "+ money[0]));
  }
  else{
    Serial3.println("");
  }
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  
}
  
  u8g.firstPage();  
  do {
    if(RFID_CardUID == "9a 6c 4d 80 ") page2();
    else page1();
  } while( u8g.nextPage() );
}
//串口1监听函数，一旦缓冲区接收到数据就执行此函数
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
     //变量清空，便于下一次存储
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
      Serial.print(inputString); 
      inputString=""; 
}
