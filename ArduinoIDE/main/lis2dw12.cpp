#include "lis2dw12.h"
#include "src/i2c_bsp/i2c_bsp.h"
#include "driver/i2c_master.h"
#include <Arduino.h>
#include <math.h>

#define LIS2DW12_ADDR 0x19

extern I2cMasterBus i2c_dev;
i2c_master_dev_handle_t lis2dw12_handle;  // Added missing handle

void lis2dw12_setup() {
  i2c_master_bus_handle_t g_i2c_bus_handle = i2c_dev.Get_I2cBusHandle();

  i2c_device_config_t dev_cfg = {};
  dev_cfg.dev_addr_length = I2C_ADDR_BIT_LEN_7;
  dev_cfg.scl_speed_hz = 300000;
  dev_cfg.device_address = LIS2DW12_ADDR;
  ESP_ERROR_CHECK(i2c_master_bus_add_device(g_i2c_bus_handle, &dev_cfg, &lis2dw12_handle));

  uint8_t reg = 0x0F;
  uint8_t id = 0;
  i2c_master_transmit_receive(lis2dw12_handle, &reg, 1, &id, 1, -1);

  if (id == 0x44) {
    Serial.println("LIS2DW12 Initialized in Low Power Mode!");

    // 1. CTRL1: 12.5Hz ODR, Low-Power Mode 4 (bits 4-3: 11)
    uint8_t ctrl1[2] = { 0x20, 0x30 };
    i2c_master_transmit(lis2dw12_handle, ctrl1, 2, -1);

    // 2. Configure Wake-Up Interrupts (Threshold, Duration, Routing, Latching)
    uint8_t int_cfg[][2] = {
      { 0x22, 0x10 },  // CTRL3: Enable LIR (Latch Interrupt Request) via bit 4
      { 0x34, 0x0A },  // WAKE_UP_THS: Increased threshold (~625mg) to ignore minor noise
      { 0x35, 0x03 },  // WAKE_UP_DUR: Duration (3 cycle)
      { 0x3F, 0x20 },  // CTRL7: Bit 5 (INTERRUPTS_ENABLE) must be 1
      { 0x23, 0x20 }   // CTRL4_INT1_PAD_CTRL: Route Wake-Up to INT1
    };

    // Update loop limit to 5
    for (int i = 0; i < 5; i++) {
      i2c_master_transmit(lis2dw12_handle, int_cfg[i], 2, -1);
    }

    // 3. Clear pending interrupts
    uint8_t dummy_data;
    uint8_t clear_reg = 0x2B | 0x80;  // WAKE_UP_SRC with auto-increment
    i2c_master_transmit_receive(lis2dw12_handle, &clear_reg, 1, &dummy_data, 1, -1);
  }
}

float angleTo360Degrees(float angle) {
  if (angle > 0) return angle;
  return 360 + angle;
}

Orientation angleToOrientation(float angle) {
  if (angle > 315 || angle <= 45) return Orientation::DEGREES_0;
  if (angle > 45 && angle <= 135) return Orientation::DEGREES_90;
  if (angle > 135 && angle <= 225) return Orientation::DEGREES_180;
  if (angle > 225 && angle <= 315) return Orientation::DEGREES_270;
  return Orientation::DEGREES_180;
}

float lis2dw12_getRoll() {
  uint8_t reg = 0x28 | 0x80;
  uint8_t data[6];

  if (i2c_master_transmit_receive(lis2dw12_handle, &reg, 1, data, 6, -1) == ESP_OK) {
    // x is omitted as it is unused in the roll calculation
    int16_t y = (int16_t)((data[3] << 8) | data[2]);
    int16_t z = (int16_t)((data[5] << 8) | data[4]);

    float roll = atan2((float)y, (float)z) * 180.0 / M_PI;
    roll = angleTo360Degrees(roll);
    return roll;
  }
  return -1;
}

float lis2dw12_getPitch() {
  uint8_t reg = 0x28 | 0x80;
  uint8_t data[6];

  if (i2c_master_transmit_receive(lis2dw12_handle, &reg, 1, data, 6, -1) == ESP_OK) {
    int16_t x = (int16_t)((data[1] << 8) | data[0]);
    int16_t y = (int16_t)((data[3] << 8) | data[2]);
    int16_t z = (int16_t)((data[5] << 8) | data[4]);

    // Pitch: Rotation around the Y-axis (nose up/down)
    float pitch = atan2((float)-x, sqrt((float)y * y + (float)z * z)) * 180.0 / M_PI;
    return angleTo360Degrees(pitch);
  }
  return -1;
}

float lis2dw12_getTilt() {
  uint8_t reg = 0x28 | 0x80;
  uint8_t data[6];

  if (i2c_master_transmit_receive(lis2dw12_handle, &reg, 1, data, 6, -1) == ESP_OK) {
    int16_t x = (int16_t)((data[1] << 8) | data[0]);
    int16_t y = (int16_t)((data[3] << 8) | data[2]);
    int16_t z = (int16_t)((data[5] << 8) | data[4]);

    // Tilt: Angle of the Z-axis relative to gravity
    float tilt = atan2(sqrt((float)x * x + (float)y * y), (float)z) * 180.0 / M_PI;
    return angleTo360Degrees(tilt);
  }
  return -1;
}

Orientation lis2dw12_getOrientation() {
  float roll = lis2dw12_getRoll();
  if (roll == -1) return Orientation::UNKNOWN;
  return angleToOrientation(roll);
}