/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "RTC.h"
#include "LiquidCrystal_I2C.h"
#include "Button.h"
#include "DFPLAYER.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */
RTC_Typedef hrtc1;
LiquidCrystal_I2C hlcd;
Button_Typdef start_stop_button;
Button_Typdef mode_button;
Button_Typdef light_button;
DFPLAYER_Name mp3;
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//--------define cac macro su dung o TRANG THAI IDLE----------------
typedef enum
{
	idle_state,
	choose_mode_state,
	adjust_time_state,
	set_alarm_state,
	sport_timer_state,
	choose_ringtone_state,
	adjust_volume_state,
}mode_state;
mode_state clock_state= idle_state;

typedef enum
{
	backlight_on,
	backlight_off,
}back_light_state;
back_light_state backlight_state=backlight_off;

//--------define cac macro su dung o TRANG THAI SET TIME------------
typedef enum
{
	choose_normal_display_mode,
	choose_adjust_time_mode,
	choose_set_alarm_mode,
	choose_sport_timer_mode,
	choose_choose_ringtone_mode,
	choose_adjust_volume_mode,
}choose_mode_t;
choose_mode_t choose_mode=choose_normal_display_mode;


//--------define cac macro su dung o TRANG THAI CHOOSE MODE------------
typedef enum
{
	date_adjusting,
	month_adjusting,
	year_adjusting,
	hour_adjusting,
	min_adjusting,
	second_adjusting,
}adjusting_list;
adjusting_list adjusting_state;

//--------define cac macro su dung o TRANG THAI SET ALARM-----------
typedef enum
{
	hour_setAlarm,
	min_setAlarm,
	second_setAlarm,
}alarm;
alarm alarm_state;

typedef enum
{
	alarm_on,
	alarm_off,
}alarm_on_off;
alarm_on_off alarm_on_or_off;

typedef struct
{
	uint8_t hour;
	uint8_t min;
	uint8_t second;
}alarm_time;
alarm_time alarmTime;
uint8_t alarming_flag;
//--------define cac macro su dung o TRANG THAI SPORT TIMER---------
typedef enum
{
	timer_pause,
	timer_run,
}timerState;
timerState timer_state;

typedef struct
{
	uint8_t min;
	uint8_t second;
	uint8_t tick;
}timerTime;
timerTime timer_time;
//--------define cac macro su dung o TRANG THAI CHOOSE RINGTONE---------
typedef enum
{
	song1=1,
	song2=2,
	song3=3,
}song_list_t;
song_list_t song_list;
int volume=15;
//-------------------------------------------------------------------------



