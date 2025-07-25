# Thiết kế và thi công thiết bị theo dõi các chỉ số sinh học trong y tế 
Roadmap: https://mm.tt/app/map/3556523294?t=FeYsaCJsj1
## Giai đoạn 1: Tìm hiểu IoT ứng dụng trong y tế thông minh (31/12/2024)
- Đọc các tài liệu liên quan đến IoT và ứng dụng trong y tế (ví dụ: smart health monitoring, patient tracking).
- Xác định những chỉ số sinh học cần theo dõi
  + Nhịp tim
  + SpO2
  + Nhiệt độ cơ thể
  + Chuyển động và hoạt động
- Nghiên cứu các thiết bị và cảm biến hiện có trên thị trường:
  + Vi xử lý: Kit phát triển WiFi BLE5 SoC ESP32-S3 Super Mini 4MB
  + Mạch ESP32-S3 Super Mini Expansion Board
  + Cảm biến nhịp tim và SpO2: MAX30102
  + Cảm biến nhiệt độ cơ thể: MLX90614
  + Cảm Biến gia tốc GY-521 6DOF IMU MPU6050
  + Pin: 3.7 Li-Po - 2000mA
  + Mạch sạc: TP-4056
  + Màn hình hiển thị: Panel màn hình LCD TFT IPS 1.69 Inch Rounded Corners ST7789V2 SPI Interface
  + Khác: Module tạo rung nhỏ (Buzzer), nút nhấn (3 cái)

## Giai đoạn 2: Thiết kế thiết bị wearable 
### 1. Lên ý tưởng thiết kế [Done]
#### Chức năng:
- Theo dõi sức khỏe
- Cảnh báo và nhắc nhở
- Giao tiếp không dây
- Hiển thị và giao diện
- Tiết kiệm năng lượng
#### Thiết kế kiểu dáng:
- Đường kính khoảng 40-45mm, dày 10-12mm
- Dây đeo thoải mái, chất liệu silicon hoặc da
### 2. Thiết kế mạch nguyên lý
- MCU và giao tiếp ngoại vi
- Cảm biến
- Mạch nguồn và sạc
- Màn hình hiển thị và giao tiếp không dây

## Giai đoạn 3: Chia tasks
### 1. Code phần cứng: (Hiển + Thoa)
+ Lấy dữ liệu từ các cảm biến (MAX30102, MLX90614, LSM303DLHC)
  + **[Done]** MAX30102 -> heart rate + spO2
  + **[Done]** MLX90614: temperature.ino
  + **[Done]** MPU6050: đếm bước chân -> pedometers.ino
+ **[Done]** Hiển thị dữ liệu trên màn hình Màn hình LCD ST7789V2 SPI Interface
+ **[In Progress]** Xử lý nút nhấn: mỗi lần nhấn, chuyển qua một màn hình khác nhau và hiển thị dữ liệu cảm biến
+ **[In Progress]** Xử lý mạch sạc, nguồn cho ESP32. Bổ sung thêm 1 công tắc nguồn nếu muốn bật/ngắt nguồn từ pin vào ESP32. (Làm thêm Deep Sleep Mode nếu có thời gian)
+ **[In Progress]** Tìm hiểu cách Multitasks cho ESP32 với FreeRTOS

**Reference:**  
- How to Multitask with FreeRTOS (ESP32 + Arduino series): https://www.youtube.com/watch?v=WQGAs9MwXno
- Manage FreeRTOS tasks - Suspend, Delay, Resume, Delete (ESP32 + Arduino series): https://www.youtube.com/watch?v=jJaGRCgDo9s
- Keep WiFi Connection Alive with FreeRTOS Task (ESP32 + Arduino series): https://www.youtube.com/watch?v=YSGPcm-qxDA
  
### 2. AI: Dự đoán nhịp tim để giám sát stress (Ánh, Ngân)
#### 2.1 Model Development
**[Done]** Google Colab:  [https://colab.research.google.com/drive/1AIxBh9OB1w2gnLzUGjKlHeO3H2ycOqI2?usp=sharing](https://colab.research.google.com/drive/1b7qM1nVnIkjZ6yTLP71XRZEenfaDeFCO)

#### 2.2 ESP-DL format (In Progress)

2.2.1 Requirements

- Download ESP-DL: `git clone --recursive https://github.com/espressif/esp-dl.git`

ESP-DL (ESP Deep Learning) là thư viện AI chính thức của Espressif được thiết kế để triển khai các mô hình học sâu trên các vi điều khiển như ESP32, ESP32-S3, đặc biệt tối ưu cho inference trên thiết bị (on-device AI).

Convert TensorFlow to ONNX: 

```python
model = tf.keras.models.load_model("my_model.h5")
tf.saved_model.save(model, "tmp_model")
!python -m tf2onnx.convert --saved-model tmp_model --output "model.onnx"
```

2.2.2 Optimization and Quantization


2.2.3 Evaluate

#### 2.3 Model Deployment (In Progress)

**References:** 
- ESP32-S3 Edge AI:
  + Human Activity Recognition Using Accelerometer Data and ESP-DL: https://developer.espressif.com/blog/esp32-s3-edge-ai-human-activity-recognition-using-accelerometer-data-and-esp-dl/
  + Hand Gesture Recognition with ESP-Deep Learning: https://developer.espressif.com/blog/hand-gesture-recognition-on-esp32-s3-with-esp-deep-learning/
- [Tutorial] Installation and Use of ESP-IDF Extension in Visual Studio Code: https://www.youtube.com/watch?v=EBXK3Mr6y7I
- Dataset: https://www.kaggle.com/datasets/vinayakshanawad/heart-rate-prediction-to-monitor-stress-level
- ESP-DL Github: https://github.com/espressif/esp-dl
- How to quantize model: https://docs.espressif.com/projects/esp-dl/en/latest/tutorials/how_to_quantize_model.html
- ESP-DL User Guide: https://docs.espressif.com/projects/esp-dl/en/latest/esp-dl-en-master.pdf

### 3. App + CSDL (Hoàng)
+ **[Done]** Xác định chức năng cần có của ứng dụng (hiển thị dữ liệu, cảnh báo, biểu đồ…).
+ **[Done]** Thiết kế giao diện UI/UX (dùng Figma).
  Link Figma: https://www.figma.com/design/GsawOiaFtUengL5APmdNon/Health-app-design--Community-?node-id=0-1&p=f
+ **[In Progress]** Tạo App và triển khai tích hợp với ESP32

**References:** 
- IoT with Firebase and your own Android App - Part-1: https://www.youtube.com/watch?v=eyZyYIVxaJQ
- IoT with Firebase and make your own Android App (without coding) - Part 2 : https://www.youtube.com/watch?v=xsHD0uGkDbw
- Other videos: https://www.youtube.com/results?search_query=mit+app+inventor+firebase+esp32
