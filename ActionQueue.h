/*
File: Protonify Agent 2.0.0 Main .ino File for IDE 2.3.2
Date Created: Sept 5 29, 2024
Authors: DaPhisha
Description: Action Queue for execution of priority tasks
Version: 1.0.0
*/
// ActionQueue.h
#ifndef ACTION_QUEUE_H
#define ACTION_QUEUE_H

#include "ActionObject.h"
#include <vector>
#include <algorithm>  // For std::sort

// ActionQueue class to manage actions
class ActionQueue {
private:
    std::vector<ActionObject*> actions;  // Vector of action objects

public:
    // Constructor
    ActionQueue();

    // Destructor
    ~ActionQueue();

    // Add a new action to the queue
    void addAction(ActionObject* action);

    // Sort actions by priority (higher priority first)
    void sortActions();

    // Execute all actions in the queue
    void executeActions();

    // Clear the queue and delete all actions
    void clearActions();
};

#endif // ACTION_QUEUE_H
