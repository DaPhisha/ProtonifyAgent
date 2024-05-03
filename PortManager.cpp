/*
File: PortManager.cpp
Date Created: April 29, 2024
Authors: DaPhisha
Description: Implementation of the PortManager class which handles the lifecycle and state management of Port objects, including interactions with flash memory for persistence.
Version: 1.0.0
*/

#include "PortManager.h"
#include "LogManager.h"

PortManager::PortManager(){
  //do nothing must call INIT
  isInitialized = false;
}
void PortManager::init() {
    if (!loadFromFlash()) {
        initializeDefaults();
    }
    isInitialized = true;
}
PortManager& PortManager::getInstance() {
    static PortManager instance; // Guaranteed to be destroyed and instantiated on first use
    return instance;
}

PortManager::~PortManager() {
    writeToFlash();
    // Iterate through the array and delete each Port object
    if (settings.ports) {  // Ensure ports is not nullptr
        for (int i = 0; i < MAX_PORTS; i++) {
            if (settings.ports[i]) {  // Check if the pointer to the Port object is not nullptr
                delete settings.ports[i];  // Delete the Port object
                settings.ports[i] = nullptr;  // Set to nullptr after deletion
            }
        }
    }
}
  //from the Portenta Machine Control manual
  /*Port Details
  8 X Digital Input Channels (0-24V)
  8 X Digital Output Channels 24V power supply
  3 X Analog Input (0-10V or 4-20mA input)
  4 X Analog Output Channels (0-10V DC Max 20mA per channel)
  12 X Digital Programmable channels
  3 X Tenp Channels
  2 X Encoder Channels ABZ
  1 X RS-232/ RA-422/Rs-485 
  */
void PortManager::initializeDefaults() {

    try {
        // Initialize each Port with appropriate constructor parameters
        //port, pin, type, PIN_TYPE, CIRCUIT_TYPE, Description, Active
        settings.ports[0] = new Port(0,0, ANALOG_INPUT, ONOFF, "Pin AI0", true);
        settings.ports[1] = new Port(1,1, ANALOG_INPUT, ONOFF, "Pin AI1", true);
        settings.ports[2] = new Port(2,2, ANALOG_INPUT, ONOFF, "Pin AI1", true);
        settings.ports[3] = new Port(3,0, ANALOG_OUTPUT, ONOFF, "Pin AO0", true);
        settings.ports[4] = new Port(4,1, ANALOG_OUTPUT, ONOFF, "Pin AO1", true);
        settings.ports[5] = new Port(5,2, ANALOG_OUTPUT, ONOFF, "Pin AO2", true);
        settings.ports[6] = new Port(6,3, ANALOG_OUTPUT, ONOFF, "Pin AO3", true);


        //set default Admin items
        strncpy(settings.Admin_USERNAME, "admin", sizeof(settings.Admin_USERNAME));
        //set default password
         strncpy(settings.Admin_PASSWORD, "password", sizeof(settings.Admin_PASSWORD));

         //Default IP Address
         settings.IP_ADDRESS[0] = 192;
         settings.IP_ADDRESS[1] = 168;
         settings.IP_ADDRESS[2] = 10;
         settings.IP_ADDRESS[3] = 200;

         //default IP Address
         settings.DNS[0] = 8;
         settings.DNS[1] = 8;
         settings.DNS[2] = 8;
         settings.DNS[3] = 8;
         
         //Default Gateway
         settings.GATEWAY[0] = 192;
         settings.GATEWAY[1] = 168;
         settings.GATEWAY[2] = 10;
         settings.GATEWAY[3] = 1;

         //Default Subnet
         settings.SUBNET[0] = 255;
         settings.SUBNET[1] = 255;
         settings.SUBNET[2] = 255;
         settings.SUBNET[3] = 0;

         //byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; 
         settings.MAC[0] = 0xDE;
         settings.MAC[1] = 0xAD;
         settings.MAC[2] = 0xBE;
         settings.MAC[3] = 0xEF;
         settings.MAC[4] = 0xFE;
         settings.MAC[5] = 0xED;
         
         strncpy(settings.SERIAL_NUMBER, "002A00453130510B31353431", sizeof(settings.SERIAL_NUMBER));
         strncpy(settings.MODEL, "PORTENTA", sizeof(settings.MODEL));
         settings.REFRESH_RATE = 300000;
         strncpy(settings.SHARED_SECRET,  "12345678901234567890123456789012", sizeof(settings.SHARED_SECRET) - 1);
         settings.SHARED_SECRET[sizeof(settings.SHARED_SECRET) - 1] = '\0';
         strncpy(settings.CALL_HOME_URL,"http://192.168.10.199",sizeof(settings.CALL_HOME_URL));
         settings.REGISTRATION_STATUS = false;
         // Initialize date fields
         settings.DATE_LAST_UPDATED = time(NULL);  
         settings.DATE_LAST_REBOOT = time(NULL);

    } catch (const std::exception& e) {
        // If an exception occurs, clean up already allocated ports
        for (int j = 0; j < MAX_PORTS; ++j) {
            delete settings.ports[j];  // Free individual Port object
        }
        throw;  // Re-throw to handle the error further up the call stack
    }
}


void PortManager::writeToFlash() {
    // Serialization logic here

    if(!isInitialized){
        throw std::runtime_error("****** NOT INITIALIZED ****** UNABLE to WRITE to flash");
    }

    throw std::runtime_error("****** NOT IMPLEMETED ****** WRITE to flash");
}