void btn_press_short_callback(Button_Typdef *ButtonX ) // ngat duoc goi moi khi co 1 nut duoc nhan nha nhanh
{
	if (ButtonX == &mode_button) // neu nut duoc nhan la nut mode
	{
		HAL_Delay(10);
		lcd_clear_display(&hlcd);
		if (clock_state == idle_state)
		{
			clock_state=choose_mode_state;
			choose_mode= choose_normal_display_mode;
		}
		else if (clock_state == choose_mode_state)
		{
			switch(choose_mode)
			{
				case choose_normal_display_mode:
					clock_state= idle_state;
					break;
				case choose_adjust_time_mode:
					clock_state= adjust_time_state;
					adjusting_state= date_adjusting;
					break;
				case choose_set_alarm_mode:
					clock_state= set_alarm_state;
					alarm_state= second_setAlarm;
					alarmTime.hour=0;
					alarmTime.min=0;
					alarmTime.second=0;
					break;
				case choose_sport_timer_mode:
					clock_state= sport_timer_state;
					timer_state=timer_pause;
					break;
				case choose_choose_ringtone_mode:
					clock_state= choose_ringtone_state;
					song_list=song1;
					break;
				case choose_adjust_volume_mode:
					clock_state= adjust_volume_state;
					break;
				default:
					break;
			}
		}
		
		else if (clock_state == set_alarm_state) 
		{
			alarm_on_or_off=alarm_on;
			clock_state=idle_state;
		}
		else if (clock_state == adjust_volume_state)
		{
			DFPLAYER_Stop(&mp3);
			clock_state=idle_state;
		}
		else
		{
			clock_state=idle_state;
		}
	}
	if (ButtonX == &start_stop_button) // neu nut duoc nhan la nut start_stop
	{
		HAL_Delay(10);
		lcd_clear_display(&hlcd);
		if (clock_state== choose_mode_state)
		{
			switch(choose_mode)
			{
				case choose_normal_display_mode:
					choose_mode= choose_adjust_time_mode;
					break;
				case choose_adjust_time_mode:
					choose_mode= choose_set_alarm_mode;
					break;
				case choose_set_alarm_mode:
					choose_mode= choose_sport_timer_mode;
					break;
				case choose_sport_timer_mode:
					choose_mode= choose_choose_ringtone_mode;
					break;
				case choose_choose_ringtone_mode:
					choose_mode= choose_adjust_volume_mode;
					break;
				case choose_adjust_volume_mode:
					choose_mode= choose_normal_display_mode;
					break;
				default:
					break;
			}
		}
		if(clock_state == adjust_time_state)
		{
			switch(adjusting_state)
			{
				case date_adjusting:
					RTC_ReadTime(&hrtc1,&hrtc1.date_time);
					if (hrtc1.date_time.date==31) { hrtc1.date_time.date = 1;}
					else {hrtc1.date_time.date=(hrtc1.date_time.date)%31+1;}
					RTC_WriteTime(&hrtc1,&hrtc1.date_time);
					break;
				
				case month_adjusting:
					RTC_ReadTime(&hrtc1,&hrtc1.date_time);
					if (hrtc1.date_time.month==12) { hrtc1.date_time.month = 1;}
					else {hrtc1.date_time.month=(hrtc1.date_time.month)%12+1;}
					RTC_WriteTime(&hrtc1,&hrtc1.date_time);
					break;
				
				case year_adjusting:
					RTC_ReadTime(&hrtc1,&hrtc1.date_time);
					if (hrtc1.date_time.year==99) { hrtc1.date_time.year = 0;}
					else {hrtc1.date_time.year=(hrtc1.date_time.year++)%100;}
					RTC_WriteTime(&hrtc1,&hrtc1.date_time);
					break;
					
				case hour_adjusting:
					RTC_ReadTime(&hrtc1,&hrtc1.date_time);
					if (hrtc1.date_time.hour==23) { hrtc1.date_time.hour = 0;}
					else {hrtc1.date_time.hour=(hrtc1.date_time.hour++)%24;}
					RTC_WriteTime(&hrtc1,&hrtc1.date_time);
					break;
					
				case min_adjusting:
					RTC_ReadTime(&hrtc1,&hrtc1.date_time);
					if (hrtc1.date_time.min==59) { hrtc1.date_time.min = 0;}
					else {hrtc1.date_time.min=(hrtc1.date_time.min++)%60;}
					RTC_WriteTime(&hrtc1,&hrtc1.date_time);
					break;
					
				case second_adjusting:
					RTC_ReadTime(&hrtc1,&hrtc1.date_time);
					if (hrtc1.date_time.second==59) { hrtc1.date_time.second = 0;}
					else {hrtc1.date_time.second=(hrtc1.date_time.second++)%60;}
					RTC_WriteTime(&hrtc1,&hrtc1.date_time);
					break;
				
				default:
					break;
			}
		}
		if(clock_state==set_alarm_state)
		{
			switch(alarm_state)
			{
				case second_setAlarm:
					alarmTime.second= ((alarmTime.second)+1)%60;
					break;
				case min_setAlarm:
					alarmTime.min= ((alarmTime.min)+1)%60;
					break;
				case hour_setAlarm:
					alarmTime.hour= ((alarmTime.hour)+1)%24;
				break;
				
				default:
					break;
			}
		}
		if (clock_state== sport_timer_state)
		{
			switch(timer_state)
			{
				case timer_pause:
					timer_state= timer_run;
					break;
				case timer_run:
					timer_state= timer_pause;
					break;
				default:
					break;
			}
		}
		if (clock_state == choose_ringtone_state)
		{
			switch(song_list)
			{
				case song1:
					song_list=song3;
					break;
				case song3:
					song_list=song2;
					break;
				case song2:
					song_list=song1;
					break;
				default:
					break;
			}
		}
		if (clock_state == adjust_volume_state)
		{
			if (volume<30)
			{
				volume+=3;
			}
			DFPLAYER_SetVolume(&mp3,volume);
			DFPLAYER_PlayTrack(&mp3, 2);
		}
	}
	if (ButtonX== &light_button) // neu nut duoc nhan la nut light
	{
		HAL_Delay(10);
		lcd_clear_display(&hlcd);
		if (alarming_flag==1)
		{
			DFPLAYER_Stop(&mp3);
			alarm_on_or_off= alarm_off;
			alarming_flag=0;
		}
		if (clock_state == idle_state)
		{
			switch (backlight_state)
			{
				case backlight_off:
					lcd_backlight_on(&hlcd);
					backlight_state= backlight_on;
					break;
				case backlight_on:
					lcd_backlight_off(&hlcd);
					backlight_state= backlight_off;
					break;
				default:
					break;
			}
		}
		
		if (clock_state== choose_mode_state)
		{
			switch(choose_mode)
			{
				case choose_normal_display_mode:
					choose_mode= choose_adjust_volume_mode;
					break;
				case choose_adjust_time_mode:
					choose_mode= choose_normal_display_mode;
					break;
				case choose_set_alarm_mode:
					choose_mode= choose_adjust_time_mode;
					break;
				case choose_sport_timer_mode:
					choose_mode= choose_set_alarm_mode;
					break;
				case choose_choose_ringtone_mode:
					choose_mode= choose_sport_timer_mode;
					break;
				case choose_adjust_volume_mode:
					choose_mode= choose_choose_ringtone_mode;
					break;
				default:
					break;
			}
		}

		if (clock_state == adjust_time_state)
		{
			switch(adjusting_state) 
			{
				case date_adjusting:
					adjusting_state = month_adjusting;
				break;
				
				case month_adjusting:
					adjusting_state= year_adjusting;
				break;
				
				case year_adjusting:
					adjusting_state= hour_adjusting;
				break;
				
				case hour_adjusting:
					adjusting_state= min_adjusting;
				break;
				
				case min_adjusting:
					adjusting_state= second_adjusting;
				break;
				
				case second_adjusting:
					adjusting_state= date_adjusting;
				
				default:
					break;
					
			}
		}
		if (clock_state== set_alarm_state)
		{
			switch(alarm_state)
			{
				case second_setAlarm:
					alarm_state= min_setAlarm;
					break;
				case min_setAlarm:
					alarm_state= hour_setAlarm;
					break;
				case hour_setAlarm:
					alarm_state= second_setAlarm;
					break;
				default:
					break;
			}
		}
		if (clock_state== sport_timer_state)
		{
			timer_time.min=0;
			timer_time.second=0;
			timer_time.tick=0;
		}
		if (clock_state == choose_ringtone_state)
		{
			switch(song_list)
			{
				case song1:
					song_list=song2;
					break;
				case song2:
					song_list=song3;
					break;
				case song3:
					song_list=song1;
					break;
				default:
					break;
			}
		}
		if (clock_state == adjust_volume_state)
		{
			if(volume>0)
			{
				volume-=3;
			}
			DFPLAYER_SetVolume(&mp3,volume);
			DFPLAYER_PlayTrack(&mp3, 2);
		}
	}
}

