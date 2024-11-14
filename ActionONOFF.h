/*
File: Protonify Agent 2.0.0 ActionONOFF.h File for IDE 2.3.2
Date Created: Sept 5 29, 2024
Authors: DaPhisha
Description: ONOFF Circuit Type 
Version: 1.0.0
*/
// ActionTest.h
#ifndef ACTION_ONOFF_H
#define ACTION_ONOFF_H

#include "ActionObject.h"  // Include base class
#include "PortManager.h"   // Include PortManager for access to Ports structure
#include <Arduino.h>

// Derived class for testing actions
class ActionONOFF : public ActionObject {
private:
    //String msg;  // Message to display

public:
    // Constructor to initialize with a port, priority, and message
    ActionONOFF(Ports* port, int priority, String msg);

    // Override methods
    void init() override;
    void execute() override;
    void stop() override;
};

#endif // ACTION_TEST_H


