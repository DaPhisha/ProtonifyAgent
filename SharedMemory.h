#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#define LOG_BUFFER_SIZE 1024  // Define the size of the log buffer
#define MAX_LOG_ENTRY_LENGTH 128  // Maximum length of a single log entry

#ifdef ARDUINO_PORTENTA_H7_M7  // Check if compiling for M7 core
#include "PortManager.h"  // Relative path to PortManager.h for M7 core
#endif

#ifdef ARDUINO_PORTENTA_H7_M4  // Check if compiling for M7 core
#include "../PortManager.h"  // Relative path to PortManager.h for M7 core
#endif

// Shared log buffer in TCM memory
__attribute__((section(".bss.$TCM"))) volatile char logBuffer[LOG_BUFFER_SIZE];
__attribute__((section(".bss.$TCM"))) volatile int logWriteIndex = 0;  // Index for writing to the buffer
__attribute__((section(".bss.$TCM"))) volatile int logReadIndex = 0;   // Index for reading from the buffer

// Lock for synchronizing access
__attribute__((section(".bss.$TCM"))) volatile bool logBufferLock = false;
/*
1024/128 = 8 log entries per buffer cycle

*/
__attribute__((section(".bss.$TCM")))
volatile Ports sharedPorts[MAX_PORTS];  // Shared ports array

#endif // SHARED_MEMORY_H