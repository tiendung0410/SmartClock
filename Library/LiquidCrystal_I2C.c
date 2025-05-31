/*
 *	LiquidCrystal_I2C.c
 * 
 *	Thư viện điều khiển màn hình LCD sử dụng giao tiếp I2C
 *  Tích hợp với vi điều khiển STM32 thông qua HAL library.
 *  
 *	Author: Ngoc Tuan
 *	Date: YYYY-MM-DD
 *	Version: 1.0
 * 
 * Chức năng chính:
 * - Gửi lệnh và dữ liệu đến màn hình LCD thông qua giao tiếp I2C.
 * - Cung cấp các chức năng điều khiển hiển thị, đèn nền, và con trỏ.
 * - Hỗ trợ định dạng chuỗi với `printf`.
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "LiquidCrystal_I2C.h"

// *** Gửi lệnh đến màn hình LCD ***
/*
 * Gửi một lệnh đến LCD.
 * @param lcd: Con trỏ đến cấu trúc LiquidCrystal_I2C.
 * @param cmd: Lệnh cần gửi.
 */
static void lcd_send_cmd(LiquidCrystal_I2C *lcd, uint8_t cmd)
{
    uint8_t data_h, data_l;
    uint8_t data_t[4];
    data_h = (cmd & 0xf0);  // Lấy 4 bit cao
    data_l = ((cmd << 4) & 0xf0);  // Lấy 4 bit thấp
    data_t[0] = data_h | (lcd->Backlightval | LCD_RS_CLEAR | LCD_EN_SET);
    data_t[1] = data_h | (lcd->Backlightval | LCD_RS_CLEAR | LCD_EN_CLEAR);
    data_t[2] = data_l | (lcd->Backlightval | LCD_RS_CLEAR | LCD_EN_SET);
    data_t[3] = data_l | (lcd->Backlightval | LCD_RS_CLEAR | LCD_EN_CLEAR);
    HAL_I2C_Master_Transmit(lcd->pI2c, lcd->Addr, (uint8_t *)data_t, 4, 100);
}

// *** Gửi dữ liệu đến màn hình LCD ***
/*
 * Gửi một byte dữ liệu đến LCD.
 * @param lcd: Con trỏ đến cấu trúc LiquidCrystal_I2C.
 * @param data: Byte dữ liệu cần gửi.
 */
static void lcd_send_data(LiquidCrystal_I2C *lcd, uint8_t data)
{
    uint8_t data_h, data_l;
    uint8_t data_t[4];
    data_h = (data & 0xf0);  // Lấy 4 bit cao
    data_l = ((data << 4) & 0xf0);  // Lấy 4 bit thấp
    data_t[0] = data_h | (lcd->Backlightval | LCD_RS_SET | LCD_EN_SET);
    data_t[1] = data_h | (lcd->Backlightval | LCD_RS_SET | LCD_EN_CLEAR);
    data_t[2] = data_l | (lcd->Backlightval | LCD_RS_SET | LCD_EN_SET);
    data_t[3] = data_l | (lcd->Backlightval | LCD_RS_SET | LCD_EN_CLEAR);
    HAL_I2C_Master_Transmit(lcd->pI2c, lcd->Addr, (uint8_t *)data_t, 4, 100);
}

// *** Hiển thị một ký tự lên LCD ***
/*
 * Gửi một ký tự ASCII đến LCD.
 * @param lcd: Con trỏ đến cấu trúc LiquidCrystal_I2C.
 * @param data: Ký tự cần hiển thị.
 */
void lcd_putchar(LiquidCrystal_I2C *lcd, uint8_t data)
{
    lcd_send_data(lcd, data);
}

// *** Xóa toàn bộ nội dung hiển thị trên LCD ***
/*
 * Xóa màn hình LCD.
 * @param lcd: Con trỏ đến cấu trúc LiquidCrystal_I2C.
 */
void lcd_clear_display(LiquidCrystal_I2C *lcd)
{
    lcd_send_cmd(lcd, LCD_CLEARDISPLAY);
    HAL_Delay(1);  // Đợi LCD hoàn tất việc xóa màn hình
}

