#include <Wire.h>//申明OLED 12864的函数库
#include <Adafruit_GFX.h>//申明OLED 12864的函数库
#include <Adafruit_SSD1306.h>//申明OLED 12864的函数库
#include "dht11.h"
#include <Wire.h>   //IIC头文件
#include <MsTimer2.h>               //定时器库的 头文件
dht11 DHT11;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
   Wire.begin(0);
int incomingByte = 0;                    // 接收到的 data byte
String inputString = "";                 // 用来储存接收到的内容
boolean newLineReceived = false;         // 前一次数据结束标志
boolean startBit  = false;               //协议开始标志
String returntemp = "";           //存储返回值 
char temp[100] = {0};
}
uint16_t result;
float temp1;

void loop() {
  // put your main code here, to run repeatedly:
  Wire.beginTransmission(0x5A);
  Wire.write(0x07);                // sends instruction byte
  Wire.endTransmission(false);     // stop transmitting
  Wire.requestFrom(0x5A, 3);   //Send data n-bytes read
  result = Wire.read(); //Receive DATA
  result |= Wire.read() << 8; //Receive DATA
  uint8_t pec = Wire.read();
  //temp1 =  result*0.02-273.15;  //温度数值转换
  Serial.print(result);
  delay(500);
}
