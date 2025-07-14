#include <Wire.h>

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Đang quét I2C...");

  Wire.begin();
}
void loop() {
  byte error, address;
  int nDevices = 0;
  for (address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Thiết bị I2C tìm thấy tại địa chỉ 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
      nDevices++;
    }
  }

  if (nDevices == 0)
    Serial.println("Không tìm thấy thiết bị I2C nào.");
  else
    Serial.println("Quét hoàn tất.");

  delay(1000);
}

/*
Thiết bị I2C tìm thấy tại địa chỉ 0x19 
Thiết bị I2C tìm thấy tại địa chỉ 0x1E 
Thiết bị I2C tìm thấy tại địa chỉ 0x5A -> MLX 
Quét hoàn tất.
*/