// *** Bật và tắt hiển thị màn hình ***
/*
 * Tắt màn hình LCD nhưng không mất dữ liệu.
 * @param lcd: Con trỏ đến cấu trúc LiquidCrystal_I2C.
 */
void lcd_display_off(LiquidCrystal_I2C *lcd)
{
    lcd->DisplayControl &= ~LCD_DISPLAYON;
    lcd_send_cmd(lcd, LCD_DISPLAYCONTROL | lcd->DisplayControl);
}

/*
 * Bật màn hình LCD.
 * @param lcd: Con trỏ đến cấu trúc LiquidCrystal_I2C.
 */
void lcd_display_on(LiquidCrystal_I2C *lcd)
{
    lcd->DisplayControl |= LCD_DISPLAYON;
    lcd_send_cmd(lcd, LCD_DISPLAYCONTROL | lcd->DisplayControl);
}

// *** Điều khiển đèn nền ***
/*
 * Bật đèn nền LCD.
 * @param lcd: Con trỏ đến cấu trúc LiquidCrystal_I2C.
 */
void lcd_backlight_on(LiquidCrystal_I2C *lcd)
{
    lcd->Backlightval = LCD_BACKLIGHT;
    HAL_I2C_Master_Transmit(lcd->pI2c, lcd->Addr, &lcd->Backlightval, 1, 100);
}

/*
 * Tắt đèn nền LCD.
 * @param lcd: Con trỏ đến cấu trúc LiquidCrystal_I2C.
 */
void lcd_backlight_off(LiquidCrystal_I2C *lcd)
{
    lcd->Backlightval = LCD_NOBACKLIGHT;
    HAL_I2C_Master_Transmit(lcd->pI2c, lcd->Addr, &lcd->Backlightval, 1, 100);
}

// *** Đặt vị trí con trỏ trên LCD ***
/*
 * Đặt vị trí con trỏ trên LCD.
 * @param lcd: Con trỏ đến cấu trúc LiquidCrystal_I2C.
 * @param row: Hàng (0 hoặc 1).
 * @param col: Cột (0 đến 15).
 */
