# STM32F103C8T6 Digital Clock

## Tổng quan

Dự án này là đồng hồ điện tử sử dụng STM32F103C8T6, LCD I2C, module RTC DS3231, DFPlayer Mini (phát nhạc báo thức từ thẻ nhớ SD). Giao diện điều khiển chỉ với 3 nút: Light, Mode, Start/Stop.

## Chức năng chính

- **Hiển thị thời gian thực**
- **Bật/tắt đèn nền LCD**
- **Chỉnh thời gian**
- **Đặt báo thức (chọn nhạc báo thức từ thẻ SD)**
- **Chỉnh âm lượng báo thức**
- **Bấm giờ thể thao (Stopwatch)**

## Sơ đồ nút nhấn

| Nút        | Chức năng chính ngoài mode | Chức năng trong các mode |
|------------|---------------------------|-------------------------|
| Light      | Bật/tắt đèn nền LCD       | Di chuyển con trỏ/thông số, giảm giá trị, reset stopwatch, tắt báo thức |
| Mode       | Đổi mode, giữ để vào mode | Lưu và thoát mode       |
| Start/Stop | Tăng giá trị, bắt đầu/tạm dừng stopwatch | Tăng giá trị           |

## Hướng dẫn sử dụng

### 1. Chế độ hiển thị thời gian (Display mode)
- Mặc định hiển thị giờ, ngày, tháng, năm.
- **Light:** Bật/tắt đèn nền LCD.
- **Mode:** Nhấn giữ để chuyển mode.

### 2. Chỉnh thời gian (Time adjustment mode)
- **Vào mode:** Nhấn giữ Mode 1 giây.
- **Light:** Di chuyển qua từng thông số (ngày, tháng, năm, giờ, phút, giây).
- **Start/Stop:** Tăng giá trị thông số đang chọn.
- **Mode:** Lưu và thoát.

### 3. Đặt báo thức (Set alarm mode)
- **Vào mode:** Nhấn giữ Mode 2 giây.
- **Light:** Di chuyển qua từng thông số (giờ, phút, giây).
- **Start/Stop:** Tăng giá trị thông số đang chọn.
- **Mode:** Lưu và thoát.

### 4. Bấm giờ thể thao (Sports stopwatch mode)
- **Vào mode:** Nhấn giữ Mode 3 giây.
- **Start/Stop:** Bắt đầu/tạm dừng stopwatch.
- **Light:** Reset stopwatch về 0.
- **Mode:** Lưu và thoát.

### 5. Chọn nhạc báo thức (Ringtone select mode)
- **Vào mode:** Chọn từ menu mode.
- **Start/Stop:** Chuyển bài nhạc tiếp theo.
- **Light:** Chuyển bài nhạc trước đó.
- **Mode:** Xác nhận chọn bài nhạc.

### 6. Chỉnh âm lượng báo thức (Volume adjust mode)
- **Vào mode:** Chọn từ menu mode.
- **Start/Stop:** Tăng âm lượng.
- **Light:** Giảm âm lượng.
- **Mode:** Xác nhận và lưu.

### Khi báo thức kêu
- Đèn nền LCD nhấp nháy mỗi 1 giây.
- **Light:** Nhấn để tắt báo thức.

## Phần cứng sử dụng

- STM32F103C8T6
- LCD 16x2 I2C
- RTC DS3231
- DFPlayer Mini + thẻ nhớ microSD (file nhạc: 001.mp3, 002.mp3, ...)
- 3 nút nhấn: Light, Mode, Start/Stop

## Hướng dẫn nạp và sử dụng

1. Nạp code cho STM32F103C8T6.
2. Kết nối phần cứng như sơ đồ.
3. Copy file nhạc báo thức vào thẻ nhớ microSD (đặt tên 001.mp3, 002.mp3, ...).
4. Lắp thẻ nhớ vào DFPlayer.
5. Cấp nguồn cho mạch.
6. Sử dụng 3 nút để thao tác như hướng dẫn ở trên.

## License

Dự án mã nguồn mở, sử dụng cho mục đích cá nhân và học tập.

---
