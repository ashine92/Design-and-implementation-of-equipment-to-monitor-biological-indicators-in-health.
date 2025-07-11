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
  + Cảm biến la bàn số và gia tốc: LSM303DLHC
  + Pin: 3.7 Li-Po - 2000mA
  + Mạch sạc: TP-4056
  + Màn hình hiển thị: Panel màn hình LCD TFT IPS 1.69 Inch Rounded Corners ST7789V2 SPI Interface
  + Khác: Module tạo rung nhỏ (Buzzer), nút nhấn (3 cái)

## Giai đoạn 2: Thiết kế thiết bị wearable 
### 1. Lên ý tưởng thiết kế
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
### 3. Vẽ layout PCB (Thuê)
### 4. Gia công mạch in (Thuê)
### 5. Lắp ráp linh kiện + kiểm tra (Pass tới giai đoạn sau)

## Giai đoạn 3: Chia tasks
### Code phần cứng: (Hiển + Thoa)
- Code hiển thị dữ liệu cảm biến & xử lý nút nhấn
+ Lấy dữ liệu từ các cảm biến (MAX30102, MLX90614, LSM303DLHC).
+ Hiển thị dữ liệu trên màn hình GC9A01.
+ Xử lý nút nhấn: mỗi lần nhấn, chuyển qua một màn hình khác nhau.

### AI (Ánh, Ngân)

Google Colab:  https://colab.research.google.com/drive/1AIxBh9OB1w2gnLzUGjKlHeO3H2ycOqI2?usp=sharing

References: https://drive.google.com/drive/u/0/folders/16fmTTK8AtNspV0BJJl2BhCymmkxGqh6t

+ Tìm hiểu các mô hình AI có thể áp dụng (so sánh các mô hình để lựa chọn mô hình tối uu)
+ Xây dựng dataset (dữ liệu nhịp tim, SpO2, nhiệt độ, vận động).
+ So sánh hiệu suất và độ chính xác của các mô hình.

### App (Hoàng)
+ Xác định chức năng cần có của ứng dụng (hiển thị dữ liệu, cảnh báo, biểu đồ…).
+ Thiết kế giao diện UI/UX (dùng Figma).
+ Tìm hiểu cách kết nối ứng dụng với ESP32 (BLE hoặc MQTT).

### Nghiên cứu cơ sở dữ liệu & lưu trữ dữ liệu (...)
+ Chọn cách lưu dữ liệu (Firebase, MySQL, hoặc SQLite).
+ Tìm hiểu cách đẩy dữ liệu từ ESP32 lên database.
+ Viết API để ứng dụng di động lấy dữ liệu từ server. (hoặc cách khác)

