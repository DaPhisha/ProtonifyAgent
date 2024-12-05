/*
File: Protonify Agent 2.0.0 ActionMA420.h File for IDE 2.3.2
Date Created: Sept 5 29, 2024
Authors: DaPhisha
Description: MA420 Action Object 
Version: 1.0.0
*/
// ActionMA420.h
#ifndef ACTION_MA420_H
#define ACTION_MA420_H

#include "ActionObject.h"  // Base class
#include "PortManager.h"   // Include PortManager to access Ports structure
#include <Arduino.h>       // Arduino functions and types

// Derived class for handling MA420 actions
class ActionMA420 : public ActionObject {
private:
    //String msg;  // Custom message to be used in logging

public:
    // Constructor to initialize with a port, priority, and message
    ActionMA420(Ports* port, int priority, String msg);

    // Override methods
    void init() override;
    void execute() override;
    void stop() override;
};

#endif // ACTION_MA420_H
