/*
File: Protonify Agent 2.0.0 ActionON24V.h File for IDE 2.3.2
Date Created: Dec 5, 2024
Authors: DaPhisha
Description: Set 24V to ON for output Circuit Type 
Version: 1.0.0
*/
// ActionTest.h
#ifndef ACTION_ON24V_H
#define ACTION_ON24V_H

#include "ActionObject.h"  // Include base class
#include "PortManager.h"   // Include PortManager for access to Ports structure
#include <Arduino.h>

// Derived class for testing actions
class ActionON24V : public ActionObject {
private:
    //String msg;  // Message to display

public:
    // Constructor to initialize with a port, priority, and message
    ActionON24V(Ports* port, int priority, String msg);

    // Override methods
    void init() override;
    void execute() override;
    void stop() override;
};

#endif // ACTION_ON24V_H