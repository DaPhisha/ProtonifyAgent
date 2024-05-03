/*
File: EventScheduler.h
Date Created: April 24, 2024
Authors: DaPhisha
Description: This header file declares the EventScheduler class, which facilitates non-blocking timed execution of tasks.
Version: 1.0.0
*/

#ifndef EVENTSCHEDULER_H
#define EVENTSCHEDULER_H

#include <Arduino.h>

struct Task {
    unsigned long interval;  // Interval between tasks in milliseconds
    unsigned long lastRun;   // Last run time in milliseconds
    void (*callback)();      // Pointer to the function to run
};

class EventScheduler {
private:
    Task *tasks;       // Array of tasks
    int taskCount;     // Number of tasks
    int capacity;      // Capacity of task array

public:
    EventScheduler(int capacity);
    ~EventScheduler();
    void addTask(unsigned long interval, void (*callback)());
    void run();
};

#endif // EVENTSCHEDULER_H
