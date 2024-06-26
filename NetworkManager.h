/*
File: NetworkManager.h
Date Created: May 10, 2024
Authors: DaPhisha
Description: Implementation of the NetworkManager class which handles the lifecycle and state management of network connections (wifi and ethernet in current implementation).
Version: 1.0.0
*/
#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H
#include <WiFi.h>
#include <PortentaEthernet.h>
#include "PortManager.h"

class NetworkManager {
private:
    EthernetClient ethClient;
    WiFiClient wifiClient;
    bool isEthernetConnected;
    bool isWiFiConnected;

    // Private constructor for singleton pattern
    NetworkManager();

public:
    // Deleted copy constructor and assignment operator for singleton enforcement
    NetworkManager(const NetworkManager&) = delete;
    NetworkManager& operator=(const NetworkManager&) = delete;
    static NetworkManager& getInstance();
    bool initializeNetwork();
    bool initializeEthernet();
    bool initializeWiFi();
    void shutdownNetwork();
    bool checkNetworkConnection();

};

#endif // NETWORK_MANAGER_ 


