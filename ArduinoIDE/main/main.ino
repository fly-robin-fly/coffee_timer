// #include "user_config.h"
// #include "esp_err.h"
// #include "lvgl_port.h"
// #include "lvgl.h"
// #include "src/ui.h"
#include <Wire.h>
#include <LIS2DW12Sensor.h>

#define I2C_SDA_PIN 8
#define I2C_SCL_PIN 7


// Initialize the I2C bus
TwoWire dev_i2c(0);


// Create the sensor instance
LIS2DW12Sensor Accelero(&dev_i2c);


// void user_ui_init(void) {
//   // Call your SquareLine init here
//   ui_init();
// }

void setup() {
  Serial.begin(9600);

  dev_i2c.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  // This initializes the display hardware and the LVGL port
  // Lvgl_PortInit();

  // // The Waveshare port uses a mutex lock to prevent display corruption
  // if (lvgl_lock(0)) {
  //   user_ui_init();
  //   lvgl_unlock();
  // }

  // Initialize Sensor
  if (Accelero.begin() != 0) {
    Serial.println("Failed to initialize LIS2DW12!");
    while (1)
      ;
  }

  Accelero.Enable_X();
  Serial.println("LIS2DW12 Initialized.");
}


void loop() {
  vTaskDelay(pdMS_TO_TICKS(500));

  int32_t accelerometer[3];

  // PROTECT THE BUS:
  // Before accessing I2C, you must acquire the lock that the
  // display/touch driver uses.
  // if (lvgl_lock(0)) {

  // Now that we own the bus, the touch controller
  // will wait until we call lvgl_unlock()
  Accelero.Get_X_Axes(accelerometer);

  // Release the bus immediately
  // lvgl_unlock();
  // }

  Serial.printf("X: %d | Y: %d | Z: %d\n", accelerometer[0], accelerometer[1], accelerometer[2]);
}