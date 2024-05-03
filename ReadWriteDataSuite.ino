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

#define MAX_EVENTS 10  // Pin Reference

const char* PROJECT_VERSION = "2.0.0"; //version

//init a event scheduler
EventScheduler scheduler(MAX_EVENTS); 

//init Network Interface (wifi) 
//init the etherner NOTE for WIFI would need different library
EthernetUDP ntpUDP;

//init the time client
NTPClient timeClient(ntpUDP, "pool.ntp.org");
time_t systemStartTime;//global to measure elapse refresh rates

//ConfigurationManager configManager;

void setup() {
    LogManager& logManager = LogManager::getInstance(); // Obtain the global LogManager instance
    logManager.init();  // Initialize LogManager (e.g., setting up Serial communication)
    try{
       //Task Scheduler add tasks timeout and callback
      scheduler.addTask(50000, checkMemory);

      //port manager init
      PortManager& portManager = PortManager::getInstance(); // Automatically loads or initializes ports
      portManager.init();

      //Print out the port manager settings REMOVE FROM RELEASE
      LogManager::getInstance().writeLog(portManager.toString());

      //configManager

      //network manager

      //time manager

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
