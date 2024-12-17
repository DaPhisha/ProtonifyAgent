#include "pins_arduino.h"
/*
File: PortManager.cpp
Date Created: April 29, 2024
Authors: DaPhisha
Description: Implementation of the PortManager class which handles the lifecycle and state management of Port objects, including interactions with flash memory for persistence.
Version: 1.0.0
https://docs.arduino.cc/tutorials/portenta-machine-control/user-manual/
*/

#include "PortManager.h"
#include "LogManager.h"

#include <sstream>
#include <iomanip> // for std::setw
#include <cstring>

PortManager::PortManager(){
  isInitialized = false;
  m_loopCounter = 0;
}
void PortManager::init() {
    if (!loadFromFlash()) {
        LOG("FAILED Reading settings from flash.  Initiating Defaults.");
        initializeDefaults();
    }else{
      isInitialized = true;
    }
}
PortManager& PortManager::getInstance() {
    static PortManager instance; // Guaranteed to be destroyed and instantiated on first use
    return instance;
}

PortManager::~PortManager() {
    //assum we should write the current DB to flash but maybe not 
    //writeToFlash();
    //MachineControl_AnalogOut.
}

void PortManager::initializeDefaults() {
    try {

        //set default Admin items
        strncpy(settings.Admin_USERNAME, "admin", sizeof(settings.Admin_USERNAME));
        //set default password
        strncpy(settings.Admin_PASSWORD, "password", sizeof(settings.Admin_PASSWORD));
        //strncpy(settings.WIFI_SSID, "HAL2000", sizeof(settings.WIFI_SSID));
        //strncpy(settings.WIFI_PASSWORD, "quiettree081", sizeof(settings.WIFI_PASSWORD));
        strncpy(settings.WIFI_SSID, "ArduinoNetwork", sizeof(settings.WIFI_SSID));
        strncpy(settings.WIFI_PASSWORD, "hotdogtimemachine!69", sizeof(settings.WIFI_PASSWORD));

         //Default IP Address
         settings.IP_ADDRESS[0] = 192;
         settings.IP_ADDRESS[1] = 168;
         settings.IP_ADDRESS[2] = 10;
         settings.IP_ADDRESS[3] = 99;
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
         
         strncpy(settings.DESCRIPTION, "Portenta H7 DEV",sizeof(settings.DESCRIPTION));
         strncpy(settings.SERIAL_NUMBER, "004400373132511539313731", sizeof(settings.SERIAL_NUMBER));
         strncpy(settings.MODEL, "PORTENTA", sizeof(settings.MODEL));
         settings.REFRESH_RATE = 300000;//every 5 mins
         strncpy(settings.SHARED_SECRET,  "12345678901234567890123456789012", sizeof(settings.SHARED_SECRET) - 1);
         settings.SHARED_SECRET[sizeof(settings.SHARED_SECRET) - 1] = '\0';
         strncpy(settings.CALL_HOME_HOST,"192.168.10.100",sizeof(settings.CALL_HOME_HOST));
         settings.REGISTRATION_STATUS = false;
         // Initialize date fields
         settings.DATE_LAST_UPDATED = time(NULL);  
         settings.DATE_LAST_REBOOT = time(NULL);

         //disable serial set to false
         settings.DISABLESERIAL = false;
         settings.DISABLEWIFI = false;

         settings.DISPLAY_STATUS = false;
         //SET DEFAULT DISPLAY
         strncpy(settings.DISPLAY_TEXT,  "Loading Defaults :)", sizeof(settings.DISPLAY_TEXT) - 1);
         
         //reset the log counters and log array
         settings.currentLogIndex = 0;
         for(int i = 0; i < MAX_LOG_ENTRIES; i++){
              settings.logEntries[i][0]= '\0';
         }
         isInitialized = true;
         loadPortDefaults();
         writeToFlash();
    } catch (const std::exception& e) {
        
        throw;  
    }
}
void PortManager::clearLog() {
    memset(settings.logEntries, 0, sizeof(settings.logEntries));
    settings.currentLogIndex = 0;
    writeToFlash();
}


