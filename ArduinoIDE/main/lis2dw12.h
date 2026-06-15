#pragma once

enum class Orientation {
  DEGREES_0,
  DEGREES_90,
  DEGREES_180,
  DEGREES_270,
  UNKNOWN
};

void lis2dw12_setup();
float lis2dw12_getRoll();
float lis2dw12_getPitch();
float lis2dw12_getTilt();
Orientation lis2dw12_getOrientation();