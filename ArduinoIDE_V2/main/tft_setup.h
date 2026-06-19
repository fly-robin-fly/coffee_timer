#pragma once

#define USER_SETUP_INFO "Waveshare_128"
#define GC9A01_DRIVER
#define TFT_WIDTH  240
#define TFT_HEIGHT 240

#define TFT_MOSI 11
#define TFT_SCLK 10
#define TFT_CS   9
#define TFT_DC   8
#define TFT_RST  14
#define TFT_BL   2

// Required by LVGL / TFT_eSPI bridge
#define LOAD_GLCD
#define LOAD_FONT2
#define SPI_FREQUENCY  27000000