void PortManager::loadPortDefaults(){

        time_t currentTime = time(nullptr);

        //Array value is the port number
        //start of analog pins
        setPortValues(0,false,false,0,-1,ANALOG_INPUT,ONOFF,"PIN AI_00", 0.00,0.00, currentTime,"INIT DEFAULT");
        setPortValues(1,false,false,1,-1,ANALOG_INPUT,MA420,"PIN AI_01", 0.00,0.00, currentTime,"INIT DEFAULT");
        setPortValues(2,false,false,2,-1,ANALOG_INPUT,CTEMP,"PIN AI_02", 0.00,0.00, currentTime,"INIT DEFAULT");
        setPortValues(3,true,false,-1,0,ANALOG_OUTPUT,OFF10V,"PIN AO_00",0.00,0.00, currentTime,"OFF");
        setPortValues(4,false,false,-1,1,ANALOG_OUTPUT,OFF10V,"PIN AO_01", 0.00,0.00, currentTime,"INIT DEFAULT");
        setPortValues(5,false,false,-1,2,ANALOG_OUTPUT,OFF10V,"PIN AO_02", 0.00,0.00,currentTime,"INIT DEFAULT");
        setPortValues(6,false,false,-1,3,ANALOG_OUTPUT,OFF10V,"PIN AO_03", 0.00,0.00, currentTime,"INIT DEFAULT");

        //start of digital input pins 0-7
        setPortValues(7,false,false,DIN_READ_CH_PIN_00,-1,DIGITAL_INPUT,FLOW,"DI_00", 0.00,0.00, currentTime,"INIT DEFAULT");
        setPortValues(8,false,false,DIN_READ_CH_PIN_01,-1,DIGITAL_INPUT,MA420,"DI_01", 0.00,0.00, currentTime,"INIT DEFAULT");
        setPortValues(9,false,false,DIN_READ_CH_PIN_02,-1,DIGITAL_INPUT,NOT_ASSIGNED,"DI_02", 0.00,0.00, currentTime,"INIT DEFAULT");
        setPortValues(10,false,false,DIN_READ_CH_PIN_03,-1,DIGITAL_INPUT,CTEMP,"DI_03", 0.00,0.00, currentTime,"INIT DEFAULT");
        setPortValues(11,false,false,DIN_READ_CH_PIN_04,-1,DIGITAL_INPUT,VALVE,"DI_04", 0.00,0.00, currentTime,"INIT DEFAULT");
        setPortValues(12,false,false,DIN_READ_CH_PIN_05,-1,DIGITAL_INPUT,FILL,"DI_05", 0.00,0.00, currentTime,"INIT DEFAULT");
        setPortValues(13,false,false,DIN_READ_CH_PIN_06,-1,DIGITAL_INPUT,NOT_ASSIGNED,"DI_06", 0.00,0.00, currentTime,"INIT DEFAULT");
        setPortValues(14,false,false,DIN_READ_CH_PIN_07,-1,DIGITAL_INPUT,PULSE,"DI_07", 0.00,0.00, currentTime,"INIT DEFAULT");
        //end of digital input pins
         //start of digital output pins 15-22
         setPortValues(15,true,false,-1,0,DIGITAL_OUTPUT,OFF24V,"DO_00", 0.00,0.00, currentTime,"OFF");
         setPortValues(16,false,false,-1,1,DIGITAL_OUTPUT,OFF24V,"DO_01", 0.00,0.00, currentTime,"INIT DEFAULT");
         setPortValues(17,false,false,-1,2,DIGITAL_OUTPUT,OFF24V,"DO_02", 0.00,0.00, currentTime,"INIT DEFAULT");
         setPortValues(18,false,false,-1,3,DIGITAL_OUTPUT,OFF24V,"DO_03", 0.00,0.00, currentTime,"INIT DEFAULT");
         setPortValues(19,false,false,-1,4,DIGITAL_OUTPUT,OFF24V,"DO_04", 0.00,0.00, currentTime,"INIT DEFAULT");
         setPortValues(20,false,false,-1,5,DIGITAL_OUTPUT,OFF24V,"DO_05", 0.00,0.00, currentTime,"INIT DEFAULT");
         setPortValues(21,false,false,-1,6,DIGITAL_OUTPUT,OFF24V,"DO_06", 0.00,0.00, currentTime,"INIT DEFAULT");
         setPortValues(22,false,false,-1,7,DIGITAL_OUTPUT,OFF24V,"DO_07", 0.00,0.00, currentTime,"INIT DEFAULT");
         //end of the digital output pins

         //start of 12 digital PROGRAMMABLE_IO pins
         setPortValues(23,false,false,IO_WRITE_CH_PIN_00,IO_READ_CH_PIN_00,PROGRAMMABLE_IO,ONOFF,"PIO_00", 0.00,0.00, currentTime,"INIT DEFAULT");
         setPortValues(24,false,false,IO_WRITE_CH_PIN_01,IO_READ_CH_PIN_01,PROGRAMMABLE_IO,ONOFF,"PIO_01", 0.00,0.00, currentTime,"INIT DEFAULT");
         setPortValues(25,false,false,IO_WRITE_CH_PIN_02,IO_READ_CH_PIN_02,PROGRAMMABLE_IO,ONOFF,"PIO_02", 0.00,0.00, currentTime,"INIT DEFAULT");
         setPortValues(26,false,false,IO_WRITE_CH_PIN_03,IO_READ_CH_PIN_03,PROGRAMMABLE_IO,ONOFF,"PIO_03", 0.00,0.00, currentTime,"INIT DEFAULT");
         setPortValues(27,false,false,IO_WRITE_CH_PIN_04,IO_READ_CH_PIN_04,PROGRAMMABLE_IO,ONOFF,"PIO_04", 0.00,0.00, currentTime,"INIT DEFAULT");
         setPortValues(28,false,false,IO_WRITE_CH_PIN_05,IO_READ_CH_PIN_05,PROGRAMMABLE_IO,ONOFF,"PIO_05", 0.00,0.00, currentTime,"INIT DEFAULT");
         setPortValues(29,false,false,IO_WRITE_CH_PIN_06,IO_READ_CH_PIN_06,PROGRAMMABLE_IO,ONOFF,"PIO_06", 0.00,0.00, currentTime,"INIT DEFAULT");
         setPortValues(30,false,false,IO_WRITE_CH_PIN_07,IO_READ_CH_PIN_07,PROGRAMMABLE_IO,ONOFF,"PIO_07", 0.00,0.00, currentTime,"INIT DEFAULT");
         setPortValues(31,false,false,IO_WRITE_CH_PIN_08,IO_READ_CH_PIN_08,PROGRAMMABLE_IO,ONOFF,"PIO_08", 0.00,0.00, currentTime,"INIT DEFAULT");
         setPortValues(32,false,false,IO_WRITE_CH_PIN_09,IO_READ_CH_PIN_09,PROGRAMMABLE_IO,ONOFF,"PIO_09", 0.00,0.00, currentTime,"INIT DEFAULT");
         setPortValues(33,false,false,IO_WRITE_CH_PIN_10,IO_READ_CH_PIN_10,PROGRAMMABLE_IO,ONOFF,"PIO_10", 0.00,0.00, currentTime,"INIT DEFAULT");
         setPortValues(34,false,false,IO_WRITE_CH_PIN_11,IO_READ_CH_PIN_11,PROGRAMMABLE_IO,ONOFF,"PIO_11", 0.00,0.00, currentTime,"INIT DEFAULT");
         
         setPortValues(35,false,false,1,1,HMI,PULSE,"HMI TXP 485", 0.00,0.00, currentTime,"INIT DEFAULT");
         setPortValues(36,false,false,-1,-1,HMI,PULSE,"HMI TXN 485", 0.00,0.00, currentTime,"INIT DEFAULT");
        
        setPortValues(37,false,false,1,1,HMI,PULSE,"HMI RXP 485", 0.00,0.00, currentTime,"INIT DEFAULT");
         setPortValues(38,false,false,-1,-1,HMI,PULSE,"HMI RXN 485", 0.00,0.00, currentTime,"INIT DEFAULT");

        setPortValues(39,false,false,1,1,HMI,PULSE,"Bus CANH", 0.00,0.00, currentTime,"INIT DEFAULT");
        setPortValues(40,false,false,-1,-1,HMI,PULSE,"Bus CANL", 0.00,0.00, currentTime,"INIT DEFAULT");

        //start of temp probe pins
        setPortValues(41,false,false,0,0,PTEMP,CTEMP,"TMP TP-00", 0.00,0.00,currentTime,"INIT DEFAULT");

        
        setPortValues(42,false,false,1,1,PTEMP,CTEMP,"TMP TN-00", 0.00,0.00, currentTime,"INIT DEFAULT");
        setPortValues(43,false,false,2,2,PTEMP,CTEMP,"TMP RTD-00", 0.00,0.00, currentTime,"INIT DEFAULT");

        setPortValues(44,false,false,3,3,PTEMP,CTEMP,"TMP TP-01", 0.00,0.00, currentTime,"INIT DEFAULT");
        setPortValues(45,false,false,4,4,PTEMP,CTEMP,"TMP TN-01", 0.00,0.00, currentTime,"INIT DEFAULT");
        setPortValues(46,false,false,5,5,PTEMP,CTEMP,"TMP RTD-01", 0.00,0.00, currentTime,"INIT DEFAULT");
        
        setPortValues(47,false,false,6,6,PTEMP,CTEMP,"TMP TP-02", 0.00,0.00, currentTime,"INIT DEFAULT");
        setPortValues(48,false,false,7,7,PTEMP,CTEMP,"TMP TN-02", 0.00,0.00, currentTime,"INIT DEFAULT");
        setPortValues(49,false,false,8,8,PTEMP,CTEMP,"TMP RTD-02", 0.00,0.00, currentTime,"INIT DEFAULT");
        //end temp probesIO_READ_CH_PIN_00
        
        //start of encoders pins
        setPortValues(50,false,false,0,0,ENCODER,PULSE,"ENCODER A0", 0.00,0.00, currentTime,"INIT DEFAULT");
        setPortValues(51,false,false,1,1,ENCODER,PULSE,"ENCODER B0", 0.00,0.00, currentTime,"INIT DEFAULT");
        setPortValues(52,false,false,2,2,ENCODER,PULSE,"ENCODER Z0", 0.00,0.00, currentTime,"INIT DEFAULT");
        setPortValues(53,false,false,3,3,ENCODER,PULSE,"ENCODER A1", 0.00,0.00, currentTime,"INIT DEFAULT");
        setPortValues(54,false,false,4,4,ENCODER,PULSE,"ENCODER B1", 0.00,0.00, currentTime,"INIT DEFAULT");
        setPortValues(55,false,false,5,5,ENCODER,PULSE,"ENCODER Z1", 0.00,0.00, currentTime,"INIT DEFAULT");
     //end of encoders pins
}
void PortManager::setPortValues(int indexPort, bool isActive, bool isSimulated, int readPinNum,int writePinNum, PIN_TYPE pinType, CIRCUIT_TYPE circuitType, String pinDescription, float curReading, float lastReading, time_t currentTime, String stateStr){

        if(indexPort < 0 || indexPort > MAX_PORTS){
            String errMsg = "****** ARRAY VIOLATION ****** array index invalid " + String(indexPort);
            throw std::runtime_error(errMsg.c_str());
        }
        settings.ports[indexPort].isActive = isActive;
        settings.ports[indexPort].isSimulated = isSimulated;
        settings.ports[indexPort].readPinNumber = readPinNum;
        settings.ports[indexPort].writePinNumber = writePinNum;
        settings.ports[indexPort].pinType = pinType;
        settings.ports[indexPort].circuitType = circuitType;
        strcpy(settings.ports[indexPort].pinDescription, pinDescription.c_str());
        settings.ports[indexPort].currentReading = curReading;
        settings.ports[indexPort].lastReading = lastReading;
        settings.ports[indexPort].lastUpdated = currentTime;
        strcpy(settings.ports[indexPort].state,stateStr.c_str());
}

