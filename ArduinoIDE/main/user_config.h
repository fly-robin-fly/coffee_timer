#ifndef USER_CONFIG_H
#define USER_CONFIG_H

//spi & i2c handle
#define LCD_HOST SPI2_HOST
#define TOUCH_HOST I2C_NUM_0

// touch I2C port
#define ESP32_SCL_NUM (GPIO_NUM_7)
#define ESP32_SDA_NUM (GPIO_NUM_8)

//  DISP

#define LCD_H_RES              466 
#define LCD_V_RES              466 
#define LVGL_BUF_HEIGHT        (LCD_V_RES / 4)

#define LCD_CS_PIN      22
#define LCD_PCLK_PIN    18
#define LCD_DATA0_PIN   19
#define LCD_DATA1_PIN   20
#define LCD_DATA2_PIN   10
#define LCD_DATA3_PIN   11
#define LCD_RST_PIN     21
#define BK_LIGHT        (-1)
#define LCD_TE_PIN      (-1)


#define TOUCH_ADDR            0x15
#define TOUCH_RST_PIN         (-1)
#define TOUCH_INT_PIN         (-1)


#define LVGL_TICK_PERIOD_MS    2
#define LVGL_TASK_MAX_DELAY_MS 500
#define LVGL_TASK_MIN_DELAY_MS 5
#define LVGL_TASK_STACK_SIZE   (6 * 1024)
#define LVGL_TASK_PRIORITY     2


//sys power io

#define SYS_POWER_IO_PIN 2

#define PWR_BUTTON_GPIO  GPIO_NUM_1

#define BacklightTestEN  0  //Backlight Test

#endif