void btn_press_timeout1s_callback(Button_Typdef *ButtonX) // ngat duoc goi moi khi co 1 nut duoc nhan giu trong 1s
{
	
}
void btn_press_timeout2s_callback(Button_Typdef *ButtonX) // ngat duoc goi moi khi co 1 nut duoc nhan giu trong 2s
{
	
}
void btn_press_timeout3s_callback(Button_Typdef *ButtonX) // ngat duoc goi moi khi co 1 nut duoc nhan giu trong 3s
{

}

void btn_press_timeout4s_callback(Button_Typdef *ButtonX) // ngat duoc goi moi khi co 1 nut duoc nhan giu trong 4s
{
	
}
void display_handle() // ham hien thi len LCD
{
	if (clock_state== idle_state)
	{
					RTC_ReadTime(&hrtc1,&hrtc1.date_time);
					lcd_set_cursor(&hlcd, 1,4);
					lcd_printf(&hlcd, "%02d:%02d:%02d",hrtc1.date_time.hour,
									hrtc1.date_time.min, hrtc1.date_time.second);
					lcd_set_cursor(&hlcd, 0,4);
					lcd_printf(&hlcd, "%02d-%02d-%02d",hrtc1.date_time.date,
									hrtc1.date_time.month,hrtc1.date_time.year);
	}

	if (clock_state== choose_mode_state)
	{
		switch(choose_mode)
			{
				case choose_normal_display_mode:
					lcd_set_cursor(&hlcd,0,2);
					lcd_printf(&hlcd,"-Enter Mode-");
					lcd_set_cursor(&hlcd,1,2);
					lcd_printf(&hlcd,"Time Display");
					break;
				case choose_adjust_time_mode:
					lcd_set_cursor(&hlcd,0,2);
					lcd_printf(&hlcd,"-Enter Mode-");
					lcd_set_cursor(&hlcd,1,2);
					lcd_printf(&hlcd,"Time Adjust");
					break;
				case choose_set_alarm_mode:
					lcd_set_cursor(&hlcd,0,2);
					lcd_printf(&hlcd,"-Enter Mode-");
					lcd_set_cursor(&hlcd,1,3);
					lcd_printf(&hlcd,"Set Alarm");
					break;
				case choose_sport_timer_mode:
					lcd_set_cursor(&hlcd,0,2);
					lcd_printf(&hlcd,"-Enter Mode-");
					lcd_set_cursor(&hlcd,1,3);
					lcd_printf(&hlcd,"Sport Timer");
					break;
				case choose_choose_ringtone_mode:
					lcd_set_cursor(&hlcd,0,2);
					lcd_printf(&hlcd,"-Enter Mode-");
					lcd_set_cursor(&hlcd,1,1);
					lcd_printf(&hlcd,"Choose Ringtone");
					break;
				case choose_adjust_volume_mode:
					lcd_set_cursor(&hlcd,0,2);
					lcd_printf(&hlcd,"-Enter Mode-");
					lcd_set_cursor(&hlcd,1,2);
					lcd_printf(&hlcd,"Volume Adjust");
					break;
				default:
					break;
			}
		
	}

	if (clock_state== adjust_time_state)
	{
		switch(adjusting_state)
		{
			case date_adjusting:
					RTC_ReadTime(&hrtc1,&hrtc1.date_time);
					lcd_set_cursor(&hlcd, 1,4);
					lcd_printf(&hlcd, "%02d:%02d:%02d",hrtc1.date_time.hour,
											hrtc1.date_time.min, hrtc1.date_time.second);
					lcd_set_cursor(&hlcd, 0,4);
					lcd_printf(&hlcd, "%02d-%02d-%02d",hrtc1.date_time.date,
									hrtc1.date_time.month,hrtc1.date_time.year);
					for( uint16_t i=0;i<10000;i++) {}
						RTC_ReadTime(&hrtc1,&hrtc1.date_time);
					lcd_set_cursor(&hlcd, 1,4);
					lcd_printf(&hlcd, "%02d:%02d:%02d",hrtc1.date_time.hour,
											hrtc1.date_time.min, hrtc1.date_time.second);
					lcd_set_cursor(&hlcd, 0,4);
					lcd_printf(&hlcd, "%s-%02d-%02d","  ",
									hrtc1.date_time.month,hrtc1.date_time.year);
					for( uint16_t i=0;i<10000;i++) {}
				break;
			
			case month_adjusting:
					RTC_ReadTime(&hrtc1,&hrtc1.date_time);
					lcd_set_cursor(&hlcd, 1,4);
					lcd_printf(&hlcd, "%02d:%02d:%02d",hrtc1.date_time.hour,
									hrtc1.date_time.min, hrtc1.date_time.second);
					lcd_set_cursor(&hlcd, 0,4);
					lcd_printf(&hlcd, "%02d-%02d-%02d",hrtc1.date_time.date,
							hrtc1.date_time.month,hrtc1.date_time.year);
					for( uint16_t i=0;i<10000;i++) {}
					RTC_ReadTime(&hrtc1,&hrtc1.date_time);
					lcd_set_cursor(&hlcd, 1,4);
					lcd_printf(&hlcd, "%02d:%02d:%02d",hrtc1.date_time.hour,
									hrtc1.date_time.min, hrtc1.date_time.second);
					lcd_set_cursor(&hlcd, 0,4);
					lcd_printf(&hlcd, "%02d-%s-%02d",hrtc1.date_time.date,
							"  ",hrtc1.date_time.year);
					for( uint16_t i=0;i<10000;i++) {}
				break;
			
			case year_adjusting:
					RTC_ReadTime(&hrtc1,&hrtc1.date_time);
					lcd_set_cursor(&hlcd, 1,4);
					lcd_printf(&hlcd, "%02d:%02d:%02d",hrtc1.date_time.hour,
									hrtc1.date_time.min, hrtc1.date_time.second);
					lcd_set_cursor(&hlcd, 0,4);
					lcd_printf(&hlcd, "%02d-%02d-%02d",hrtc1.date_time.date,
							hrtc1.date_time.month,hrtc1.date_time.year);
					for( uint16_t i=0;i<10000;i++) {}
					RTC_ReadTime(&hrtc1,&hrtc1.date_time);
					lcd_set_cursor(&hlcd, 1,4);
					lcd_printf(&hlcd, "%02d:%02d:%02d",hrtc1.date_time.hour,
									hrtc1.date_time.min, hrtc1.date_time.second);
					lcd_set_cursor(&hlcd, 0,4);
					lcd_printf(&hlcd, "%02d-%02d-%s",hrtc1.date_time.date,
							hrtc1.date_time.month,"  ");
					for( uint16_t i=0;i<10000;i++) {}
				break;
			
			case hour_adjusting:
					RTC_ReadTime(&hrtc1,&hrtc1.date_time);
					lcd_set_cursor(&hlcd, 1,4);
					lcd_printf(&hlcd, "%02d:%02d:%02d",hrtc1.date_time.hour,
									hrtc1.date_time.min, hrtc1.date_time.second);
					lcd_set_cursor(&hlcd, 0,4);
					lcd_printf(&hlcd, "%02d-%02d-%02d",hrtc1.date_time.date,
							hrtc1.date_time.month,hrtc1.date_time.year);
					for( uint16_t i=0;i<10000;i++) {}
					RTC_ReadTime(&hrtc1,&hrtc1.date_time);
					lcd_set_cursor(&hlcd, 1,4);
					lcd_printf(&hlcd, "%s:%02d:%02d","  ",
									hrtc1.date_time.min, hrtc1.date_time.second);
					lcd_set_cursor(&hlcd, 0,4);
					lcd_printf(&hlcd, "%02d-%02d-%02d",hrtc1.date_time.date,
							hrtc1.date_time.month,hrtc1.date_time.year);
					for( uint16_t i=0;i<10000;i++) {}
				break;
			
			case min_adjusting:
					RTC_ReadTime(&hrtc1,&hrtc1.date_time);
					lcd_set_cursor(&hlcd, 1,4);
					lcd_printf(&hlcd, "%02d:%02d:%02d",hrtc1.date_time.hour,
									hrtc1.date_time.min, hrtc1.date_time.second);
					lcd_set_cursor(&hlcd, 0,4);
					lcd_printf(&hlcd, "%02d-%02d-%02d",hrtc1.date_time.date,
							hrtc1.date_time.month,hrtc1.date_time.year);
					for( uint16_t i=0;i<10000;i++) {}
					RTC_ReadTime(&hrtc1,&hrtc1.date_time);
					lcd_set_cursor(&hlcd, 1,4);
					lcd_printf(&hlcd, "%02d:%s:%02d",hrtc1.date_time.hour,
									"  ", hrtc1.date_time.second);
					lcd_set_cursor(&hlcd, 0,4);
					lcd_printf(&hlcd, "%02d-%02d-%02d",hrtc1.date_time.date,
							hrtc1.date_time.month,hrtc1.date_time.year);
					for( uint16_t i=0;i<10000;i++) {}
				break;
			
			case second_adjusting:
					RTC_ReadTime(&hrtc1,&hrtc1.date_time);
					lcd_set_cursor(&hlcd, 1,4);
					lcd_printf(&hlcd, "%02d:%02d:%02d",hrtc1.date_time.hour,
									hrtc1.date_time.min, hrtc1.date_time.second);
					lcd_set_cursor(&hlcd, 0,4);
					lcd_printf(&hlcd, "%02d-%02d-%02d",hrtc1.date_time.date,
							hrtc1.date_time.month,hrtc1.date_time.year);
					for( uint16_t i=0;i<10000;i++) {}
					RTC_ReadTime(&hrtc1,&hrtc1.date_time);
					lcd_set_cursor(&hlcd, 1,4);
					lcd_printf(&hlcd, "%02d:%02d:%s",hrtc1.date_time.hour,
									hrtc1.date_time.min, "  ");
					lcd_set_cursor(&hlcd, 0,4);
					lcd_printf(&hlcd, "%02d-%02d-%02d",hrtc1.date_time.date,
							hrtc1.date_time.month,hrtc1.date_time.year);
					for( uint16_t i=0;i<10000;i++) {}
				break;
			
			default:
				break;
		}
	}
	
	if (clock_state == set_alarm_state)
	{
		if(alarm_state == second_setAlarm)
		{
			lcd_set_cursor(&hlcd,0,4);
			lcd_printf(&hlcd,"SETALARM");
			lcd_set_cursor(&hlcd,1,4);
			lcd_printf(&hlcd, "%02d:%02d:%02d",alarmTime.hour,alarmTime.min,alarmTime.second);
			for( uint16_t i=0;i<10000;i++) {}
			lcd_set_cursor(&hlcd,0,4);
			lcd_printf(&hlcd,"SETALARM");
			lcd_set_cursor(&hlcd,1,4);
			lcd_printf(&hlcd, "%02d:%02d:%s",alarmTime.hour,alarmTime.min,"  ");			
			for( uint16_t i=0;i<10000;i++) {}
		}
		if(alarm_state == min_setAlarm)
		{
			lcd_set_cursor(&hlcd,0,4);
			lcd_printf(&hlcd,"SETALARM");
			lcd_set_cursor(&hlcd,1,4);
			lcd_printf(&hlcd, "%02d:%02d:%02d",alarmTime.hour,alarmTime.min,alarmTime.second);
			for( uint16_t i=0;i<10000;i++) {}
			lcd_set_cursor(&hlcd,0,4);
			lcd_printf(&hlcd,"SETALARM");
			lcd_set_cursor(&hlcd,1,4);
			lcd_printf(&hlcd, "%02d:%s:%02d",alarmTime.hour,"  ",alarmTime.second);			
			for( uint16_t i=0;i<10000;i++) {}
		}
		if(alarm_state == hour_setAlarm)
		{
			lcd_set_cursor(&hlcd,0,4);
			lcd_printf(&hlcd,"SETALARM");
			lcd_set_cursor(&hlcd,1,4);
			lcd_printf(&hlcd, "%02d:%02d:%02d",alarmTime.hour,alarmTime.min,alarmTime.second);
			for( uint16_t i=0;i<10000;i++) {}
			lcd_set_cursor(&hlcd,0,4);
			lcd_printf(&hlcd,"SETALARM");
			lcd_set_cursor(&hlcd,1,4);
			lcd_printf(&hlcd, "%s:%02d:%02d","  ",alarmTime.min,alarmTime.second);		
			for( uint16_t i=0;i<10000;i++) {}
		}
	}
	if (clock_state== sport_timer_state)
	{
		lcd_set_cursor(&hlcd,0,4);
		lcd_printf(&hlcd," TIMER  ");
		lcd_set_cursor(&hlcd,1,4);
		lcd_printf(&hlcd, "%02d:%02d:%02d",timer_time.min,timer_time.second,timer_time.tick);
	}
	if (clock_state == choose_ringtone_state)
	{
		switch(song_list)
		{
			case song1:
				lcd_set_cursor(&hlcd,0,0);
				lcd_printf(&hlcd,"CHOOSE SONG 1:");
				lcd_set_cursor(&hlcd,1,0);
				lcd_printf(&hlcd, "BAC PHAN");
				break;
			case song2:
				lcd_set_cursor(&hlcd,0,0);
				lcd_printf(&hlcd,"CHOOSE SONG 2:");
				lcd_set_cursor(&hlcd,1,0);
				lcd_printf(&hlcd, "HONG NHAN");
				break;
			case song3:
				lcd_set_cursor(&hlcd,0,0);
				lcd_printf(&hlcd,"CHOOSE SONG 3:");
				lcd_set_cursor(&hlcd,1,0);
				lcd_printf(&hlcd, "SONG GIO");
				break;
			default:
				break;
		}
	}
	if (clock_state== adjust_volume_state)
	{
		lcd_set_cursor(&hlcd,0,2);
		lcd_printf(&hlcd,"SET VOLUME:");
		lcd_set_cursor(&hlcd,1,2);
		lcd_printf(&hlcd,"Volume: %d",volume*10/3);
	}
}