void PortManager::writeToFlash() {
   

    if(!isInitialized){
        throw std::runtime_error("****** NOT INITIALIZED ****** UNABLE to WRITE to flash");
    }

    //create a buffer the size of the AdminSettings + DATA_SIGNATURE
    size_t m_MemoryRequired = getDataSizeWithSignature();
    char buffer[m_MemoryRequired] {};

    //copy in the DATA_SIGNATURE
    memcpy(buffer, &DATA_SIGNATURE, sizeof(DATA_SIGNATURE));
    //copy in the main admin settings payload
    memcpy(buffer + sizeof(DATA_SIGNATURE), &settings, sizeof(AdminSettings));

    auto [flashSize, startAddress, iapSize] = getFlashIAPLimits();
    FlashIAPBlockDevice blockDevice(startAddress, iapSize);

    int result = blockDevice.init();
    if (result != 0) {
      throw std::runtime_error("****** BLOCK DEVICE ****** UNABLE to INIT to device");
    }

    const size_t eraseSize = blockDevice.get_erase_size();
    blockDevice.erase(0, eraseSize);

    result = blockDevice.program(buffer, 0, m_MemoryRequired);
    if (result != 0) {
      float dataSizeInMB = getDataSizeWithSignature() / (float)(1024 * 1024);
      String errMsg = "***** FLASH BLOCK DEVICE ***** Error writing to blockdevice: " + String(dataSizeInMB);
      blockDevice.deinit();
      throw std::runtime_error(errMsg.c_str());
    } else {
      blockDevice.deinit();
    }
}

