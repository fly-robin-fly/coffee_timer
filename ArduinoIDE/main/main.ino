#include "user_config.h"
#include "esp_err.h"
#include "lvgl_port.h"
#include "lvgl.h"
#include "src/ui.h"
#include "driver/i2c_master.h"  // Include ESP-IDF I2C driver
#include <math.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

SemaphoreHandle_t i2c_mutex;
extern i2c_master_bus_handle_t g_i2c_bus_handle;  // The handle we exposed
i2c_master_dev_handle_t lis2dw12_handle;          // Device handle for the sensor

#define LIS2DW12_ADDR 0x19

void user_ui_init(void) {
  ui_init();
}

void setup() {
  Serial.begin(115200);
  i2c_mutex = xSemaphoreCreateMutex();

  // 1. Initialize LVGL FIRST (This initializes the native I2C bus and sets g_i2c_bus_handle)
  Lvgl_PortInit();
  if (lvgl_lock(0)) {
    user_ui_init();
    lvgl_unlock();
  }

  // 2. Register the LIS2DW12 on the same bus
  // i2c_device_config_t dev_cfg = {};
  // dev_cfg.dev_addr_length = I2C_ADDR_BIT_LEN_7;
  // dev_cfg.scl_speed_hz = 300000;
  // dev_cfg.device_address = LIS2DW12_ADDR;
  // ESP_ERROR_CHECK(i2c_master_bus_add_device(g_i2c_bus_handle, &dev_cfg, &lis2dw12_handle));

  // // 3. Initialize the Sensor (Wrap in mutex since LVGL is now polling)
  // if (xSemaphoreTake(i2c_mutex, portMAX_DELAY) == pdTRUE) {

  //   // Read WHO_AM_I register (0x0F)
  //   uint8_t reg = 0x0F;
  //   uint8_t id = 0;
  //   i2c_master_transmit_receive(lis2dw12_handle, &reg, 1, &id, 1, -1);

  //   if (id == 0x44) {
  //     Serial.println("LIS2DW12 Initialized natively via ESP-IDF!");
  //     // Write CTRL1 (0x20) -> Set 100Hz ODR, High-Performance Mode (0x54)
  //     uint8_t init_cmd[2] = { 0x20, 0x54 };
  //     i2c_master_transmit(lis2dw12_handle, init_cmd, 2, -1);
  //   } else {
  //     Serial.printf("LIS2DW12 Init Failed. WHO_AM_I = 0x%02X\n", id);
  //   }
  //   xSemaphoreGive(i2c_mutex);
  // }
}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(1000));
  App_SetRotation(APP_DISP_ROT_0);
  vTaskDelay(pdMS_TO_TICKS(1000));
  App_SetRotation(APP_DISP_ROT_90);
  vTaskDelay(pdMS_TO_TICKS(1000));
  App_SetRotation(APP_DISP_ROT_180);
  vTaskDelay(pdMS_TO_TICKS(1000));
  App_SetRotation(APP_DISP_ROT_270);
  // Serial.println("ping");

  // float roll, pitch;
  // bool success = false;

  // if (i2c_mutex != NULL && xSemaphoreTake(i2c_mutex, pdMS_TO_TICKS(10)) == pdTRUE) {

  //   uint8_t reg = 0x28 | 0x80;
  //   uint8_t data[6];

  //   esp_err_t ret = i2c_master_transmit_receive(lis2dw12_handle, &reg, 1, data, 6, -1);

  //   if (ret == ESP_OK) {
  //     // LIS2DW12 is little-endian
  //     int16_t x = (int16_t)((data[1] << 8) | data[0]);
  //     int16_t y = (int16_t)((data[3] << 8) | data[2]);
  //     int16_t z = (int16_t)((data[5] << 8) | data[4]);

  //     // Serial.printf("%d %d %d\n", x, y, z);
  //     pitch = atan2((-x), sqrt((float)y * y + (float)z * z)) * 180.0 / M_PI;
  //     roll = atan2((float)y, (float)z) * 180.0 / M_PI;

  //     Serial.printf("Pitch: %.2f | Roll: %.2f\n", pitch, roll);
  //     success = true;
  //   } else {
  //     Serial.println("Native I2C Read Failed.");
  //   }

  //   xSemaphoreGive(i2c_mutex);
  // }
  // if (success) {
  //   if (lvgl_lock(0)) {
  //     // Use this specific function to update the existing style rotation property
  //     // without re-allocating style memory.
  //     lv_obj_set_style_transform_rotation(ui_Battery, (int32_t)(roll * 10), LV_PART_MAIN | LV_STATE_DEFAULT);

  //     lvgl_unlock();
  //     Serial.println("display done...");
  //   }
  // }
}