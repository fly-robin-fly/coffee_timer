#include "qmi.h"
#include "SensorQMI8658.hpp"  // By Lewis He V0.4.1
#include "consts.h"

SensorQMI8658 qmi;

void QMI::setup() {
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  // Initialize IMU
  if (!qmi.begin(Wire, QMI8658_L_SLAVE_ADDRESS, I2C_SDA_PIN, I2C_SCL_PIN)) {
    Serial.println("QMI8658 initialization failed!");
  }

  qmi.configAccelerometer(
      SensorQMI8658::ACC_RANGE_4G,      
      SensorQMI8658::ACC_ODR_1000Hz,    
      SensorQMI8658::LPF_MODE_0         
  );

  qmi.enableAccelerometer();
}

void QMI::setupWakeup() {
  qmi.configWakeOnMotion(
    200,                                   // WoMThreshold
    SensorQMI8658::ACC_ODR_LOWPOWER_128Hz, // Energy efficient frequency
    SensorQMI8658::INTERRUPT_PIN_1,        // Interrupt pin 1
    0                                      // Normally 0
  );
  float dummyX, dummyY, dummyZ;
  qmi.getAccelerometer(dummyX, dummyY, dummyZ);
}

bool QMI::getAccelerometer(float &ax, float &ay, float &az) {
  return qmi.getAccelerometer(ax, ay, az);
}