size_t PortManager::getDataSizeWithSignature() const {
    size_t totalDataSize = sizeof(DATA_SIGNATURE) + sizeof(AdminSettings);
    size_t programSize = 32;  // as indicated by your testCases
    if (totalDataSize % programSize != 0) {
        totalDataSize += programSize - (totalDataSize % programSize);
    }
    return totalDataSize;
}


bool PortManager::loadFromFlash() {
    
    //FlashIAPLimits limits = getFlashIAPLimits();  // Get the current flash memory limits
    auto [flashSize, startAddress, iapSize] = getFlashIAPLimits();

    String flashStringData = "Flash Size: " + String(flashSize) + "\nFlash Available: " + String(iapSize) + "\nStart Address: " + String(startAddress);

    if (iapSize == 0) {
        throw std::runtime_error("****** FLASH ERROR ****** No memory available."); // If no available memory, 
        //throw load error indicating load failure of flash at boot
    }
    FlashIAPBlockDevice blockDevice(startAddress, iapSize);
    int result = blockDevice.init();
    if (result != 0) {
        throw std::runtime_error("***** FLASH ERROR ****** unable to read from block device"); // If no available memory, 
    }
    size_t m_MemSizeRequired = getDataSizeWithSignature();
    char buffer[m_MemSizeRequired] = {0};
    
    result = blockDevice.read(buffer, 0,m_MemSizeRequired);
    
    if (result != 0) {
        blockDevice.deinit();
        throw std::runtime_error("***** FLASH ERROR ****** unable to read from block device"); 
    }

    if(memcmp(buffer, DATA_SIGNATURE, sizeof(DATA_SIGNATURE)) != 0){
      //DID NOT FIND DATA_SIGNATURE
      LOG("DID NOT FIND DATA_SIGNATURE");
      return false;
    }
    memcpy(&settings, buffer + sizeof(DATA_SIGNATURE), sizeof(AdminSettings));
    return true;  // Signature does not match or data corrupted
}

