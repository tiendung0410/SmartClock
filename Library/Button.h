#ifndef BUTTON_H
#define BUTTON_H

#include "main.h"

// Định nghĩa cấu trúc lưu trữ thông tin trạng thái của một nút bấm
typedef struct
{
    uint8_t btn_current;         // Trạng thái hiện tại của nút (0: nhấn, 1: không nhấn)
    uint8_t btn_last;            // Trạng thái trước đó của nút
    uint8_t btn_filter;          // Trạng thái đã qua lọc (debounce)
    uint8_t is_debouncing;       // Cờ kiểm tra trạng thái debounce
    uint32_t time_deboune;       // Thời gian bắt đầu debounce
    uint32_t time_start_press;   // Thời gian bắt đầu nhấn nút
    uint8_t is_press_timeout;    // Cờ kiểm tra trạng thái timeout (nhấn giữ quá lâu)
    GPIO_TypeDef *GPIOx;         // Con trỏ đến cổng GPIO mà nút bấm kết nối
    uint16_t GPIO_Pin;           // Chân GPIO của nút bấm
} Button_Typdef;

// Khai báo các hàm xử lý nút bấm

/**
 * @brief Hàm xử lý tín hiệu nút bấm. Quản lý các sự kiện nhấn, thả, thời gian nhấn.
 * 
 * Hàm này được gọi trong vòng lặp chính để liên tục kiểm tra và xử lý trạng thái của nút bấm.
 * Nó thực hiện các tác vụ như debounce (lọc tín hiệu), nhấn ngắn, nhấn lâu và nhả nút.
 * 
 * @param ButtonX Con trỏ đến cấu trúc Button_Typdef chứa thông tin của nút bấm.
 */
void button_handle(Button_Typdef *ButtonX);

/**
 * @brief Hàm khởi tạo cấu trúc Button và các thông số GPIO.
 * 
 * Hàm này được sử dụng để cấu hình thông tin về GPIO (cổng và chân) mà nút bấm sử dụng.
 * 
 * @param ButtonX Con trỏ đến cấu trúc Button_Typdef cần khởi tạo.
 * @param GPIOx Con trỏ đến cổng GPIO mà nút bấm sẽ kết nối.
 * @param GPIO_Pin Chân GPIO mà nút bấm sẽ sử dụng.
 */
void button_init(Button_Typdef *ButtonX, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

#endif /* BUTTON_H */
