/*
File: Protonify Agent 2.0.0 Main .ino File for IDE 2.3.2
Date Created: April 29, 2024
Authors: DaPhisha
Description: Agent for reporting port status as JSON to Mongo Server
Version: 2.0.0

Road Map -
Add Cards for import and export a confifuration.

Fix Wifi

Improve UX for Ports

Stick Ports into Meters or Vessels or Processes...


*/


#include <Arduino_PortentaMachineControl.h>
#include <Arduino.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <NTPClient.h>
#include <time.h>
#include "LogManager.h"
#include "EventScheduler.h"
#include "TaskCallbacks.h"
#include "PortManager.h"
#include "NetworkManager.h"
#include "AdminServerManager.h"

#define MAX_EVENTS 10  // Pin Reference
#define MASTER_DEBUG true

const char* PROJECT_VERSION = "2.0.0"; //version

//init a event scheduler
EventScheduler scheduler(MAX_EVENTS); 

//init Network Interface (wifi) 
//init the etherner NOTE for WIFI would need different library
EthernetUDP ntpUDP;
AdminServerManager m_webServer;


//init the time client
NTPClient timeClient(ntpUDP, "pool.ntp.org");
time_t systemStartTime;//global to measure elapse refresh rates


void setup() {
    LogManager& logManager = LogManager::getInstance(); // Obtain the global LogManager instance
    logManager.init(MASTER_DEBUG);  // Initialize LogManager (e.g., setting up Serial communication)
    try{
       //Task Scheduler add tasks timeout and callback - REMOVE FOR RELEASE
      scheduler.addTask(86400000, checkMemory);//for now every 24 hours
      scheduler.addTask(3600000, checkNetwork);//every hour
      //periodically write the current settings to flash - REMOVE FOR RELEASE 
      scheduler.addTask(86400000,writeSettingsToFlash);//every 24 hrs write the settings to flash
      //print settings - REMOVE FOR RELEASE
      //scheduler.addTask(600001, printSettings);

      //port manager init
      PortManager& portManager = PortManager::getInstance(); // Automatically loads or initializes ports
      portManager.init();

      //Print out the port manager settings 
      DEBUG(portManager.toString());
    
      //Print out port settings
      //LOG(portManager.AllPortsToString()); 

      //network manager
      NetworkManager& networkManager = NetworkManager::getInstance();
      if (!networkManager.initializeNetwork()) {
        LOG("Failed to initialize network.");
      }
      //time manager
      //Start the admin Web server
      m_webServer.init();

      //Set Pin MOdes and Led Modes
      // pinMode(LED_BUILTIN, OUTPUT);
      // digitalWrite(LED_BUILTIN, LED_OFF);
      
      //FORCE BOOT OF OTHER CORE - CAUTION
      //bootM4();
      
    }catch (const std::exception& e) { // Catch exceptions derived from std::exception
    LogManager::getInstance().writeLog("*********MAJOR STARTUP FAULT************");
    LogManager::getInstance().writeLog("Caught std::exception: " + String(e.what()));
  } catch(...){
    LogManager::getInstance().writeLog("*********MAJOR STARTUP FAULT************");
    LogManager::getInstance().writeLog("*********UNKNOWN************");
  }
  logManager.writeLog("System Initialized Successfully"); 
}

void loop() {
    // Main loop operations
    /**************************/
    
  try{
      // Main Loop (M7)
      //TASK SCHEDULER
      scheduler.run(); // Run scheduled tasks
      //process any client requests.
      m_webServer.handleClient();
      //processM7Tasks();
      // Sensor Loop (M4)
      //processM4Tasks();
  
  //GLOBAL CATCH ALL
  } catch (const std::exception& e) { // Catch exceptions derived from std::exception
    LogManager::getInstance().writeLog("*********MAJOR SYSTEM FAULT************");
    LogManager::getInstance().writeLog("Caught std::exception: " + String(e.what()));
  } catch(...){
    //dataHandler.addLogEntry("MAIN LOOP CRASH ERROR -- RESETTING DEVICE");
    //dataHandler.writeTo();
    //NVIC_SystemReset();
    LogManager::getInstance().writeLog("*********MAJOR SYSTEM FAULT************");
    LogManager::getInstance().writeLog("*********UNKNOWN************");
  }
}
void shutdown() {
    // Proper cleanup if necessary before shutdown
    LogManager::getInstance().writeLog("*********SYSTEM SHUTDOWN************");
}