size_t PortManager::getDataSizeWithSignature() {
    size_t totalDataSize = sizeof(DATA_SIGNATURE) + sizeof(AdminSettings);
    size_t programSize = 32;  // as indicated by testCases
    if (totalDataSize % programSize != 0) {
        totalDataSize += programSize - (totalDataSize % programSize);
    }
    return totalDataSize;
}

String PortManager::AllPortsToString() {
    String result;
    result.reserve(1024); // Reserve enough space to avoid frequent reallocations

    result += "PortManager Port Summary:\n";
    for (int i = 0; i < MAX_PORTS; i++) {    
      result += "Port " + String(i) + ": ";
      result += OnePortToString(i) + "\n"; // Utilize the Port's toString method
    }
    return result;
}

int PortManager::getActivePortCount(){
    int counter = 0;
    for(int i = 0; i < MAX_PORTS; i++){
      if(settings.ports[i].isActive == true){
        counter ++;
      }
    }
    return counter;
}

String PortManager::OnePortToString(int i) {
  char buffer[256]; // Allocate a buffer for the string
  snprintf(buffer, sizeof(buffer), 
    "Port Number: %d, Read Pin: %d, Write Pin: %d, Pin Type: %s, Circuit Type: %s, Description: %s, Current Reading: %.2f, Last Reading: %.2f, State: %s, Active: %s, Simulated: %s, Last Updated: %s",
             i, 
             settings.ports[i].readPinNumber, 
             settings.ports[i].writePinNumber, 
             pinTypeToString(settings.ports[i].pinType).c_str(), 
             circuitTypeToString(settings.ports[i].circuitType).c_str(), 
             settings.ports[i].pinDescription, 
             settings.ports[i].currentReading, 
             settings.ports[i].lastReading, 
             settings.ports[i].state, 
             settings.ports[i].isActive ? "Yes" : "No", 
             settings.ports[i].isSimulated ? "Yes" : "No", 
             LogManager::getInstance().timeToString(settings.ports[i].lastUpdated).c_str());
    return String(buffer);
    
}




