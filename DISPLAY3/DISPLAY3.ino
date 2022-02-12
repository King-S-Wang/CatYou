#include <Wire.h>
void setup() {
  // put your setup code here, to run once:
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
}
 
uint16_t result;
float temp;
 
void loop() {
   
  // put your main code here, to run repeatedly:
  Wire.beginTransmission(0x5A);
  Wire.write(0x07);            // sends instruction byte
  Wire.endTransmission(false);     // stop transmitting
 
  Wire.requestFrom(0x5A, 3);//Send data n-bytes read
  result = Wire.read(); //Receive DATA
  result |= Wire.read() << 8; //Receive DATA
 
  uint8_t pec = Wire.read();
   
  temp =  result*0.02-273.15;//
 
  Serial.print(temp);
 
  Serial.println();
  // delay(500)

}

