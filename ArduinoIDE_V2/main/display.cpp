#include "display.h"
#include "consts.h"
#include <lvgl.h>
#include <Wire.h>
#include <TFT_eSPI.h>  // By Bodmer V2.5.43
#include "src/ui.h"    // SquareLine Studio generated header


TFT_eSPI tft = TFT_eSPI();

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[240 * 20];
lv_disp_drv_t disp_drv;

void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);
  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, false);
  tft.endWrite();
  lv_disp_flush_ready(disp);
}


void Display::setup() {
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

  lv_disp_drv_register(&disp_drv);

  ui_init();
}


void Display::updateBattery(int percentage) {
  lv_label_set_text_fmt(ui_BatteryLabel, "%d%%", percentage);
  lv_bar_set_value(ui_Battery, percentage, LV_ANIM_OFF);
  if (percentage > 30) {
    lv_obj_set_style_bg_color(ui_Battery, lv_color_hex(0x66ff33), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(ui_Battery, lv_color_hex(0x134d00), LV_PART_MAIN);
  } else if (percentage > 20) {
    lv_obj_set_style_bg_color(ui_Battery, lv_color_hex(0xffff00), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(ui_Battery, lv_color_hex(0x4d4d00), LV_PART_MAIN);
  } else if (percentage > 10) {
    lv_obj_set_style_bg_color(ui_Battery, lv_color_hex(0xff9933), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(ui_Battery, lv_color_hex(0x994d00), LV_PART_MAIN);
  } else {
    lv_obj_set_style_bg_color(ui_Battery, lv_color_hex(0xff0000), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(ui_Battery, lv_color_hex(0x800000), LV_PART_MAIN);
  }
}

void Display::shutOffBacklight() {
  digitalWrite(TFT_BL_PIN, LOW);
}

void Display::rotateScreen(Orientation ori) {
  switch (ori) {
    case Orientation::DEG_0: tft.setRotation(0); break;
    case Orientation::DEG_90: tft.setRotation(1); break;
    case Orientation::DEG_180: tft.setRotation(2); break;
    case Orientation::DEG_270: tft.setRotation(3); break;
    default: tft.setRotation(0);
  }

  lv_obj_invalidate(lv_scr_act());
}

void Display::updateTimer(int remSeconds, int selSeconds) {
  int minutes = (int)remSeconds / 60;
  int seconds = (int)remSeconds % 60;
  lv_label_set_text_fmt(ui_Countdown, "%02d:%02d", minutes, seconds);
  int progress = 100 - ((remSeconds * 100) / selSeconds);
  lv_arc_set_value(ui_Arc1, progress);
}

unsigned long lastFinishChange = 0;
bool finishColorState = false;

void Display::cycleTimerFinish() {
  if (millis() - lastFinishChange >= 500) {
    if (finishColorState) lv_obj_set_style_arc_color(ui_Arc1, lv_color_hex(0x2095F6), LV_PART_INDICATOR);
    else lv_obj_set_style_arc_color(ui_Arc1, lv_color_hex(0xf55442), LV_PART_INDICATOR);
    finishColorState = !finishColorState;
    lastChange = millis();
  }
}