String PortManager::pinTypeToString(PIN_TYPE type){
  switch (type) {
        case ANALOG_INPUT: return "ANALOG INPUT";
        case ANALOG_OUTPUT: return "ANALOG_OUTPUT";
        case DIGITAL_INPUT: return "DIGITAL_INPUT";
        case DIGITAL_OUTPUT: return "DIGITAL_OUTPUT";
        case PROGRAMMABLE_IO: return "PROGRAMMABLE_IO";
        case PTEMP: return "PTEMP";
        case HMI: return "HMI";
        case ENCODER: return "ENCODER";
        default: return "UNKNOWN";
    }
}

String PortManager::circuitTypeToString(CIRCUIT_TYPE type){
  switch (type) {
        case NOT_ASSIGNED: return "ON OFF 0-24V";
        case ONOFF: return "ON OFF 0-24V";
        case ON24V: return "ON 24V";
        case OFF24V: return "OFF 24V";
        case ON10V: return "ON 10V";
        case OFF10V: return "OFF 10V";
        case MA420: return "4-20 mA";
        case CTEMP: return "Temperature";
        case VALVE: return "Valve Reading";
        case FILL: return "Tank Fill Level";
        case PULSE: return "Pulse";
        case FLOW: return "Flow";
        default: return "UNKNOWN";
    }

}
String PortManager::circuitTypeToCode(CIRCUIT_TYPE type){
  switch (type) {
        case NOT_ASSIGNED: return "not_assigned";
        case ONOFF: return "ONOFF";
        case ON24V: return "ON24V";
        case OFF24V: return "OFF24V";
        case ON10V: return "ON10V";
        case OFF10V: return "OFF10V";
        case MA420: return "MA420";
        case CTEMP: return "CTEMP";
        case VALVE: return "VALVE";
        case FILL: return "FILL";
        case PULSE: return "PULSE";
        case FLOW: return "FLOW";
        default: return "UNKNOWN";
    }

}

String PortManager::ipToString(const uint8_t ip[4]) {
    return String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
}

bool PortManager::stringToIP(const String &ipString, uint8_t ip[4]) {
    int parts[4];
    if (sscanf(ipString.c_str(), "%d.%d.%d.%d", &parts[0], &parts[1], &parts[2], &parts[3]) == 4) {
        for (int i = 0; i < 4; i++) {
            if (parts[i] < 0 || parts[i] > 255) {
                return false; // Invalid IP part
            }
            ip[i] = static_cast<uint8_t>(parts[i]);
        }
        return true;
    }
    return false; // Invalid IP format
}

String PortManager::macToString(const byte mac[6]) {
    char macStr[18]; // 6 * 2 (hex) + 5 (colons) + 1 (null terminator) = 18
    sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return String(macStr);
}

