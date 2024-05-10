/*
  File: FlashAPILimits.cpp
  Created: Sept 6, 2023
  Version: release candidate 001
  Author: DaPhisha
  Version: 1.0.0

  Taken from Arduino Website:
  https://docs.arduino.cc/tutorials/portenta-h7/reading-writing-flash-memory#mbed-os-apis-for-flash-storage
*/

#include "FlashAPILimits.h"

// Get the actual start address and available size for the FlashIAP Block Device
// considering the space already occupied by the sketch (firmware).
FlashIAPLimits getFlashIAPLimits()
{
  // Alignment lambdas
  auto align_down = [](uint64_t val, uint64_t size) {
    return (((val) / size)) * size;
  };
  auto align_up = [](uint32_t val, uint32_t size) {
    return (((val - 1) / size) + 1) * size;
  };

  size_t flash_size;
  uint32_t flash_start_address;
  uint32_t start_address;
  FlashIAP flash;

  // Starts the flash interface which prepares it for reading/writing

  auto result = flash.init();
  if (result != 0)
    return { }; // If initialization fails, return an empty struct

  // Find the start of first sector after text area
  // Computes the start address of the flash memory right after the application code
  int sector_size = flash.get_sector_size(FLASHIAP_APP_ROM_END_ADDR);
  start_address = align_up(FLASHIAP_APP_ROM_END_ADDR, sector_size);

  // Retrieves the starting physical address of the flash memory and its total size
  flash_start_address = flash.get_flash_start();
  flash_size = flash.get_flash_size();
  
  // Deinitializes the flash object, which is good practice to free up resources
  result = flash.deinit();

  // Calculates the available size for data storage by subtracting the used space
  int available_size = flash_start_address + flash_size - start_address;
  if (available_size % (sector_size * 2)) {
    available_size = align_down(available_size, sector_size * 2); // Ensures the size aligns properly with sector boundaries
  }
  // Returns the filled FlashIAPLimits struct with size, start address, and available size
  return { flash_size, start_address, available_size };

}