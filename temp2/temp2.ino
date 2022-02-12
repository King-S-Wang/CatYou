#include <Wire.h>
#include <Adafruit_MLX90614.h> 
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  Serial.begin(9600);
  mlx.begin();
}

void loop() {
  Serial.println("Temperature from MLX90614:");
  Serial.print("Ambient:      ");
  Serial.print(mlx.readAmbientTempC());
  Serial.println(" °C");
  Serial.print("Contactless: ");
  Serial.print(mlx.readObjectTempC());
  Serial.println(" °C");
  Serial.println();
  delay(1000);
}

