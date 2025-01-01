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
  + Vi xử lý: STM32F103C8T6, 32-Bit ARM Cortex M3 Microcontroller, 72MHz, 64KB FLASH, 48-LQFP
  + Cảm biến nhịp tim và SpO2: MAX30102
  + Cảm biến nhiệt độ cơ thể: MLX90614
  + Cảm biến chuyển động MPU6050
  + Pin: 3.7 Li-Po
  + Mạch sạc: TP-4056
  + Màn hình hiển thị: GC9A01
  + Bluetooth: HC-05
  + Khác: Module tạo rung nhỏ (Buzzer), nút nhấn (2 cái)

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
### 3. Vẽ layout PCB
### 4. Gia công mạch in
### 5. Lắp ráp linh kiện + kiểm tra
