/*
  File: FlashAPILimits.h
  Created: Sept 6, 2023
  Version: 1.0.1
  Author: DaPhisha
  

  Taken from Arduino Website:
  https://docs.arduino.cc/tutorials/portenta-h7/reading-writing-flash-memory#mbed-os-apis-for-flash-storage
*/
// Ensures that this file is only included once
#pragma once 

#include <Arduino.h>
#include <FlashIAP.h>
#include <FlashIAPBlockDevice.h>

using namespace mbed;

// A helper struct for FlashIAP limits
struct FlashIAPLimits {
  size_t flash_size;
  uint32_t start_address;
  uint32_t available_size;
};

FlashIAPLimits getFlashIAPLimits();