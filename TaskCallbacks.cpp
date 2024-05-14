/*
File: TaskCallbacks.cpp
Date Created: April 24, 2024
Authors: DaPhisha
Description: This file defines the implementation of various task callback functions for scheduled tasks.
Version: 1.0.0
*/

#include "TaskCallbacks.h"


mbed_stats_heap_t heapStatsHistory[MAX_HEAPS];  // Array to store last 5 heap statistics
int heapStatsIndex = 0;  // Index for circular buffer

std::vector<char*> allocations; // Store pointers to keep track of allocated memory

//used to test allocating memory and see what happens on overflow
void allocateMemory() {
    static const size_t allocationSize = 8192; // Change the size based on your test requirements e.g., 2048, 4096, 8192
    char* block = nullptr;

    try {
        block = new (std::nothrow) char[allocationSize];
        if (block != nullptr) {
            memset(block, 0, allocationSize);
            allocations.push_back(block);
            LogManager::getInstance().writeLog("Allocated " + String(allocationSize) + " bytes, total: " + String(allocations.size() * allocationSize));
        } else {
            // Allocation failed, handle the error
            throw std::runtime_error("****** WARNING****** Memory allocation failed");
        }
    } catch (const std::bad_alloc& e) {
        // Catch any other type of allocation failure not caught by std::nothrow
        LogManager::getInstance().writeLog("******SYSTEM WARNING****** Exception during memory allocation: " + String(e.what()));
    } catch (const std::exception& e) {
        // Handle any other exceptions, potentially from LogManager or other parts of the try block
        LogManager::getInstance().writeLog("******SYSTEM WARNING****** Exception: " + String(e.what()));
    } catch (...) {
        // Catch all other types of exceptions
        LogManager::getInstance().writeLog("******SYSTEM WARNING****** An unexpected error occurred during memory allocation.");
    }

    // Optionally, attempt recovery or further actions if allocation fails
    if (block == nullptr) {
        LogManager::getInstance().writeLog("******SYSTEM WARNING****** Failed to allocate memory. Total successful allocation: " + String(allocations.size() * allocationSize) + " bytes");
        // Consider freeing some previously allocated memory if critical
        freeSomeMemory();
        // Retry allocation or adjust system behavior
        // retryAllocation();
    }
}

void freeSomeMemory() {
    if (!allocations.empty()) {
        delete[] allocations.back();
        allocations.pop_back();
        LogManager::getInstance().writeLog("Freed memory to cope with allocation failure.");
    }
}

// Define retryAllocation if you decide it's appropriate for your application
// void retryAllocation() { ... }

void printSettings(){
   LOG("CALLBACK -- Admin Settings");
   LOG(PortManager::getInstance().AllPortsToString());

}

void writeSettingsToFlash(){
    LOG("CALLBACK -- WRITING FLASH");
    PortManager::getInstance().writeToFlash();
}


void checkNetwork() {
    // Example function that checks network status

    if (!NetworkManager::getInstance().checkNetworkConnection()) {
        LOG("******************Network check failed, trying to reconnect...");
    }
}

void checkSerialConnection() {
    // Example function that checks the serial connection status
    LogManager::getInstance().writeLog("Checking serial bus...");
}

  /*
  The Portenta Machine Edition includes an STM32H747XI SoC with a high-performance DSP, Arm Cortex-M7 + Cortex-M4 MCU, with 2MBytes of Flash memory
  */

void checkMemory() {
    LogManager::getInstance().writeLog("****************Checking Memory****************");
    mbed_stats_heap_t heap_stats;
    mbed_stats_heap_get(&heap_stats);

    // Update the circular buffer with new stats
    heapStatsHistory[heapStatsIndex] = heap_stats;
    heapStatsIndex = (heapStatsIndex + 1) % MAX_HEAPS;  // Move to next position in a circular manner

    float usagePercentage = (float)heap_stats.current_size / heap_stats.reserved_size * 100.0;
    LogManager::getInstance().writeLog("Heap Used: " + String(heap_stats.current_size) + 
                                       " bytes (" + String(usagePercentage, 2) + "% of total reserved memory)");

    // Check if memory usage is nearing full capacity
    if (usagePercentage > memoryWarningThreshold) {
        LogManager::getInstance().writeLog("******SYSTEM WARNING****** Memory usage is at " + String(usagePercentage, 2) + 
                                           "% of total reserved memory. Close to or exceeding the threshold.");
    }

    // Print all heap stats in the history
    printHeapStatsHistory();
    printFlashStats();
}

void printFlashStats(){

  FlashIAPLimits limits = getFlashIAPLimits();  // Get the current flash memory limits

    String flashStringData = "Read flash memory. \nFlash Size: " + String(limits.flash_size) + "\nFlash Available: " + String(limits.available_size) + "\nStart Address: " + String(limits.start_address);
    LOG(flashStringData);
}

void printHeapStatsHistory() {
    LogManager::getInstance().writeLog("Heap Stats History:");
    int currentIndex = (heapStatsIndex + MAX_HEAPS - 1) % MAX_HEAPS;  // Start with the most recent complete record

    for (int i = 0; i < MAX_HEAPS; i++) {
        mbed_stats_heap_t& currentStat = heapStatsHistory[currentIndex];
        float usagePercentage = (float)currentStat.current_size / currentStat.reserved_size * 100.0;
        int deltaIndex = (currentIndex + MAX_HEAPS - 1) % MAX_HEAPS;  // Previous index in a circular manner
        int delta = currentStat.current_size - heapStatsHistory[deltaIndex].current_size;

        String label = i == 0 ? "Current" : "Current -" + String(i);
        String message = label + ": " +
                         "Used: " + String(currentStat.current_size) + " bytes (" +
                         String(usagePercentage, 2) + "% of total), " +
                         "Delta: " + (delta >= 0 ? "+" : "") + String(delta) + " bytes, " +
                         "Reserved: " + String(currentStat.reserved_size) + " bytes, " +
                         "Max: " + String(currentStat.max_size) + " bytes";

        LogManager::getInstance().writeLog(message);

        currentIndex = deltaIndex;  // Move to the next older record
    }
}