bool PortManager::stringToMAC(const String &macString, byte mac[6]) {
    int parts[6];
    if (sscanf(macString.c_str(), "%02x:%02x:%02x:%02x:%02x:%02x", &parts[0], &parts[1], &parts[2], &parts[3], &parts[4], &parts[5]) == 6) {
        for (int i = 0; i < 6; i++) {
            if (parts[i] < 0 || parts[i] > 255) {
                return false; // Invalid MAC part
            }
            mac[i] = static_cast<byte>(parts[i]);
        }
        return true;
    }
    return false; // Invalid MAC format
}

String PortManager::timeToString(time_t t) {
    char buffer[20];
    struct tm* tm_info = localtime(&t);
    strftime(buffer, 20, "%Y-%m-%d %H:%M:%S", tm_info);
    return String(buffer); // Wrapping the result in double quotes
}

String PortManager::inputPinNumberToString(int pinNumber){
  switch (pinNumber) {
        case DIN_READ_CH_PIN_00: return "DIN_READ_CH_PIN_00";
        case DIN_READ_CH_PIN_01: return "DIN_READ_CH_PIN_01";
        case DIN_READ_CH_PIN_02: return "DIN_READ_CH_PIN_02";
        case DIN_READ_CH_PIN_03: return "DIN_READ_CH_PIN_03";
        case DIN_READ_CH_PIN_04: return "DIN_READ_CH_PIN_04";
        case DIN_READ_CH_PIN_05: return "DIN_READ_CH_PIN_05";
        case DIN_READ_CH_PIN_06: return "DIN_READ_CH_PIN_06";
        case DIN_READ_CH_PIN_07: return "DIN_READ_CH_PIN_07";
        /*case PIN_A0: return "PIN_A0";
        case PIN_A1: return "PIN_A1";
        case PIN_A2: return "PIN_A2";
        case PIN_A3: return "PIN_A3";
        */
        case IO_READ_CH_PIN_00: return "IO_READ_CH_PIN_00";
        case IO_READ_CH_PIN_01: return "IO_READ_CH_PIN_01";
        case IO_READ_CH_PIN_02: return "IO_READ_CH_PIN_02";
        case IO_READ_CH_PIN_03: return "IO_READ_CH_PIN_03";
        case IO_READ_CH_PIN_04: return "IO_READ_CH_PIN_04";
        case IO_READ_CH_PIN_05: return "IO_READ_CH_PIN_05";
        case IO_READ_CH_PIN_06: return "IO_READ_CH_PIN_06";
        case IO_READ_CH_PIN_07: return "IO_READ_CH_PIN_07";
        case IO_READ_CH_PIN_08: return "IO_READ_CH_PIN_08";
        case IO_READ_CH_PIN_09: return "IO_READ_CH_PIN_09";
        case IO_READ_CH_PIN_10: return "IO_READ_CH_PIN_10";
        case IO_READ_CH_PIN_11: return "IO_READ_CH_PIN_11";
        /*
        case IO_WRITE_CH_PIN_00: return "IO_WRITE_CH_PIN_00";
        case IO_WRITE_CH_PIN_01: return "IO_WRITE_CH_PIN_01";
        case IO_WRITE_CH_PIN_02: return "IO_WRITE_CH_PIN_02";
        case IO_WRITE_CH_PIN_03: return "IO_WRITE_CH_PIN_03";
        case IO_WRITE_CH_PIN_04: return "IO_WRITE_CH_PIN_04";
        case IO_WRITE_CH_PIN_05: return "IO_WRITE_CH_PIN_05";
        case IO_WRITE_CH_PIN_06: return "IO_WRITE_CH_PIN_06";
        case IO_WRITE_CH_PIN_07: return "IO_WRITE_CH_PIN_07";
        case IO_WRITE_CH_PIN_08: return "IO_WRITE_CH_PIN_08";
        case IO_WRITE_CH_PIN_09: return "IO_WRITE_CH_PIN_09";
        case IO_WRITE_CH_PIN_10: return "IO_WRITE_CH_PIN_10";
        case IO_WRITE_CH_PIN_11: return "IO_WRITE_CH_PIN_11";
        */
        default: return "UNKNOWN";
    }
}
