/*
File: TaskCallbacks.h
Date Created: April 24, 2024
Authors: DaPhisha
Description: This file declares various task callback functions for scheduled tasks in the Arduino project.
Version: 1.0.0
*/

#ifndef TASKCALLBACKS_H
#define TASKCALLBACKS_H
#include <mbed.h>
#include "PortManager.h"
#define MAX_HEAPS 5  //max number of heaps to display
const float memoryWarningThreshold = 85.0; //set the threshold to 85.0
extern mbed_stats_heap_t heapStatsHistory[MAX_HEAPS];  // Array to store last 5 heap statistics
extern int heapStatsIndex;  // Index for circular buffer

// Function declarations for callbacks
void allocateMemory();
void checkNetwork();
void checkSerialConnection();
void checkMemory();
void printHeapStatsHistory();
void freeSomeMemory();
void printFlashStats();
void printSettings();
void writeSettingsToFlash();

#endif // TASKCALLBACKS_H