void lcd_set_cursor(LiquidCrystal_I2C *lcd, uint8_t row, uint8_t col)
{
    uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    lcd_send_cmd(lcd, LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// *** Bật/Tắt con trỏ gạch chân ***
/*
 * Bật con trỏ gạch chân trên LCD.
 * @param lcd: Con trỏ đến cấu trúc LiquidCrystal_I2C.
 */
void lcd_set_cursor_on(LiquidCrystal_I2C *lcd)
{
    lcd->DisplayControl |= LCD_CURSORON;
    lcd_send_cmd(lcd, LCD_DISPLAYCONTROL | lcd->DisplayControl);
}

/*
 * Tắt con trỏ gạch chân trên LCD.
 * @param lcd: Con trỏ đến cấu trúc LiquidCrystal_I2C.
 */
void lcd_set_cursor_off(LiquidCrystal_I2C *lcd)
{
    lcd->DisplayControl &= ~LCD_CURSORON;
    lcd_send_cmd(lcd, LCD_DISPLAYCONTROL | lcd->DisplayControl);
}

// *** Bật/Tắt con trỏ nhấp nháy ***
/*
 * Bật con trỏ nhấp nháy trên LCD.
 * @param lcd: Con trỏ đến cấu trúc LiquidCrystal_I2C.
 */
void lcd_set_cursor_blink_on(LiquidCrystal_I2C *lcd)
{
    lcd->DisplayControl |= LCD_BLINKON;
    lcd_send_cmd(lcd, LCD_DISPLAYCONTROL | lcd->DisplayControl);
}

/*
 * Tắt con trỏ nhấp nháy trên LCD.
 * @param lcd: Con trỏ đến cấu trúc LiquidCrystal_I2C.
 */
void lcd_set_cursor_blink_off(LiquidCrystal_I2C *lcd)
{
    lcd->DisplayControl &= ~LCD_BLINKON;
    lcd_send_cmd(lcd, LCD_DISPLAYCONTROL | lcd->DisplayControl);
}

// *** Tạo ký tự tùy chỉnh ***
/*
 * Tạo một ký tự tùy chỉnh trên LCD.
 * @param lcd: Con trỏ đến cấu trúc LiquidCrystal_I2C.
 * @param location: Vị trí lưu ký tự (0-7).
 * @param charmap: Mảng 8 byte mô tả bitmap của ký tự.
 */
void lcd_create_char(LiquidCrystal_I2C *lcd, uint8_t location, uint8_t charmap[])
{
    location &= 0x7;  // Chỉ sử dụng 3 bit thấp (tối đa 8 ký tự tùy chỉnh)
    lcd_send_cmd(lcd, LCD_SETCGRAMADDR | (location << 3));  // Đặt địa chỉ CGRAM
    for (int i = 0; i < 8; i++)
    {
        lcd_send_data(lcd, charmap[i]);
    }
}

// *** Hiển thị ký tự tùy chỉnh hoặc ký tự ASCII ***
/*
 * Gửi một ký tự ASCII hoặc ký tự tùy chỉnh đến LCD.
 * @param lcd: Con trỏ đến cấu trúc LiquidCrystal_I2C.
 * @param location: Mã ký tự ASCII hoặc vị trí ký tự tùy chỉnh (0-7).
 */
void lcd_write(LiquidCrystal_I2C *lcd, uint8_t location)
{
    lcd_send_data(lcd, location);
}

// *** In chuỗi ký tự lên màn hình LCD ***
/*
 * Hiển thị một chuỗi ký tự có định dạng trên LCD.
 * @param lcd: Con trỏ đến cấu trúc LiquidCrystal_I2C.
 * @param str: Chuỗi định dạng (giống `printf`).
 * @param ...: Các tham số định dạng đi kèm.
 */
void lcd_printf(LiquidCrystal_I2C *lcd, const char *str, ...)
{
    char stringArray[20];  // Bộ nhớ đệm chuỗi (giới hạn 20 ký tự)

    va_list args;
    va_start(args, str);  // Bắt đầu xử lý các tham số định dạng
    vsprintf(stringArray, str, args);  // Chuyển đổi định dạng thành chuỗi
    va_end(args);

    for (uint8_t i = 0; i < strlen(stringArray) && i < 16; i++)  // In tối đa 16 ký tự
    {
        lcd_send_data(lcd, (uint8_t)stringArray[i]);
    }
}

// *** Khởi tạo màn hình LCD ***
/*
 * Khởi tạo LCD với các thông số mặc định.
 * @param lcd: Con trỏ đến cấu trúc LiquidCrystal_I2C.
 * @param _hi2c: Con trỏ đến cấu trúc I2C_HandleTypeDef (giao tiếp I2C).
 * @param addr: Địa chỉ I2C của LCD (thường là 0x27 hoặc 0x3F).
 */
void lcd_init(LiquidCrystal_I2C *lcd, I2C_HandleTypeDef *_hi2c, uint8_t addr)
{
    lcd->pI2c = _hi2c;
    lcd->Addr = addr << 1;  // Địa chỉ I2C (gồm cả bit đọc/ghi)
    lcd->DisplayControl |= LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;

    // Gửi chuỗi lệnh khởi tạo màn hình
    lcd_send_cmd(lcd, 0x33);  // Khởi động LCD ở chế độ 8-bit
    lcd_send_cmd(lcd, 0x32);  // Chuyển sang chế độ 4-bit
    HAL_Delay(5);
    lcd_send_cmd(lcd, LCD_CLEARDISPLAY);  // Xóa màn hình
    HAL_Delay(5);
    lcd_send_cmd(lcd, 0x0c);  // Bật hiển thị, tắt con trỏ
    HAL_Delay(5);
    lcd_send_cmd(lcd, LCD_RETURNHOME);  // Đưa con trỏ về vị trí gốc
    HAL_Delay(5);
    lcd_send_cmd(lcd, LCD_SETDDRAMADDR);  // Đặt địa chỉ DDRAM
    lcd_set_cursor_blink_off(lcd);  // Tắt nhấp nháy con trỏ
    lcd->Backlightval = LCD_BACKLIGHT;  // Bật đèn nền
}
