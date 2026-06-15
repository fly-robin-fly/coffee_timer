#include "display.h"
#include "user_config.h"
#include "esp_err.h"
#include "lvgl_port.h"
#include "lvgl.h"
#include "src/ui.h"

void display_setup() {
  Lvgl_PortInit();
  Lcd_SetBacklight(70);
  if (lvgl_lock(0)) {
    ui_init();
    lvgl_unlock();
  }
}

void display_setOrientation(Orientation orientation) {
  switch (orientation) {
    case Orientation::DEGREES_0:
      lv_disp_set_rotation(NULL, LV_DISP_ROT_NONE);
      break;
    case Orientation::DEGREES_90:
      lv_disp_set_rotation(NULL, LV_DISP_ROT_90);
      break;
    case Orientation::DEGREES_180:
      lv_disp_set_rotation(NULL, LV_DISP_ROT_180);
      break;
    case Orientation::DEGREES_270:
      lv_disp_set_rotation(NULL, LV_DISP_ROT_270);
      break;
    default:
      lv_disp_set_rotation(NULL, LV_DISP_ROT_NONE);
  }
}

void display_updateTimer(int timer, int countFrom) {
  int minutes = (int)timer / 60;
  int seconds = (int)timer % 60;
  lv_label_set_text_fmt(ui_Countdown, "%02d:%02d", minutes, seconds);
  int progress = 100 - ((timer * 100) / countFrom);
  lv_arc_set_value(ui_Arc1, progress);
}

void display_enterSleepMode() {
  if (lvgl_lock(0)) {
    Lcd_Sleep();
    lvgl_unlock();
  }
}