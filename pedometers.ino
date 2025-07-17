// References: https://coolplaydev.com/esp32-step-counter-with-mp6050

#include <Wire.h>
#include <MPU6050_light.h>

// Step counter variables
volatile unsigned long stepCount = 0;
unsigned long lastStepTime = 0;
const float ACCEL_THRESHOLD = 0.3;
const unsigned long STEP_DELAY = 300;
bool stepDetected = false;

// Sensor
MPU6050 mpu(Wire);

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  mpu.begin();
  mpu.calcOffsets();  // Tự động cân chỉnh offset khi khởi động

  Serial.println("MPU6050 ready!");
}

void loop() {
  static unsigned long lastUpdate = 0;
  mpu.update();

  // Lấy dữ liệu gia tốc
  float accelX = mpu.getAccX();
  float accelY = mpu.getAccY();
  float accelZ = mpu.getAccZ();

  float accelMagnitude = sqrt(accelX * accelX + accelY * accelY + accelZ * accelZ);

  unsigned long currentTime = millis();

  // Phát hiện bước chân
  if (accelMagnitude > (1.0 + ACCEL_THRESHOLD)) {
    if (!stepDetected && (currentTime - lastStepTime > STEP_DELAY)) {
      stepCount++;
      stepDetected = true;
      lastStepTime = currentTime;

      Serial.print("Step detected! Total steps: ");
      Serial.println(stepCount);
    }
  } else {
    stepDetected = false;
  }

  delay(100);
}
