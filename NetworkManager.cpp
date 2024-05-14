/*
File: NetworkManager.cpp
Date Created: May 10, 2024
Authors: DaPhisha
Description: Implementation of the NetworkManager class which handles the lifecycle and state management of network connections (wifi and ethernet in current implementation).
Version: 1.0.0
*/
#include "NetworkManager.h"
#include "LogManager.h"
#include <stdexcept>

NetworkManager::NetworkManager() {
    isEthernetConnected = false;
    isWiFiConnected = false;
}

NetworkManager& NetworkManager::getInstance() {
    static NetworkManager instance;
    return instance;
}

bool NetworkManager::initializeNetwork() {
    try {
        if (initializeEthernet()) {
            LOG("Connected to ethernet.  Pull the cable to switch to WIFI.");
            return true;
        } else {
            LOG("Failed to initialize Ethernet switching to WIFI connection");
            if (initializeWiFi()) {
                return true;
            }
        }
    } catch (const std::runtime_error& e) {
        throw;  // Re-throw the same exception
    }
    throw std::runtime_error("Failed to initialize both Ethernet and WiFi networks.");
}

bool NetworkManager::initializeEthernet() {
    if (!Ethernet.begin(
      PortManager::getInstance().settings.MAC, PortManager::getInstance().settings.IP_ADDRESS, 
      PortManager::getInstance().settings.DNS, PortManager::getInstance().settings.GATEWAY,
      PortManager::getInstance().settings.SUBNET)) {
      std::string err = "Ethernet initialization failed with static IP.";
        LOG(err.c_str());
        throw std::runtime_error(err);
    }
    isEthernetConnected = true;
    LOG("Connected via Ethernet, IP: " + Ethernet.localIP().toString());
    //LOG();
  
    return true;
}

bool NetworkManager::initializeWiFi() {
    WiFi.begin(PortManager::getInstance().settings.WIFI_SSID, PortManager::getInstance().settings.WIFI_PASSWORD);
    for (int i = 0; i < 10; ++i) {
        if (WiFi.status() == WL_CONNECTED) {
            isWiFiConnected = true;
            LOG("Connected via WiFi, IP: " + WiFi.localIP().toString());
            //String ipAddressWifi = WiFi.localIP().toString();
            //LOG();
            return true;
        }
        delay(1000);
    }
    std::string err = "WiFi connection failed after multiple attempts.";
    LOG(err.c_str());
    throw std::runtime_error(err);
}

void NetworkManager::shutdownNetwork() {
    if (isEthernetConnected) {
        Ethernet.end();
        isEthernetConnected = false;
    }
    if (isWiFiConnected) {
        //WiFi.
        WiFi.disconnect();
        isWiFiConnected = false;
    }
    LOG("Network disconnected cleanly.");
}

bool NetworkManager::checkNetworkConnection() {
    if (isEthernetConnected && Ethernet.linkStatus() == LinkON) {
        LOG("Ethernet Connected.");
        return true;
    }
    if (isWiFiConnected && WiFi.status() == WL_CONNECTED) {
        LOG("WIFI Connected.");
        return true;
    }
    LOG("Network is disconnected, attempting reconnection.");
    return initializeNetwork();
}


