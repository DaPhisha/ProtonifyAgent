/*
File: EventScheduler.cpp
Date Created: April 24, 2024
Authors: DaPhisha
Description: This file implements the EventScheduler class, enabling timed, non-blocking execution of tasks.
Version: 1.0.0
*/
#include "EventScheduler.h"

EventScheduler::EventScheduler(int capacity) : taskCount(0), capacity(capacity) {
    tasks = new Task[capacity];
}

EventScheduler::~EventScheduler() {
    delete[] tasks;
}

void EventScheduler::addTask(unsigned long interval, void (*callback)()) {
    if (taskCount < this->capacity) {
        tasks[taskCount++] = {interval, millis(), callback};  // Set the lastRun to current time to avoid immediate trigger
    }
}

void EventScheduler::run() {
    unsigned long currentMillis = millis();
    for (int i = 0; i < taskCount; i++) {
        if (currentMillis - tasks[i].lastRun >= tasks[i].interval) {
            tasks[i].callback();
            tasks[i].lastRun = currentMillis;
        }
    }
}