bool PortManager::loadFromFlash() {
    // Simulate loading logic
    // if loading fails:
    throw std::runtime_error("****** NOT IMPLEMETED ****** READ from flash");
    return false; // Return false to simulate failed load
}

#include <sstream>
#include <iomanip> // for std::setw

String PortManager::toString() {

    if(!isInitialized){
        throw std::runtime_error("****** NOT INITIALIZED ****** UNABLE to convert settings toString()");
    }
    std::stringstream ss;
    ss << "Username: " << settings.Admin_USERNAME << "\n"
       << "Password: " << settings.Admin_PASSWORD << "\n" // Consider masking this for security
       << "IP Address: " << (int)settings.IP_ADDRESS[0] << "." << (int)settings.IP_ADDRESS[1] << "." << (int)settings.IP_ADDRESS[2] << "." << (int)settings.IP_ADDRESS[3] << "\n"
       << "Gateway: " << (int)settings.GATEWAY[0] << "." << (int)settings.GATEWAY[1] << "." << (int)settings.GATEWAY[2] << "." << (int)settings.GATEWAY[3] << "\n"
       << "DNS: " << (int)settings.DNS[0] << "." << (int)settings.DNS[1] << "." << (int)settings.DNS[2] << "." << (int)settings.DNS[3] << "\n"
       << "Subnet: " << (int)settings.SUBNET[0] << "." << (int)settings.SUBNET[1] << "." << (int)settings.SUBNET[2] << "." << (int)settings.SUBNET[3] << "\n"
       << "MAC: " << std::hex << std::setw(2) << std::setfill('0') << (int)settings.MAC[0] << ":" << (int)settings.MAC[1] << ":" << (int)settings.MAC[2] << ":" << (int)settings.MAC[3] << ":" << (int)settings.MAC[4] << ":" << (int)settings.MAC[5] << std::dec << "\n"
       << "Serial Number: " << settings.SERIAL_NUMBER << "\n"
       << "Model: " << settings.MODEL << "\n"
       << "Refresh Rate: " << settings.REFRESH_RATE << "ms\n"
       << "Shared Secret: " << settings.SHARED_SECRET << "\n" // Consider masking this for security
       << "URL: " << settings.CALL_HOME_URL << "\n"
       << "Registration Status: " << (settings.REGISTRATION_STATUS ? "Registered" : "Not Registered") << "\n"
       << "Last Updated: " << ctime(&settings.DATE_LAST_UPDATED)
       << "Last Reboot: " << ctime(&settings.DATE_LAST_REBOOT);
    return String(ss.str().c_str());
}

String PortManager::toHTML() {
    if(!isInitialized){
        throw std::runtime_error("****** NOT INITIALIZED ****** UNABLE to convert settings toHTML()");
    }
    std::stringstream ss;
    ss << "<h1>Admin Settings</h1>"
       << "<p><strong>Username:</strong> " << settings.Admin_USERNAME << "</p>"
       << "<p><strong>Password:</strong> " << "********" << "</p>" // Password masked for security
       << "<p><strong>IP Address:</strong> " << (int)settings.IP_ADDRESS[0] << "." << (int)settings.IP_ADDRESS[1] << "." << (int)settings.IP_ADDRESS[2] << "." << (int)settings.IP_ADDRESS[3] << "</p>"
       << "<p><strong>Gateway:</strong> " << (int)settings.GATEWAY[0] << "." << (int)settings.GATEWAY[1] << "." << (int)settings.GATEWAY[2] << "." << (int)settings.GATEWAY[3] << "</p>"
       << "<p><strong>DNS:</strong> " << (int)settings.DNS[0] << "." << (int)settings.DNS[1] << "." << (int)settings.DNS[2] << "." << (int)settings.DNS[3] << "</p>"
       << "<p><strong>Subnet:</strong> " << (int)settings.SUBNET[0] << "." << (int)settings.SUBNET[1] << "." << (int)settings.SUBNET[2] << "." << (int)settings.SUBNET[3] << "</p>"
       << "<p><strong>MAC:</strong> " << std::hex << std::setw(2) << std::setfill('0') << (int)settings.MAC[0] << ":" << (int)settings.MAC[1] << ":" << (int)settings.MAC[2] << ":" << (int)settings.MAC[3] << ":" << (int)settings.MAC[4] << ":" << (int)settings.MAC[5] << std::dec << "</p>"
       << "<p><strong>Serial Number:</strong> " << settings.SERIAL_NUMBER << "</p>"
       << "<p><strong>Model:</strong> " << settings.MODEL << "</p>"
       << "<p><strong>Refresh Rate:</strong> " << settings.REFRESH_RATE << "ms</p>"
       << "<p><strong>Shared Secret:</strong> " << "********" << "</p>" // Shared secret masked for security
       << "<p><strong>URL:</strong> " << settings.CALL_HOME_URL << "</p>"
       << "<p><strong>Registration Status:</strong> " << (settings.REGISTRATION_STATUS ? "Registered" : "Not Registered") << "</p>"
       << "<p><strong>Last Updated:</strong> " << ctime(&settings.DATE_LAST_UPDATED) << "</p>"
       << "<p><strong>Last Reboot:</strong> " << ctime(&settings.DATE_LAST_REBOOT) << "</p>";
    return String(ss.str().c_str());
}

