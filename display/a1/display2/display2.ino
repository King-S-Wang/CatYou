
#include <Wire.h>//申明OLED 12864的函数库
#include <Adafruit_GFX.h>//申明OLED 12864的函数库
#include <Adafruit_SSD1306.h>//申明OLED 12864的函数库
#include "dht11.h"
#include <Wire.h>   //IIC头文件
#include <MsTimer2.h>               //定时器库的 头文件
dht11 DHT11;
#include <Servo.h>  //Arduino IDE自带的舵机库文件
// 舵机管脚
#define Servo_Pin      5                   
//press value pin
int fsrPin = 14;     // A0 接口
int fsrReading;

//display value pin
const uint8_t OLED_CS = 9;//定义OLED 12864的CS引脚连接arduino9引脚
const uint8_t OLED_DC = 10; //定义OLED 12864的DC引脚连接arduino10引脚
const uint8_t OLED_RESET = 11;//定义OLED 12864的RES引脚连接arduino11引脚
const uint8_t OLED_MOSI = 12;//定义OLED 12864的D1(MOST)引脚连接arduino12引脚
const uint8_t  OLED_CLK = 13;//定义OLED 12864的D0(CLK)引脚连接arduino13引脚
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);


//movement test
int Move_Sensor_pin = 8;

//Temperature
int incomingByte = 0;                    // 接收到的 data byte
String inputString = "";                 // 用来储存接收到的内容
boolean newLineReceived = false;         // 前一次数据结束标志
boolean startBit  = false;               //协议开始标志
String returntemp = "";           //存储返回值 
char temp[100] = {0};

//motuo
Servo myservo;      //定义舵机对象myservo
/*printf格式化字符串初始化*/
int serial_putc( char c, struct __file * )
{
  Serial.write( c );
  return c;
}
void printf_begin(void)
{
  fdevopen( &serial_putc, 0 );
}

void setup(void) {
  //common
  Serial.begin(9600);
  
  //movement
  pinMode(Move_Sensor_pin,INPUT); //设置人体红外接口为输入状态

  //display
  display.begin(SSD1306_SWITCHCAPVCC);//初始化OLED 12864开始工作
  display.clearDisplay();   // 清屏
  display.setTextSize(2);   //字体尺寸大小2
  display.setTextColor(WHITE);//字体颜色为白色
  display.setCursor(0,0); //把光标定位在第0列，第0行
  display.print("  SMU-IoT");//显示字符
  display.setCursor(0,16); //把光标定位在第0列，第16行
  display.print("Team03");//显示数字
  display.setCursor(0,32); //把光标定位在第0列，第32行
  display.print("CatYou"); //显示字符
  display.setCursor(0,48); //把光标定位在第0列，第48行
  display.print("Fighting"); //显示字符
  display.display();//显示
  delay(2000);
  

  //Temperature
  Wire.begin(0);
  //Wire.begin();         // join i2c bus (address optional for master)

  //motuo
  printf_begin();
  //设置舵机控制引脚为5
  myservo.attach(Servo_Pin);
  //初始化舵机位置0
  myservo.write(0);
}
uint16_t result;
float temp1;


