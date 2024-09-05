/*
File: Protonify Agent 2.0.0 Main .ino File for IDE 2.3.2
Date Created: Sept 5 29, 2024
Authors: DaPhisha
Description: Action Queue for processing priority tasks
Version: 1.0.0
*/
// ActionQueue.cpp
#include "ActionQueue.h"

// Constructor
ActionQueue::ActionQueue() {}

// Destructor
ActionQueue::~ActionQueue() {
    clearActions();  // Clean up dynamically allocated actions
}

// Add a new action to the queue
void ActionQueue::addAction(ActionObject* action) {
    actions.push_back(action);
}

// Sort actions by priority (higher priority first)
void ActionQueue::sortActions() {
    std::sort(actions.begin(), actions.end(), [](ActionObject* a, ActionObject* b) {
        return a->getPriority() > b->getPriority();
    });
}

// Execute all actions in the queue
void ActionQueue::executeActions() {
    for (auto& action : actions) {
        action->execute();
    }
}

// Clear the queue and delete all actions
void ActionQueue::clearActions() {
    for (auto& action : actions) {
        delete action;
    }
    actions.clear();
}
