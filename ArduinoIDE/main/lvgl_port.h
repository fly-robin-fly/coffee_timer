#ifndef LVGL_PORT_BSP_H
#define LVGL_PORT_BSP_H

void Lvgl_PortInit(void);

void Lcd_SetBacklight(uint8_t brig);

bool lvgl_lock(int timeout_ms);
void lvgl_unlock(void);

typedef enum {
    APP_DISP_ROT_0 = 0,
    APP_DISP_ROT_90,
    APP_DISP_ROT_180,
    APP_DISP_ROT_270
} app_disp_rot_t;

extern app_disp_rot_t g_current_rotation;
void App_SetRotation(app_disp_rot_t rotation);


#endif