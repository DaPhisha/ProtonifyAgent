#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <Ethernet.h>
#include <WiFi.h>
#include "AdminSettings.h"  // Ensure this file contains the AdminSettings struct

class NetworkManager {
private:
    EthernetClient ethClient;
    WiFiClient wifiClient;
    AdminSettings* settings;
    bool isEthernetConnected;
    bool isWiFiConnected;

public:
    // Constructor
    explicit NetworkManager(AdminSettings* config);

    // Initialize network interfaces
    bool initializeNetwork();

    // Initialize Ethernet connection
    bool initializeEthernet();

    // Initialize WiFi connection
    bool initializeWiFi();

    // Shutdown network interfaces
    void shutdownNetwork();
};

#endif // NETWORK_MANAGER_H
