#include "types.h"
#include <lvgl.h>
#include <Wire.h>
#include <TFT_eSPI.h>  // By Bodmer
#include "src/ui.h"    // SquareLine Studio generated header

#define TFT_BL_PIN 2

TFT_eSPI tft = TFT_eSPI();

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[240 * 20];
lv_disp_drv_t disp_drv;

void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);
  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  tft.endWrite();
  lv_disp_flush_ready(disp);
}


void disp_setup() {
  pinMode(TFT_BL_PIN, OUTPUT);
  digitalWrite(TFT_BL_PIN, HIGH);
  // Initialize TFT
  tft.begin();
  tft.setRotation(0);  // Let LVGL handle software rotation

  // Initialize LVGL
  lv_init();
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, 240 * 20);
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = 240;
  disp_drv.ver_res = 240;
  disp_drv.flush_cb = disp_flush;
  disp_drv.draw_buf = &draw_buf;

  // Enable LVGL Software Rotation
  disp_drv.sw_rotate = 1;
  disp_drv.rotated = LV_DISP_ROT_NONE;
  lv_disp_drv_register(&disp_drv);

  // Initialize SquareLine UI
  ui_init();
}


void disp_setBattery(int percentage) {
  lv_label_set_text_fmt(ui_BatteryLabel, "%d%%", percentage);
}

void disp_shutOffBacklight() {
  digitalWrite(TFT_BL_PIN, LOW);
}

void disp_rotateScreen(Orientation ori) {
  switch (ori) {
    case Orientation::DEG_0:
      lv_disp_set_rotation(NULL, LV_DISP_ROT_NONE);
      break;
    case Orientation::DEG_90:
      lv_disp_set_rotation(NULL, LV_DISP_ROT_90);
      break;
    case Orientation::DEG_180:
      lv_disp_set_rotation(NULL, LV_DISP_ROT_180);
      break;
    case Orientation::DEG_270:
      lv_disp_set_rotation(NULL, LV_DISP_ROT_270);
      break;
    default:
      lv_disp_set_rotation(NULL, LV_DISP_ROT_NONE);
  }
}