#include <TFT_eSPI.h>
#include "DS1302.h"
#include "esp_sleep.h"
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"

// ==== CHÂN RTC DS1302 ====
#define DS1302_RST   2
#define DS1302_IO    3
#define DS1302_CLK   4

// ==== NÚT BẬT/TẮT MÀN HÌNH ====
#define BTN_SCREEN  1   // dùng GPIO14 thay vì GPIO1 cho an toàn

TFT_eSPI tft = TFT_eSPI();
DS1302 rtc(DS1302_RST, DS1302_IO, DS1302_CLK);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
Adafruit_MPU6050 mpu;
MAX30105 particleSensor;

// ==== STEP COUNTER ====
long stepCount = 0;  
float lastMagnitude = 0;
bool stepFlag = false;
unsigned long lastStepTime = 0;

// ==== HR & SpO2 ====
int heartRateVal = 0;
int spo2Val      = 0;

// buffer cho SpO2/HR algorithm
#define BUFFER_SIZE 100
uint32_t irBuffer[BUFFER_SIZE];
uint32_t redBuffer[BUFFER_SIZE];
int bufferIndex = 0;
bool bufferFull = false;

// thuật toán SpO2
int32_t spo2;
int8_t validSPO2;
int32_t heartRate;
int8_t validHeartRate;

