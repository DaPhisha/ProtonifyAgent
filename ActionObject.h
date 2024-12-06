/*
File: Protonify Agent 2.0.0 Main .ino File for IDE 2.3.2
Date Created: Sept 5 29, 2024
Authors: DaPhisha
Description: Action Object for execution of priority tasks
Version: 1.0.0
*/
// ActionObject.h
#ifndef ACTION_OBJECT_H
#define ACTION_OBJECT_H

#include "PortManager.h"  // Include PortManager to access the Port data
#include <Arduino_PortentaMachineControl.h>

// Base class for all action objects
class ActionObject {
protected:
    Ports* port;  // Pointer to the port associated with this action (if any)
    int priority;  // Priority of the action
    String msg; //msg payload

public:
    // Constructor that optionally takes a port , priority and message
    ActionObject(Ports* port = nullptr, int priority = 0, String msg = "") : port(port), priority(priority),msg(msg) {}

    // Virtual destructor for proper cleanup of derived classes
    virtual ~ActionObject() = default;

    // Initialize the action (can be overridden in derived classes)
    virtual void init() {
        // Default init code (override in derived classes if needed)
    }

    // Pure virtual function for executing the action (must be implemented by derived classes)
    virtual void execute() = 0;

    // Stop the action (can be overridden in derived classes)
    virtual void stop() {
        // Default stop code (override in derived classes if needed)
    }

    // Get the priority of the action
    int getPriority() const { return priority; }

    // Set the priority of the action
    void setPriority(int newPriority) { priority = newPriority; }
};

#endif // ACTION_OBJECT_H
