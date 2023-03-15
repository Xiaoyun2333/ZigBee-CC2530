String inputString;
void setup() 
{ 
  Serial.begin(115200); //打开串口1  波特率为115200
  Serial3.begin(115200);//打开串口3
} 
void loop() 
{ 
  // put your main code here, to run repeatedly: 
  
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



