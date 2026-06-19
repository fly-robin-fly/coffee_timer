#include "SensorQMI8658.hpp"  // By Lewis He
#include "consts.h"

SensorQMI8658 qmi;

void qmi_setup() {
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  // Initialize IMU
  if (!qmi.begin(Wire, QMI8658_L_SLAVE_ADDRESS, I2C_SDA_PIN, I2C_SCL_PIN)) {
    Serial.println("QMI8658 initialization failed!");
  }
}

void qmi_setupWakeup() {
  qmi.configWakeOnMotion();
}

bool qmi_getAccelerometer(float &ax, float &ay, float &az) {
  return qmi.getAccelerometer(ax, ay, az);
}