const char* dayNames[]   = {"", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const char* monthNames[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

bool screenOn = true;
unsigned long lastUpdate = 0;
const unsigned long updateInterval = 1000; // update LCD mỗi giây

// ===================== GIAO DIỆN =====================
void drawInterface() {
    tft.fillScreen(TFT_BLACK);

    // Header
    tft.fillRoundRect(0, 0, 240, 40, 8, TFT_BLUE);
    tft.setTextColor(TFT_WHITE, TFT_BLUE);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(2);
    tft.drawString("HEALTH MONITOR", 120, 20);

    // TIME box
    tft.setTextSize(2);
    tft.drawRoundRect(5, 45, 230, 70, 5, TFT_WHITE);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(10, 50);
    tft.print("TIME");

    // HEART RATE box
    tft.drawRoundRect(5, 120, 110, 60, 5, TFT_WHITE);
    tft.setCursor(10, 125);
    tft.print("HEART");

    // TEMPERATURE box
    tft.drawRoundRect(125, 120, 110, 60, 5, TFT_WHITE);
    tft.setCursor(130, 125);
    tft.print("TEMP");

    // SPO2 box
    tft.drawRoundRect(5, 190, 110, 60, 5, TFT_WHITE);
    tft.setCursor(10, 195);
    tft.print("SPO2");

    // STEPS box
    tft.drawRoundRect(125, 190, 110, 60, 5, TFT_WHITE);
    tft.setCursor(130, 195);
    tft.print("STEPS");
}

// ===================== CẬP NHẬT DỮ LIỆU =====================
void updateValues(Time now) {
    // --- TIME ---
    char dateBuf[40];
    sprintf(dateBuf, "%s, %02d %s, %04d", 
            dayNames[now.day], now.date, monthNames[now.mon-1], now.yr);

    char timeBuf[20];
    sprintf(timeBuf, "%02d:%02d:%02d", now.hr, now.min, now.sec);

    tft.setTextDatum(TL_DATUM);
    tft.setTextSize(2);

    tft.fillRect(10, 65, 220, 25, TFT_BLACK);
    tft.fillRect(10, 90, 220, 25, TFT_BLACK);

    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.drawString(dateBuf, 15, 70);
    tft.drawString(timeBuf, 15, 95);

    // --- TEMPERATURE ---
    double tempC = mlx.readObjectTempC();
    char tempBuf[20];
    sprintf(tempBuf, "%.1f C", tempC);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    if (tempC > 37.8) tft.setTextColor(TFT_RED, TFT_BLACK);
    else if (tempC > 37.0) tft.setTextColor(TFT_YELLOW, TFT_BLACK);

    tft.fillRect(130, 145, 100, 30, TFT_BLACK);
    tft.drawString(tempBuf, 135, 150);

    // --- HEART RATE ---
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    char hrBuf[20];
    if (heartRateVal > 0) sprintf(hrBuf, "%d BPM", heartRateVal);
    else sprintf(hrBuf, "---");
    tft.fillRect(10, 145, 100, 25, TFT_BLACK);
    tft.drawString(hrBuf, 15, 150);

    // --- SpO2 ---
    tft.setTextColor(TFT_MAGENTA, TFT_BLACK);
    char spo2Buf[20];
    if (spo2Val > 0) sprintf(spo2Buf, "%d %%", spo2Val);
    else sprintf(spo2Buf, "---");
    tft.fillRect(10, 210, 100, 25, TFT_BLACK);
    tft.drawString(spo2Buf, 15, 215);

    // --- STEPS ---
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    char stepBuf[20];
    sprintf(stepBuf, "%ld", stepCount);
    tft.fillRect(130, 210, 100, 25, TFT_BLACK);  
    tft.drawString(stepBuf, 135, 215);
}

// ===================== STEP COUNTER =====================
void updateSteps() {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    float magnitude = sqrt(a.acceleration.x * a.acceleration.x +
                           a.acceleration.y * a.acceleration.y +
                           a.acceleration.z * a.acceleration.z);

    unsigned long now = millis();
    if (!stepFlag && abs(magnitude - lastMagnitude) > 3.0 &&
        (now - lastStepTime) > 400) {
        stepCount++;
        stepFlag = true;
        lastStepTime = now;
    } else if (stepFlag && abs(magnitude - lastMagnitude) < 1.5) {
        stepFlag = false;
    }
    lastMagnitude = magnitude;
}

// ===================== ĐỌC MAX3010x =====================
void updateMax3010x() {
    if (particleSensor.available()) {
        redBuffer[bufferIndex] = particleSensor.getRed();
        irBuffer[bufferIndex]  = particleSensor.getIR();
        particleSensor.nextSample();

        bufferIndex++;
        if (bufferIndex >= BUFFER_SIZE) {
            bufferIndex = 0;
            bufferFull = true;
        }

        if (bufferFull) {
            Serial.println("Đủ 100 mẫu, chạy thuật toán...");
            maxim_heart_rate_and_oxygen_saturation(
                irBuffer, BUFFER_SIZE, redBuffer,
                &spo2, &validSPO2, &heartRate, &validHeartRate);

            if (validHeartRate && heartRate > 30 && heartRate < 200) {
                heartRateVal = heartRate;
            }
            if (validSPO2 && spo2 > 70 && spo2 <= 100) {
                spo2Val = spo2;
            }

            Serial.print("HR=");
            Serial.print(heartRateVal);
            Serial.print(" BPM  SpO2=");
            Serial.print(spo2Val);
            Serial.println(" %");
        }
    }
}


// ===================== SETUP =====================
void setup() {
    Serial.begin(115200);
    pinMode(BTN_SCREEN, INPUT_PULLUP);

    tft.init();
    tft.setRotation(0);
    tft.setTextSize(1);

    rtc.writeProtect(false);
    rtc.halt(false);
    Wire.begin(8, 9, 400000);
    if (!mlx.begin()) {
        Serial.println("Error: MLX90614 not found!");
    }
    if (!mpu.begin()) {
        Serial.println("Error: MPU6050 not found!");
    }

    if (!particleSensor.begin()) {
        Serial.println("MAX3010x not found!");
        while(1);
    }
    
    particleSensor.setup(0x32, 4, 2, 100, 411, 4096);

    drawInterface();
}

// ===================== LOOP =====================
void loop() {
    Serial.print(" R[");
    Serial.print(particleSensor.getRed());
    Serial.print("] IR[");
    Serial.print(particleSensor.getIR());
    Serial.print("] G[");
    Serial.print(particleSensor.getGreen());
    Serial.print("]");

    Serial.println();
    if (digitalRead(BTN_SCREEN) == LOW) {
        delay(200);
        if (screenOn) {
            Serial.println("Tắt màn hình - Deep Sleep");
            tft.writecommand(TFT_DISPOFF);
            tft.writecommand(TFT_SLPIN);
            screenOn = false;
            esp_sleep_enable_ext0_wakeup((gpio_num_t)BTN_SCREEN, 0);
            esp_deep_sleep_start();
        }
    }

    if (screenOn) {
        unsigned long nowMillis = millis();

        static unsigned long lastStepCheck = 0;
        if (nowMillis - lastStepCheck >= 100) {
            lastStepCheck = nowMillis;
            updateSteps();
            updateMax3010x();  // đọc dữ liệu HR + SpO2
        }

        if (nowMillis - lastUpdate >= updateInterval) {
            lastUpdate = nowMillis;
            Time now = rtc.time();
            updateValues(now);
        }
    }
    delay(1); yield();
}