void check_alarm(alarm_time* time) // ham check xem gio hien tai da den gio bao thuc chua
{
	
	RTC_ReadTime(&hrtc1,&hrtc1.date_time);
	if (hrtc1.date_time.hour == time->hour && hrtc1.date_time.min == time->min && hrtc1.date_time.second == time->second)
	{
		alarming_flag=1;
		DFPLAYER_PlayTrack(&mp3,song_list);
		HAL_TIM_Base_Start(&htim1);
		alarm_on_or_off = alarm_off;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	timer_time.second++;
	if (timer_time.second==60)
	{
		timer_time.second=0;
		(timer_time.min)++;
	}
}

void timer_handle(timerTime* time ) // quan li dong ho bam gio
{
	if (timer_state== timer_pause)
	{
		HAL_TIM_Base_Stop_IT(&htim2);
	}
	if (timer_state== timer_run)
	{

		time->tick= (TIM2->CNT)/10;
		HAL_TIM_Base_Start_IT(&htim2);
	}
}
uint32_t temp1=0;
uint32_t temp2=0;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
	RTC_Init(&hrtc1, &hi2c1);                  // Khởi tạo module RTC DS3231
    lcd_init(&hlcd, &hi2c1, LCD_ADDR_DEFAULT); // Khởi tạo LCD với địa chỉ mặc định
    button_init(&start_stop_button, GPIOA, GPIO_PIN_3); // Khởi tạo nút Start/Stop
    button_init(&mode_button, GPIOA, GPIO_PIN_4);       // Khởi tạo nút Mode
    button_init(&light_button, GPIOA, GPIO_PIN_5);      // Khởi tạo nút Light
    DFPLAYER_Init(&mp3, &huart1);                       // Khởi tạo module MP3 DFPlayer
    DFPLAYER_SetVolume(&mp3, 20);                       // Đặt âm lượng mặc định
	
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		button_handle(&start_stop_button);
		button_handle(&mode_button);
		button_handle(&light_button);
		
		//DFPLAYER_Stop(&mp3);
		if (clock_state==sport_timer_state)
		{
			timer_handle(&timer_time);
		}
		display_handle();
		if (alarm_on_or_off == alarm_on)
		{
			check_alarm(&alarmTime);
		}
		if (alarming_flag==1)
		{
			if (HAL_GetTick() - temp1 >=40000)
			{
				DFPLAYER_PlayTrack(&mp3,song_list);
				temp1=HAL_GetTick();
			}
			if (HAL_GetTick() - temp2 >=500)
			{
				switch(backlight_state)
				{
					case backlight_on:
						lcd_backlight_off(&hlcd);
						backlight_state= backlight_off;
						break;
					case backlight_off:
						lcd_backlight_on(&hlcd);
						backlight_state= backlight_on;
						break;
					default:
						break;
				}
				temp2=HAL_GetTick();
			}
		}
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 7999;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : PA3 PA4 PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
