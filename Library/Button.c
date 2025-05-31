#include "Button.h"

//--------------- Biến quản lý nút bấm ------------
/**
 * @brief Hàm callback khi nút bấm đang được nhấn (tùy chọn, có thể thay đổi trong ứng dụng).
 * @param ButtonX Con trỏ đến cấu trúc Button_Typdef chứa thông tin nút bấm.
 */
__weak void btn_pressing_callback(Button_Typdef *ButtonX)
{
}

/**
 * @brief Hàm callback khi nút bấm được nhấn ngắn (tùy chọn, có thể thay đổi trong ứng dụng).
 * @param ButtonX Con trỏ đến cấu trỏ Button_Typdef chứa thông tin nút bấm.
 */
__weak void btn_press_short_callback(Button_Typdef *ButtonX)
{
}

/**
 * @brief Hàm callback khi nút bấm được thả ra (tùy chọn, có thể thay đổi trong ứng dụng).
 * @param ButtonX Con trỏ đến cấu trúc Button_Typdef chứa thông tin nút bấm.
 */
__weak void btn_release_callback(Button_Typdef *ButtonX)
{
}

/**
 * @brief Hàm callback khi thời gian nhấn nút vượt quá ngưỡng (timeout).
 * @param ButtonX Con trỏ đến cấu trúc Button_Typdef chứa thông tin nút bấm.
 */
__weak void btn_press_timeout_callback(Button_Typdef *ButtonX)
{
}

/**
 * @brief Hàm xử lý tín hiệu nút bấm. Quản lý các trạng thái nhấn, thả, thời gian nhấn.
 * 
 * Hàm này thực hiện lọc tín hiệu nút bấm, phát hiện các sự kiện như nhấn ngắn, nhấn lâu hoặc thả nút.
 * Nó cũng xử lý các tình huống như debounce (giảm nhiễu tín hiệu).
 * 
 * @param ButtonX Con trỏ đến cấu trúc Button_Typdef chứa thông tin nút bấm.
 */
void button_handle(Button_Typdef *ButtonX)
{
    //------------------ Lọc nhiễu ------------------------
    uint8_t sta = HAL_GPIO_ReadPin(ButtonX->GPIOx, ButtonX->GPIO_Pin); // Đọc tín hiệu nút bấm
    if (sta != ButtonX->btn_filter) // Nếu tín hiệu thay đổi (lọc nhiễu)
    {
        ButtonX->btn_filter = sta;
        ButtonX->is_debouncing = 1; // Bắt đầu quá trình debounce
        ButtonX->time_deboune = HAL_GetTick(); // Ghi nhận thời gian bắt đầu debounce
    }

    //------------------ Tín hiệu đã xác lập ------------------------
    if (ButtonX->is_debouncing && (HAL_GetTick() - ButtonX->time_deboune >= 15)) // Kiểm tra thời gian debounce
    {
        ButtonX->btn_current = ButtonX->btn_filter; // Lưu giá trị tín hiệu đã lọc
        ButtonX->is_debouncing = 0; // Kết thúc debounce
    }

    //--------------------- Xử lý nhấn nút ------------------------
    if (ButtonX->btn_current != ButtonX->btn_last) // Kiểm tra nếu tín hiệu nút thay đổi
    {
        if (ButtonX->btn_current == 0) // Nếu nút được nhấn xuống (chân tín hiệu = 0)
        {
            ButtonX->is_press_timeout = 1; // Đặt cờ timeout nhấn nút
            btn_pressing_callback(ButtonX); // Gọi callback khi nút đang nhấn
            ButtonX->time_start_press = HAL_GetTick(); // Ghi nhận thời gian bắt đầu nhấn nút
        }
        else // Nếu nút được thả ra
        {
            if (HAL_GetTick() - ButtonX->time_start_press <= 1000) // Nếu thời gian nhấn nút nhỏ hơn 1 giây
            {
                btn_press_short_callback(ButtonX); // Gọi callback khi nhấn ngắn
            }
            btn_release_callback(ButtonX); // Gọi callback khi nút được thả ra
            ButtonX->is_press_timeout = 0; // Đặt lại cờ timeout
        }
        ButtonX->btn_last = ButtonX->btn_current; // Cập nhật trạng thái nút cuối cùng
    }

    //------------- Xử lý nhấn giữ nút ----------------
    if (ButtonX->is_press_timeout && (HAL_GetTick() - ButtonX->time_start_press >= 3000)) // Nếu nhấn giữ lâu hơn 3 giây
    {
        btn_press_timeout_callback(ButtonX); // Gọi callback khi nhấn giữ quá lâu
        ButtonX->is_press_timeout = 0; // Đặt lại cờ timeout
    }
}

/**
 * @brief Hàm khởi tạo cấu trúc Button và các thông số GPIO.
 * @param ButtonX Con trỏ đến cấu trúc Button_Typdef cần khởi tạo.
 * @param GPIOx Con trỏ đến cổng GPIO liên quan đến nút bấm.
 * @param GPIO_Pin Chân GPIO cụ thể của nút bấm.
 */
void button_init(Button_Typdef *ButtonX, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    ButtonX->GPIOx = GPIOx; // Cập nhật cổng GPIO
    ButtonX->GPIO_Pin = GPIO_Pin; // Cập nhật chân GPIO
}
