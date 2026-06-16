#include <Arduino.h>
#include <Wire.h>
#include <TFT_eSPI.h> // By Bodmer
#include <lvgl.h>
#include "src/ui.h" // SquareLine Studio generated header
#include "SensorQMI8658.hpp" // By Lewis He

#define I2C_SDA_PIN   6
#define I2C_SCL_PIN   7
#define IMU_INT_PIN   GPIO_NUM_4
#define TFT_BL_PIN    2

TFT_eSPI tft = TFT_eSPI();
SensorQMI8658 qmi;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[240 * 20];
lv_disp_drv_t disp_drv;

// LVGL Display Flushing
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)&color_p->full, w * h, true);
    tft.endWrite();
    lv_disp_flush_ready(disp);
}

void enter_deep_sleep() {
    Serial.println("Face down: Entering Deep Sleep.");
    
    // 1. Turn off backlight
    digitalWrite(TFT_BL_PIN, LOW);
    
    // 2. Configure IMU to trigger INT1 on Any-Motion / Wake-on-Motion
    // Note: Method name depends on your specific QMI8658 library
    qmi.configWakeOnMotion(); 
    
    // 3. Set ESP32 to wake up when IMU pulls INT1 HIGH (use 0 if active LOW)
    esp_sleep_enable_ext0_wakeup(IMU_INT_PIN, 1);
    
    // 4. Enter Sleep
    esp_deep_sleep_start();
}

void setup() {
    Serial.begin(115200);
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    pinMode(TFT_BL_PIN, OUTPUT);
    digitalWrite(TFT_BL_PIN, HIGH);

    // Initialize IMU
    if (!qmi.begin(Wire, QMI8658_L_SLAVE_ADDRESS, I2C_SDA_PIN, I2C_SCL_PIN)) {
        Serial.println("QMI8658 initialization failed!");
    }

    // Initialize TFT
    tft.begin();
    tft.setRotation(0); // Let LVGL handle software rotation

    // Initialize LVGL
    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, 240 * 20);
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = 240;
    disp_drv.ver_res = 240;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    
    // Enable LVGL Software Rotation
    disp_drv.sw_rotate = 1; 
    disp_drv.rotated = LV_DISP_ROT_NONE;
    lv_disp_drv_register(&disp_drv);

    // Initialize SquareLine UI
    ui_init(); 
}

void loop() {
    lv_timer_handler(); // Process LVGL tasks
    delay(5);

    // Read Accelerometer (Assuming 1G = roughly 1.0 or 1000mG depending on lib)
    float ax, ay, az;
    if (qmi.getAccelerometer(ax, ay, az)) {
        
        // Z-Axis face down (< -0.8G) triggers sleep
        if (az < -0.8) {
            enter_deep_sleep();
        } 
        // Determine rotation by dominant gravity vector
        else if (ay > 0.8) {
            lv_disp_set_rotation(NULL, LV_DISP_ROT_NONE); // 0°
        }
        else if (ax > 0.8) {
            lv_disp_set_rotation(NULL, LV_DISP_ROT_90);   // 90°
        }
        else if (ay < -0.8) {
            lv_disp_set_rotation(NULL, LV_DISP_ROT_180);  // 180°
        }
        else if (ax < -0.8) {
            lv_disp_set_rotation(NULL, LV_DISP_ROT_270);  // 270°
        }
    }
}