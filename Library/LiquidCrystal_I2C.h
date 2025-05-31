/*
 *	LiquidCrystal_I2C.h
 * 
 *	Thư viện điều khiển màn hình LCD sử dụng giao tiếp I2C
 *  Tích hợp với vi điều khiển STM32 thông qua HAL library.
 *  
 *	Author: Ngoc Tuan
 *	Date: YYYY-MM-DD
 *	Version: 1.0
 * 
 * Chức năng chính:
 * - Khởi tạo và cấu hình màn hình LCD I2C.
 * - Hiển thị ký tự, chuỗi, tạo các ký tự đặc biệt.
 * - Điều chỉnh các chế độ hiển thị: bật/tắt màn hình, con trỏ, nhấp nháy, đèn nền.
 * - Tích hợp dễ dàng với các ứng dụng STM32 sử dụng HAL.
 */

#ifndef LIQUIDCRYSTAL_I2C_H
#define LIQUIDCRYSTAL_I2C_H

#include "stm32f1xx_hal.h"

// *** Lệnh cơ bản cho LCD ***
#define LCD_CLEARDISPLAY 0x01       // Xóa toàn bộ màn hình LCD
#define LCD_RETURNHOME 0x02         // Trả con trỏ về vị trí mặc định (hàng 0, cột 0)
#define LCD_ENTRYMODESET 0x04       // Đặt chế độ nhập dữ liệu
#define LCD_DISPLAYCONTROL 0x08     // Điều khiển hiển thị (bật/tắt màn hình, con trỏ, nhấp nháy)
#define LCD_CURSORSHIFT 0x10        // Dịch chuyển con trỏ hoặc toàn bộ màn hình
#define LCD_FUNCTIONSET 0x20        // Cài đặt các chức năng cơ bản của LCD
#define LCD_SETCGRAMADDR 0x40       // Cài đặt địa chỉ bộ nhớ CGRAM (Custom Characters)
#define LCD_SETDDRAMADDR 0x80       // Cài đặt địa chỉ bộ nhớ DDRAM (Display Data)

// *** Cờ điều khiển hiển thị ***
#define LCD_DISPLAYON 0x04          // Bật màn hình
#define LCD_DISPLAYOFF 0x00         // Tắt màn hình
#define LCD_CURSORON 0x02           // Bật con trỏ
#define LCD_CURSOROFF 0x00          // Tắt con trỏ
#define LCD_BLINKON 0x01            // Bật chế độ nhấp nháy con trỏ
#define LCD_BLINKOFF 0x00           // Tắt chế độ nhấp nháy con trỏ

// *** Điều khiển đèn nền ***
#define LCD_BACKLIGHT 0x08          // Bật đèn nền
#define LCD_NOBACKLIGHT 0x00        // Tắt đèn nền

// *** Điều khiển chân RS và EN ***
#define LCD_RS_SET 0x01             // RS = 1 (Data mode)
#define LCD_RS_CLEAR 0x00           // RS = 0 (Command mode)
#define LCD_EN_SET 0x04             // EN = 1 (Kích hoạt giao tiếp)
#define LCD_EN_CLEAR 0x00           // EN = 0 (Kết thúc giao tiếp)

// *** Địa chỉ I2C mặc định ***
#define LCD_ADDR_DEFAULT 0x27       // Địa chỉ mặc định của module I2C LCD

// *** Cấu trúc dữ liệu của LCD ***
typedef struct
{
	uint8_t Addr;                   // Địa chỉ I2C của LCD
	I2C_HandleTypeDef *pI2c;        // Con trỏ đến giao tiếp I2C
	uint8_t DisplayControl;         // Trạng thái điều khiển hiển thị
	uint8_t Backlightval;           // Trạng thái đèn nền (bật/tắt)
} LiquidCrystal_I2C;

// *** Prototype các hàm điều khiển LCD ***
void lcd_init(LiquidCrystal_I2C *lcd, I2C_HandleTypeDef *_hi2c, uint8_t addr);   // Khởi tạo LCD
void lcd_clear_display(LiquidCrystal_I2C *lcd);                                 // Xóa màn hình LCD
void lcd_set_cursor(LiquidCrystal_I2C *lcd, uint8_t row, uint8_t col);          // Đặt vị trí con trỏ trên LCD
void lcd_display_off(LiquidCrystal_I2C *lcd);                                   // Tắt màn hình
void lcd_display_on(LiquidCrystal_I2C *lcd);                                    // Bật màn hình
void lcd_backlight_on(LiquidCrystal_I2C *lcd);                                  // Bật đèn nền
void lcd_backlight_off(LiquidCrystal_I2C *lcd);                                 // Tắt đèn nền

// Điều khiển con trỏ
void lcd_set_cursor_on(LiquidCrystal_I2C *lcd);                                 // Bật con trỏ
void lcd_set_cursor_off(LiquidCrystal_I2C *lcd);                                // Tắt con trỏ
void lcd_set_cursor_blink_on(LiquidCrystal_I2C *lcd);                           // Bật nhấp nháy con trỏ
void lcd_set_cursor_blink_off(LiquidCrystal_I2C *lcd);                          // Tắt nhấp nháy con trỏ

// Tạo và hiển thị ký tự đặc biệt
void lcd_create_char(LiquidCrystal_I2C *lcd, uint8_t location, uint8_t charmap[]);  // Tạo ký tự tùy chỉnh
void lcd_write(LiquidCrystal_I2C *lcd, uint8_t location);                        // Gửi dữ liệu hiển thị

// Hiển thị ký tự và chuỗi
void lcd_putchar(LiquidCrystal_I2C *lcd, uint8_t data);                          // Hiển thị một ký tự
void lcd_printf(LiquidCrystal_I2C *lcd, const char* str, ...);                   // Hiển thị chuỗi (hỗ trợ định dạng printf)

#endif