void loop(void) {
  Serial.print(digitalRead(Move_Sensor_pin));
  delay(1000);
  while(digitalRead(Move_Sensor_pin)==0)
  {
  display.clearDisplay();
  display.setTextSize(2);   //字体尺寸大小2
  display.setCursor(0,16); //把光标定位在第0列，第0行
  display.print("   Energy");//显示字符
  display.setCursor(0,32); //把光标定位在第0列，第16行
  display.print("   Saving");//显示数字
  display.display();//显示
  }
  

  fsrReading = analogRead(fsrPin); //heavy
  delay(2000);
  display.clearDisplay();
  display.setCursor(0,0); //把光标定位在第0列，第0行
  display.print("  SMU-IoT");//显示字符
  display.setCursor(0,16); //把光标定位在第0列，第16行
  display.print("presre "); display.print(fsrReading);//显示字符
  //Serial.print(fsrReading);
  
  Wire.beginTransmission(0x5A);
  Wire.write(0x07);                // sends instruction byte
  Wire.endTransmission(false);     // stop transmitting
  Wire.requestFrom(0x5A, 3);   //Send data n-bytes read
  result = Wire.read(); //Receive DATA
  result |= Wire.read() << 8; //Receive DATA
  uint8_t pec = Wire.read();
  temp1 =  result*0.02-273.15;  //温度数值转换
  //Serial.print(temp1);


  

  display.setCursor(0,32); //把光标定位在第0列，第32行
  display.print("Temp "); display.print(temp1);//显示字符
  display.display();//显示
  



  while (newLineReceived|| digitalRead(Move_Sensor_pin)>0)// and digitalRead(Move_Sensor_pin)>0
  { 

    while(inputString.indexOf("SERVO") == 1)    //如果要检索的字符串值“SERVO”出现
       {//$SERVO,90# 
       int i = inputString.indexOf("#",7);  //从接收到的数据中以7为起始位置检索字符串“#”的位置
       if(i > 0)        //如果检索到了
       {String temp = inputString.substring(7, i);  //提取字符串中介于指定下标7到i之间的字符赋值给temp
         int Pos = temp.toInt();                    //将字符串temp转为整型
         if(Pos >=0 && Pos <= 180)                 //如果收到的协议中舵机要转动的角度在0-180以内
         {myservo.write(Pos);                  //舵机转动相应角度
          returntemp = "$SERVO,0,#"; //返回匹配成功
          Serial.print(returntemp); //返回协议数据包       
          inputString = "";   // clear the string
          newLineReceived = false;}
         else
         { returntemp = "$SERVO,1,#";  //返回匹配失败
            Serial.print(returntemp); //返回协议数据包       
            inputString = "";   // clear the string
            newLineReceived = false;
            break;    
         }
       }
       }

        if(inputString.indexOf("SERVO") == -1)  //如果要检索的字符串值“TH”没有出现
       {    
           break;}
       }


//
//    while(inputString.indexOf("TH") == 1){
//    //_________________________________________________________________________
//
//        fsrReading = analogRead(fsrPin); //heavy
//        Wire.beginTransmission(0x5A);
//        Wire.write(0x07);                // sends instruction byte
//        Wire.endTransmission(false);     // stop transmitting
//        Wire.requestFrom(0x5A, 3);   //Send data n-bytes read
//        result = Wire.read();        //Receive DATA
//        result |= Wire.read() << 8;  //Receive DATA
//        uint8_t pec = Wire.read();
//        temp1 =  result*0.02-273.15;  //温度数值转换
//        float fTemp = temp1;
//        float weight=fsrReading;  //weight赋值为读取到的重量
        //Serial.println(fsrReading);
        
        //解析开关
       // $TH,1#
       
//       memset(temp, 0x00, sizeof(temp));        //清空temp数组
//       dtostrf(fTemp, 3, 1, temp);  // 相當於 %3.2f
//       String sTemp = temp;         //数组temp中的字符串赋给sTemp
 //      String sWt =  "";    
//       sWt += weight;
//       returntemp = "$TH,T" + sTemp + ",H" + sWt + "#";
//       Serial.print(returntemp); //返回协议数据包   
//       inputString = "";   // clear the string
//       newLineReceived = false;
      
//       if(inputString.indexOf("TH") == -1)  //如果要检索的字符串值“TH”没有出现
//       {
//       break;} 
//      }
       
  
}

void serialEvent()
{
  while (Serial.available()) 
  {    
    incomingByte = Serial.read();              //一个字节一个字节地读，下一句是读到的放入字符串数组中组成一个完成的数据包
    if(incomingByte == '$')                    //如果串口接收到数据则进入循环
    {
      startBit= true;                          //如果到来的字节是'$'，开始读取
    }
    if(startBit == true)
    {
       inputString += (char) incomingByte;     // 全双工串口可以不用在下面加延时，半双工则要加的//
    }  
    if (incomingByte == '#') 
    {
       newLineReceived = true;                //如果到来的字节是'#'，读取结束
       startBit = false;
    }
  }
}

