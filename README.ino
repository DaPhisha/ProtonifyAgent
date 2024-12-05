/*
File: Protonify Agent 2.0.0 Main .ino File for IDE 2.3.2
Date Created: April 29, 2024
Authors: DaPhisha
Description: Agent for reporting port status as JSON to Mongo Server
Version: 2.0.0

Road Map -
Add Cards for import and export a configuration.
Fix Wifi
Improve UX for Ports
Stick Ports into Meters or Vessels or Processes...
Explanation and Documentation
    Initialization:
        The setup function initializes the logging, task scheduler, port manager, network manager, and web server.
        Detailed error handling is added to catch any startup faults.

    Main Loop:
        The loop function includes a try-catch block to catch any runtime exceptions.
        The scheduler.run() method runs scheduled tasks.
        The m_webServer.handleClient() method handles client requests.
        The updatePorts function checks and updates port readings based on their refresh rates.

    Updating Ports:
        The updatePorts function checks if it's time to update port readings (every second).
        It iterates over all ports and updates those that are active and need refreshing.
        The updatePortReading function updates the reading based on the circuit type.

    Simulating Port Readings:
        Each circuit type has a corresponding simulation function that generates a simulated reading.
        The simulateOnOff, simulateMA420, simulateCTEMP, simulateValve, simulateFill, and simulatePulse functions simulate readings based on the circuit type.

    Error Handling:
        Detailed error logging is included in the setup and loop functions to catch and log exceptions.
        The updatePortReading function logs an error if an unknown circuit type is encountered.

    Efficiency and Resiliency

    Efficiency:
        The updatePorts function checks the refresh rates efficiently by iterating over the ports only once per second.
        The switch-case structure in updatePortReading ensures that only the relevant simulation function is called based on the circuit type.

    Resiliency:
        The try-catch blocks in the setup and loop functions ensure that exceptions are caught and logged, preventing the system from crashing.
        The LogManager is used extensively to log detailed error messages, making it easier to diagnose and fix issues.
*/
/*
Protonify Agent 2.0.0 Documentation
Overview

The Protonify Agent is a versatile and robust system designed to monitor and manage various types of ports and their configurations. It reports port statuses as JSON to a Mongo server and provides a web interface for administration and monitoring.
System Architecture

The Protonify Agent is built with modularity in mind. The core components include:

    Main Application Loop: The central loop that manages tasks, client requests, and port updates.
    Event Scheduler: Handles periodic tasks such as memory checks and network updates.
    Port Manager: Manages port configurations and readings.
    Network Manager: Handles network initialization and communication.
    AdminServerManager: Provides a web interface for administration and monitoring.
    Logging: Captures and records events and errors for debugging and maintenance.

Module Descriptions
1. Main Application Loop

The main application loop is responsible for initializing all necessary components and continuously running scheduled tasks and handling client requests.

Key Responsibilities:

    Setup: Initializes logging, scheduler, port manager, network manager, and web server.
    Loop: Runs the scheduler, processes client requests, and updates port readings.
    Error Handling: Captures and logs any exceptions or errors that occur during execution.

2. Event Scheduler

The EventScheduler class manages tasks that need to be executed periodically. Tasks such as checking memory, network status, and writing settings to flash are scheduled and executed at specified intervals.

Key Responsibilities:

    Task Registration: Allows the registration of tasks with specific intervals.
    Task Execution: Executes tasks when their intervals are reached.
    Efficient Timing: Ensures that tasks are executed with minimal delay and overlap.

3. Port Manager

The PortManager class is responsible for managing port configurations, including initialization, updating readings, and storing settings.

Key Responsibilities:

    Initialization: Loads or initializes port settings from persistent storage.
    Updating Readings: Updates port readings based on their refresh rates.
    Settings Management: Stores and retrieves port settings, ensuring they are up-to-date and accurate.
    Simulation: Provides mechanisms to simulate port behavior for testing purposes.

4. Network Manager

The NetworkManager class handles the initialization and management of network connections, including Ethernet and potentially Wi-Fi.

Key Responsibilities:

    Initialization: Sets up network interfaces and ensures connectivity.
    Communication: Manages network communication and connectivity checks.
    Error Handling: Captures and logs any network-related errors or issues.

5. AdminServerManager

The AdminServerManager class provides the web interface for administration and monitoring of the Protonify Agent. It serves HTML pages, handles form submissions, and returns JSON responses for client-side processing.

Key Responsibilities:

    Initialization: Sets up the web server and defines routes for handling various HTTP requests.
    Request Handling: Processes incoming HTTP requests and provides appropriate responses.
    Port Management: Handles requests for port configurations and updates.
    Security: Ensures secure access to the administration interface using authentication tokens.

Detailed Breakdown:
Initialization

The init method initializes the web server, setting up routes for handling various HTTP requests. It ensures that all necessary routes are defined and that the server is ready to handle incoming requests.
Request Handling

The handleClient method processes incoming client requests, delegating to appropriate handlers based on the request URL. Each handler is responsible for processing specific types of requests, such as fetching port configurations or updating port settings.
Port Management

The AdminServerManager includes methods for managing ports, such as handling requests to fetch analog input ports, digital input ports, and other types of ports. These methods retrieve the necessary port data, format it as JSON, and send it to the client.
6. Logging

The Logging module captures and records events and errors for debugging and maintenance. It provides a centralized mechanism for logging messages, ensuring that all significant events are recorded and can be reviewed later.

Key Responsibilities:

    Initialization: Sets up the logging mechanism, including configuring output destinations (e.g., serial, file).
    Logging Events: Captures and records events, including errors and significant state changes.
    Error Handling: Logs any exceptions or errors that occur, providing detailed information for debugging.

Usage
Initialization

    LogManager: Initializes the logging mechanism.
    EventScheduler: Registers and schedules periodic tasks.
    PortManager: Loads or initializes port configurations.
    AdminServerManager: Initializes the web server and defines routes.

Main Loop

The main loop continuously runs scheduled tasks, handles client requests, and updates port readings. It captures any exceptions or errors, logs them, and ensures the system continues to run smoothly.
Port Simulation and Reading

The PortManager periodically updates port readings based on their configured refresh rates. It supports simulating port behavior for testing purposes, allowing for robust testing and debugging.


Webserver

This guide explains the steps to add a new route, create a new page, and incorporate new CSS and JavaScript into the Protonify Agent web system. This involves updates to the AdminServerManager class, the ResourceHandler class, and appropriate JavaScript files.
Steps to Add a New Route, Page, CSS, and JavaScript
1. Adding a New Route
a. Define the Route

AdminServerManager.h

    Function Declaration:
        Function: handleNewPage
        Description: Declares the handler function for the new route.
        Parameters:
            EthernetClient& client: The client making the request.
            const String& request: The HTTP request.
            int contentLength: The length of the content.
            const String& authToken: The authentication token.
        Returns: void

AdminServerManager.cpp

    Route Definition:
        Definition: Adds the new route to the route table.
        Route: {"GET", "/newpage", handleNewPage}

b. Implement the Route Handler

AdminServerManager.cpp

    Function Implementation:
        Function: handleNewPage
        Description: Generates the HTML content for the new page.
        Parameters:
            EthernetClient& client: The client making the request.
            const String& request: The HTTP request.
            int contentLength: The length of the content.
            const String& authToken: The authentication token.
        Returns: void

2. Creating a New HTML Page

ResourceHandler.h / ResourceHandler.cpp

    Function Declaration:
        Function: getNewPageContent
        Description: Returns the HTML content for the new page.
        Parameters: None
        Returns: const char* - The HTML content as a C-string.

3. Adding New CSS

ResourceHandler.h / ResourceHandler.cpp

    Function Declaration:
        Function: getCSS
        Description: Returns the CSS rules, including those for the new page.
        Parameters: None
        Returns: const char* - The CSS content as a C-string.

4. Adding New JavaScript

ResourceHandler.h / ResourceHandler.cpp

    Function Declaration:
        Function: getJavascript
        Description: Returns the JavaScript code, including new functionalities for the new page.
        Parameters: None
        Returns: const char* - The JavaScript content as a C-string.

Detailed Function Descriptions
handleNewPage

Function: handleNewPage

Description: Handles the HTTP GET request for the new page, generates and sends the HTML content to the client.

Parameters:

    EthernetClient& client: The client making the request.
        Type: EthernetClient&
        Description: The reference to the Ethernet client.
    const String& request: The HTTP request.
        Type: const String&
        Description: The string containing the HTTP request data.
    int contentLength: The length of the content.
        Type: int
        Description: The integer specifying the length of the content.
    const String& authToken: The authentication token.
        Type: const String&
        Description: The string containing the authentication token.

Returns: void
getNewPageContent

Function: getNewPageContent

Description: Provides the HTML content for the new page.

Parameters: None

Returns:

    const char*
        Type: const char*
        Description: The HTML content as a C-string.

getCSS

Function: getCSS

Description: Returns the CSS rules for the entire system, including the new page.

Parameters: None

Returns:

    const char*
        Type: const char*
        Description: The CSS content as a C-string.

getJavascript

Function: getJavascript

Description: Returns the JavaScript code for the entire system, including the functionalities for the new page.

Parameters: None

Returns:

    const char*
        Type: const char*
        Description: The JavaScript content as a C-string.


Help Page for Protonify Agent Web System
Overview

The Protonify Agent Web System is designed to manage and monitor various ports on a device. This guide will walk you through the different functionalities available in the system, including viewing stats, updating settings, managing ports, and using the console.
Stats Page

The Stats Page provides an overview of the system's current status, including vital statistics such as uptime, memory usage, and network status. Here are the key elements you will find on the Stats Page:

    System Uptime: Displays how long the system has been running since the last reboot.
    Memory Usage: Shows the current memory usage of the system.
    Network Status: Provides information about the current network connection, including IP address, gateway, and DNS.

Settings Page

The Settings Page allows you to configure various system parameters. Below are the settings available, along with their default values and descriptions:

    Admin Settings
        Username:
            Default: admin
            Description: The username for logging into the admin interface.
        Password:
            Default: password
            Description: The password for logging into the admin interface.

    WiFi Settings
        SSID:
            Default: HAL2000
            Description: The SSID of the WiFi network to connect to.
        Password:
            Default: password
            Description: The password for the WiFi network.

    Network Settings
        IP Address:
            Default: 192.168.10.200
            Description: The static IP address assigned to the device.
        DNS Server:
            Default: 8.8.8.8
            Description: The DNS server used by the device.
        Gateway:
            Default: 192.168.10.1
            Description: The gateway IP address for the network.
        Subnet Mask:
            Default: 255.255.255.0
            Description: The subnet mask for the network.
        MAC Address:
            Default: DE:AD:BE:EF:FE
            Description: The MAC address of the device.

    Device Information
        Serial Number:
            Default: 002A00453130510B31353431
            Description: The unique serial number of the device.
        Model:
            Default: PORTENTA
            Description: The model name of the device.
        Refresh Rate:
            Default: 300000 (milliseconds)
            Description: The refresh rate for updating port data.
        Shared Secret:
            Default: 12345678901234567890123456789012
            Description: A shared secret key for secure communication.
        Call Home URL:
            Default: http://192.168.10.199
            Description: The URL the device calls for updates and registration.
        Registration Status:
            Default: false
            Description: Indicates if the device is registered.

Ports Page

The Ports Page allows you to manage and monitor different ports on the device. Ports can be configured as active or inactive, and each port has specific settings based on its type.
Adding/Removing Ports from Active

    To Add a Port:
        Navigate to the Ports Page.
        Select the port you want to activate.
        Update its settings and mark it as active.

    To Remove a Port:
        Navigate to the Ports Page.
        Select the port you want to deactivate.
        Update its settings and mark it as inactive.

Configuring Ports for the Simulator

To configure a port to use the simulator, follow these steps:

    Navigate to the Ports Page.
    Select the port you want to configure.
    In the port settings, enable the simulation option.
    Configure the simulation parameters as needed.

Console Page

The Console Page provides advanced tools for managing the device, including resetting the system and viewing logs.
Using the Console

    Resetting the System:
        Navigate to the Console Page.
        Select the type of reset you want to perform (soft or factory reset).
        Confirm the reset action.

    Viewing Logs:
        Navigate to the Console Page.
        View the system logs to troubleshoot issues or monitor activity.

Default Settings for Reset

Performing a factory reset will restore the following default settings:

    Admin Username: admin
    Admin Password: password
    WiFi SSID: HAL2000
    WiFi Password: password
    IP Address: 192.168.10.200
    DNS Server: 8.8.8.8
    Gateway: 192.168.10.1
    Subnet Mask: 255.255.255.0
    MAC Address: DE:AD:BE:EF:FE
    Serial Number: 002A00453130510B31353431
    Model: PORTENTA
    Refresh Rate: 300000 milliseconds
    Shared Secret: 12345678901234567890123456789012
    Call Home URL: http://192.168.10.199
    Registration Status: false

*/