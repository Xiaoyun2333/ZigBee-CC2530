
#include <SPI.h>
#include <MFRC522.h>

MFRC522 rfid(53, 48);
String RFID_CardUID;
byte set[1]={20};

byte money[1]={0};

boolean MFRC522_WriteCard(MFRC522 *_name, byte _block, byte *_buffer, byte _length){
  MFRC522::MIFARE_Key _key;
  for(byte i = 0; i < 6; i++)
    _key.keyByte[i] = 0xFF;
  MFRC522::StatusCode _status;
  _status = _name->PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, _block, &_key, &(_name->uid));
  if(_status != MFRC522::STATUS_OK){
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(_name->GetStatusCodeName(_status));
    return false;
  }
  else{
    Serial.println(F("PCD_Authenticate() success;"));
  }
  _status = _name->MIFARE_Write(_block, _buffer, _length);
  if(_status != MFRC522::STATUS_OK){
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(_name->GetStatusCodeName(_status));
    return false;
  }
  else{
    Serial.println(F("MIFARE_Write() success;"));
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
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(_name->GetStatusCodeName(_status));
    return false;
  }
  else{
    Serial.println(F("PCD_Authenticate() success;"));
  }
  if(_length < 18){
    byte _Read_buffer[18];
    byte _Read_buffer_length = 18;
    _status = _name->MIFARE_Read(_block, _Read_buffer, &_Read_buffer_length);
    if(_status != MFRC522::STATUS_OK){
      Serial.print(F("MIFARE_Read() failed: "));
      Serial.println(_name->GetStatusCodeName(_status));
      return false;
    }
    else{
      Serial.println(F("MIFARE_Read() success;"));
    }
    for(byte _i = 0; _i < _length; _i++)
      _buffer[_i] = _Read_buffer[_i];
  }
  else{
    _status = _name->MIFARE_Read(_block, _buffer, &_length);
    if(_status != MFRC522::STATUS_OK){
      Serial.print(F("MIFARE_Read() failed: "));
      Serial.println(_name->GetStatusCodeName(_status));
      return false;
    }
    else{
      Serial.println(F("MIFARE_Read() success;"));
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

void setup(){
  Serial.begin(38400);
  SPI.begin();
  rfid.PCD_Init();
  RFID_CardUID = "";
}

void loop(){
  if(MFRC522_IsNewCard(&rfid)){
    MFRC522_WriteCard(&rfid, 1, set, 16);
    MFRC522_ReadCard(&rfid, 1, money, 1);
    Serial.println(money[(int)(0)]);
    Serial.println(MFRC522_ReadCardUID(&rfid));
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
  delay(200);

}