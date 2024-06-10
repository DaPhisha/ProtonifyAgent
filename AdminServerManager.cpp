

/*
  File: AdminServerManager.cpp
  Date Created: May 11, 2024
  Authors: DaPhisha, ChatGPT, and Assistant
  Description: Code file for the administrative web server.
  Last Modified: May 11, 2024
  Version 2.0.0
*/
#include "AdminServerManager.h"
#include "ResourceHandler.h"
#include <cstddef>
#include <regex>
EthernetServer server(HTTP_SERVER_PORT);
bool m_serverStatusConnected = false;
static String m_activeToken = "";
static unsigned long m_tokenGenerationTime = 0;


AdminServerManager::AdminServerManager() {
  static RouteHandler localHandlers[] = {
        {"GET", "/", handleRoot},
        {"GET", "/login", handleRoot},

        {"GET", "/logout", handleLogOut},
        {"POST", "/login", handleLogin},
        
        {"GET", "/home", handleHome},
        {"POST", "/home", handleHome},
        
        {"GET", "/console", handleConsole},
        {"POST", "/console/reset", handleReset},
        
        {"GET", "/admin", handleAdmin},
        {"POST", "/admin", processAdmin},

        {"GET", "/admin/test", handleTestConnection},
        {"POST", "/admin/register", handleRegister},
        {"POST", "/admin/unregister", handleUnregister},


        {"GET", "/ports/active", handleAllActivePorts},
        {"GET", "/ports/active/list", handleGetAllActivePorts},
        {"GET", "/ports/analogin", handleAnalogINPorts}, 
        {"GET", "/ports/analogin/list", handleGetAnalogINPorts},
        {"GET", "/ports/analogout", handleAnalogOUTPorts}, 
        {"GET", "/ports/analogout/list", handleGetAnalogOUTPorts},
        {"GET", "/ports/digitalin", handleDigitalINPorts}, 
        {"GET", "/ports/digitalin/list", handleGetDigitalINPorts},
        {"GET", "/ports/digitalout", handleDigitalOUTPorts}, 
        {"GET", "/ports/digitalout/list", handleGetDigitalOUTPorts},
        {"GET", "/ports/programableio", handleProgrammableIOPorts}, 
        {"GET", "/ports/programableio/list", handleGetProgrammableIOPorts},
        {"GET", "/ports/hmi", handleHMIPorts}, 
        {"GET", "/ports/hmi/list", handleGetHMIPorts},
        {"GET", "/ports/ptemp", handlePTEMPPorts}, 
        {"GET", "/ports/ptemp/list", handleGetPTEMPPorts},
        {"GET", "/ports/encoders", handleENCODERPorts}, 
        {"GET", "/ports/encoders/list", handleGetENCODERPorts},
        {"POST", "/ports/update", handlePortUpdate},   
        {"GET", "/js/script.js", handleJS},
        {"GET", "/css/style.css", handleCSS},
        {"GET", "/help", handleHelp}
    };
    handlers = localHandlers;
    m_handlersCount = sizeof(localHandlers) / sizeof(localHandlers[0]);
}
String AdminServerManager::generateRandomToken() {
    String token = "";
    for (int i = 0; i < 10; i++) { 
        char c = (char) random(33, 126); // Generate a random character from '!'(33) to '~'(126)
        token += c;
    }
    token += String(millis());
    return token;
}
void AdminServerManager::init() {
  m_activeToken = generateRandomToken();
  IPAddress m_IPAddress(PortManager::getInstance().settings.IP_ADDRESS[0],
                        PortManager::getInstance().settings.IP_ADDRESS[1],
                        PortManager::getInstance().settings.IP_ADDRESS[2],
                        PortManager::getInstance().settings.IP_ADDRESS[3]);
  
  IPAddress m_DNS(PortManager::getInstance().settings.DNS[0],
                  PortManager::getInstance().settings.DNS[1],
                  PortManager::getInstance().settings.DNS[2],
                  PortManager::getInstance().settings.DNS[3]);
  
  IPAddress m_Gateway(PortManager::getInstance().settings.GATEWAY[0],
                  PortManager::getInstance().settings.GATEWAY[1],
                  PortManager::getInstance().settings.GATEWAY[2],
                  PortManager::getInstance().settings.GATEWAY[3]);

  IPAddress m_Subnet(PortManager::getInstance().settings.SUBNET[0],
                  PortManager::getInstance().settings.SUBNET[1],
                  PortManager::getInstance().settings.SUBNET[2],
                  PortManager::getInstance().settings.SUBNET[3]);

  
  int retValue = Ethernet.begin(
  PortManager::getInstance().settings.MAC,
  m_IPAddress,
  m_DNS,
  m_Gateway,
  m_Subnet);
  server.begin();
}
void AdminServerManager::handleClient() {
    EthernetClient client = server.available();
    if (client) {

        String currentLine = ""; // Stores the current line of the request
        String method = "";      // Stores the HTTP method (GET, POST, etc.)
        String url = "";         // Stores the URL requested
        bool isHeaderComplete = false; // Flag to check if the HTTP header has been fully received
        String requestBody = ""; // Stores the body of a POST request
        String authToken = "";   // Stores the value of the authToken cookie
        int contentLength = 0;   // Stores the length of the POST request body

        while (client.connected()) {
            if (client.available()) {
                char c = client.read(); 
                if (c == '\n') { 
                    if (currentLine.length() == 0) {
                        isHeaderComplete = true; 
                        if (method == "POST" && contentLength > 0) {
                            while (requestBody.length() < contentLength) {
                                if (client.available()) {
                                    char c = client.read();
                                    requestBody += c;
                                }
                            }
                        }

                        for (int i = 0; i < m_handlersCount; i++) {
                            if (method == handlers[i].method && url == handlers[i].path) {
                                
                                handlers[i].handler(client, requestBody, contentLength, authToken);
                                break;
                            }
                        }
                        break;
                    } else {
                        
                        if (method == "" && url == "") {
                            int firstSpace = currentLine.indexOf(' ');
                            int secondSpace = currentLine.lastIndexOf(' ');
                            if (firstSpace != -1 && secondSpace != -1 && firstSpace != secondSpace) {
                                method = currentLine.substring(0, firstSpace);
                                url = currentLine.substring(firstSpace + 1, secondSpace);
                            }
                        }

                        if (currentLine.startsWith("Content-Length: ")) {
                            contentLength = currentLine.substring(16).toInt();
                        }

                        if (currentLine.startsWith("Cookie: ")) {
                            int authTokenIndex = currentLine.indexOf("authToken=");
                            if (authTokenIndex != -1) {
                                int authTokenEnd = currentLine.indexOf(';', authTokenIndex);
                                if (authTokenEnd == -1) {
                                    authTokenEnd = currentLine.length();
                                }
                                authToken = currentLine.substring(authTokenIndex + 10, authTokenEnd);
                            }
                        }

                        currentLine = ""; // Reset currentLine for the next line
                    }
                } else if (c != '\r') {
                    currentLine += c; // Add character to currentLine if it's not a carriage return
                }
            }
        }
        client.stop(); // Close the client connection
    }
}

String AdminServerManager::getUptimeString() {
  unsigned long totalSeconds = millis() / 1000;
  unsigned long days = totalSeconds / 86400;
  totalSeconds %= 86400;
  unsigned long hours = totalSeconds / 3600;
  totalSeconds %= 3600;
  unsigned long minutes = totalSeconds / 60;
  unsigned long seconds = totalSeconds % 60;

  String uptime = "Uptime: " + String(days) + "d " + String(hours) + "h " + String(minutes) + "m " + String(seconds) + "s";
  return uptime;
}

void AdminServerManager::handleHome(EthernetClient& client, const String& request,int contentLength, const String &authToken) {

  if(authToken == m_activeToken){
    handleHomeLandingPage(client, request,"Authentication Token found.");
  }else{
    handleError(client, 401, "User not authorized! Please re-login.");
  }
}

void AdminServerManager::handleRoot(EthernetClient& client, const String& request,int contentLength, const String &authToken) {
    String message = "Welcome to the Protonify Agent Management System!" + AdminServerManager::getUptimeString();
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head>");
    client.println("<link rel='stylesheet' href='/css/style.css'>");
    client.println("<style>");
    client.println("");
    client.println("</style>");
    client.println("</head>");
    client.println("<body>");
    client.println("<br><div class='center' style='color: blue;'>" + message + "</div><br>");
    client.println("<div class='container center' style='background-color: gray;'>");
    client.print(ResourceHandler::getLoginCard());
    client.println("</div>");
    client.print(ResourceHandler::getFooter());
    client.println("</body>");
    client.println("</html>");
}



void AdminServerManager::handleLogOut(EthernetClient& client, const String& request,int contentLength, const String &authToken) {
      m_activeToken = generateRandomToken();
      handleRoot(client, request, 0,"");
}
bool AdminServerManager::isValidMAC(const String& mac) {
    String decodedMac = urlDecode(mac);
    int parts[6];
    char colon;
    if (sscanf(decodedMac.c_str(), "%x%c%x%c%x%c%x%c%x%c%x", &parts[0], &colon, &parts[1], &colon, &parts[2], &colon, &parts[3], &colon, &parts[4], &colon, &parts[5]) != 11) {
        return false;
    }
    for (int i = 0; i < 6; ++i) {
        if (parts[i] < 0 || parts[i] > 255) {
            return false;
        }
    }
    return true;
}
bool AdminServerManager::isValidIP(const String& ip) {
    const std::regex ipRegex(
        R"(^(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})$)"
    );

    std::smatch match;
    std::string ipStr = ip.c_str(); // Convert String to std::string for regex processing

    if (std::regex_match(ipStr, match, ipRegex)) {
        // Check if each part of the IP address is within the range [0, 255]
        for (int i = 1; i <= 4; ++i) {
            int part = std::stoi(match[i].str());
            if (part < 0 || part > 255) {
                return false;
            }
        }
        return true;
    }

    return false;
}

bool AdminServerManager::isValidURL(const String& url) {
    String decodedUrl = urlDecode(url);
    const std::regex urlPattern(R"((http|https)://(([a-zA-Z0-9-]+\.)+[a-zA-Z]{2,}|(\d{1,3}\.){3}\d{1,3})(:\d{1,5})?(/.*)?)");
    if (std::regex_match(decodedUrl.c_str(), urlPattern)) {
        return true;
    }
    return false;
}

void AdminServerManager::processAdmin(EthernetClient& client, const String& request, int contentLength, const String &authToken) {

    if(authToken != m_activeToken){
         handleError(client, 404, "Un-Authorized Settings Change Request to /admin");
         return;
    }
    String m_userMessage = "";
    bool isValid = true;
    String errorMessage = "";
    String adminUsername = extractValueFromPostData(request, "Admin_USERNAME");
    String adminPassword = extractValueFromPostData(request, "Admin_PASSWORD");    
    if (adminUsername.length() > MAX_USERNAME || adminPassword.length() > MAX_PASSWORD) {
        isValid = false;
        errorMessage += "Admin credentials invalid. ";
    }
    String wifiSSID = extractValueFromPostData(request, "WIFI_SSID");
    String wifiPassword = extractValueFromPostData(request, "WIFI_PASSWORD");

    if (wifiSSID.length() > MAX_SSID || wifiPassword.length() > MAX_PASSWORD) {
        isValid = false;
        errorMessage += "WiFi settings invalid. ";
    }

    String ipAddress = extractValueFromPostData(request, "IP_ADDRESS");
    String gateway = extractValueFromPostData(request, "GATEWAY");
    String dns = extractValueFromPostData(request, "DNS");
    String subnet = extractValueFromPostData(request, "SUBNET");
    String macAddress = extractValueFromPostData(request, "MAC");
    if (!isValidIP(ipAddress) || !isValidIP(gateway) || !isValidIP(dns) || !isValidIP(subnet)) {
        isValid = false;
        errorMessage += "Network settings invalid. ";
    }

    if (!isValidMAC(macAddress)) {
        isValid = false;
        errorMessage += "MAC address invalid.";
    }

    // Extract and validate Device information
    String serialNumber = extractValueFromPostData(request, "SERIAL_NUMBER");
    String model = extractValueFromPostData(request, "MODEL");
    String refreshRateStr = extractValueFromPostData(request, "REFRESH_RATE");
    String sharedSecret = extractValueFromPostData(request, "SHARED_SECRET");
    String callHomeHost = extractValueFromPostData(request, "CALL_HOME_HOST");

    if (serialNumber.length() > MAX_SERIAL || model.length() > MAX_MODEL || sharedSecret.length() > MAX_SHARED_SECRET || !isValidIP(callHomeHost)) {
        isValid = false;
        errorMessage += "Call home Host IP invalid. ";
    }
    uint32_t refreshRate = refreshRateStr.toInt();
    if (refreshRate == 0) {
        isValid = false;
        errorMessage += "Refresh rate invalid. ";
    }

    // If all data is valid, update settings
    if (isValid) {
        strncpy(PortManager::getInstance().settings.Admin_USERNAME, adminUsername.c_str(), MAX_USERNAME);
        strncpy(PortManager::getInstance().settings.Admin_PASSWORD, adminPassword.c_str(), MAX_PASSWORD);
        strncpy(PortManager::getInstance().settings.WIFI_SSID, wifiSSID.c_str(), MAX_SSID);
        strncpy(PortManager::getInstance().settings.WIFI_PASSWORD, wifiPassword.c_str(), MAX_PASSWORD);

        sscanf(ipAddress.c_str(), "%hhu.%hhu.%hhu.%hhu", &PortManager::getInstance().settings.IP_ADDRESS[0], &PortManager::getInstance().settings.IP_ADDRESS[1], &PortManager::getInstance().settings.IP_ADDRESS[2], &PortManager::getInstance().settings.IP_ADDRESS[3]);
        sscanf(gateway.c_str(), "%hhu.%hhu.%hhu.%hhu", &PortManager::getInstance().settings.GATEWAY[0], &PortManager::getInstance().settings.GATEWAY[1], &PortManager::getInstance().settings.GATEWAY[2], &PortManager::getInstance().settings.GATEWAY[3]);
        sscanf(dns.c_str(), "%hhu.%hhu.%hhu.%hhu", &PortManager::getInstance().settings.DNS[0], &PortManager::getInstance().settings.DNS[1], &PortManager::getInstance().settings.DNS[2], &PortManager::getInstance().settings.DNS[3]);
        sscanf(subnet.c_str(), "%hhu.%hhu.%hhu.%hhu", &PortManager::getInstance().settings.SUBNET[0], &PortManager::getInstance().settings.SUBNET[1], &PortManager::getInstance().settings.SUBNET[2], &PortManager::getInstance().settings.SUBNET[3]);
        sscanf(macAddress.c_str(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &PortManager::getInstance().settings.MAC[0], &PortManager::getInstance().settings.MAC[1], &PortManager::getInstance().settings.MAC[2], &PortManager::getInstance().settings.MAC[3], &PortManager::getInstance().settings.MAC[4], &PortManager::getInstance().settings.MAC[5]);

        strncpy(PortManager::getInstance().settings.SERIAL_NUMBER, serialNumber.c_str(), MAX_SERIAL);
        strncpy(PortManager::getInstance().settings.MODEL, model.c_str(), MAX_MODEL);
        PortManager::getInstance().settings.REFRESH_RATE = refreshRate;
        strncpy(PortManager::getInstance().settings.SHARED_SECRET, sharedSecret.c_str(), MAX_SHARED_SECRET);
        strncpy(PortManager::getInstance().settings.CALL_HOME_HOST, urlDecode(callHomeHost).c_str(), MAX_CALL_HOME_HOST);
        
        PortManager::getInstance().writeToFlash();
        sendSuccessResponse(client,"SUCCESS: Admin settings written to flash.");

    } else {
        sendErrorResponse(client, errorMessage);
    }
}


void AdminServerManager::handleLogin(EthernetClient& client, const String& request,int contentLength, const String &authToken) {
      if(authToken == m_activeToken){
          handleHomeLandingPage(client,request, "Resuming session with found Token.");
          return;
      }

      DEBUG("REQUEST: " + request);
      String m_userMessage = "";
      String m_userName = extractValueFromPostData(request,"username");
      String m_password = extractValueFromPostData(request,"password");

      if (m_userName == PortManager::getInstance().settings.Admin_USERNAME && m_password == PortManager::getInstance().settings.Admin_PASSWORD) {
        m_activeToken = generateRandomToken();
        m_tokenGenerationTime = millis();
        m_userMessage = "PASSED CREDENTIAL CHECK.";
        handleHomeLandingPage(client,request, "Welcome back " + m_userName);
    } else {
        m_userMessage = "LOGIN FAILED";
        handleError(client, 404, m_userMessage);
    }
}

int AdminServerManager::getContentLength(String requestString){
    int contentLengthIndex = requestString.indexOf("Content-Length: ");
    if (contentLengthIndex != -1) {
        int contentLengthStart = contentLengthIndex + 15;
        int contentLengthEnd = requestString.indexOf("\r\n", contentLengthStart);
        String contentLengthStr = requestString.substring(contentLengthStart, contentLengthEnd);
        return contentLengthStr.toInt();
    }
    return -1;
}

String AdminServerManager::urlDecode(const String &str) {
    String decoded = "";
    char charBuffer[3];
    charBuffer[2] = '\0';
    for (unsigned int i = 0; i < str.length(); i++) {
        char c = str.charAt(i);
        if (c == '+') { 
            decoded += ' ';
        } else if (c == '%' && (i + 2 < str.length())) {
            charBuffer[0] = str.charAt(++i);
            charBuffer[1] = str.charAt(++i);
            decoded += (char) strtol(charBuffer, NULL, 16);
        } else {
            decoded += c;
        }
    }
    return decoded;
}

String AdminServerManager::getPostData(int contentLength, const String &request) {
    if (contentLength <= 0) {
        return String();  // return empty string if content length is invalid
    }
    int headerEnd = request.indexOf("\r\n\r\n");
    return request.substring(headerEnd + 4, headerEnd + 4 + contentLength);
}

String AdminServerManager::extractValueFromPostData(String postData, String variableName) {
    int variableIndex = postData.indexOf(variableName + "=");
    if (variableIndex != -1) {
        int valueIndex = variableIndex + variableName.length() + 1; // Skip '='
        int endIndex = postData.indexOf("&", valueIndex);
        if (endIndex == -1) {
            endIndex = postData.length(); // Reached end of string
        }
        return postData.substring(valueIndex, endIndex);
    }
    return ""; 
}

void AdminServerManager::handleJS(EthernetClient& client, const String& request,int contentLength, const String &authToken) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/javascript");
    client.println("Connection: close");
    client.print(ResourceHandler::getJavascript());
    client.stop();
}

void AdminServerManager::handleCSS(EthernetClient& client, const String& request,int contentLength, const String &authToken) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/css");
    client.println("Connection: close");
    client.print(ResourceHandler::getCSS());
    client.stop();
}

void AdminServerManager::handleError(EthernetClient& client, int errorCode, String errorMessage) {
  client.println("HTTP/1.1 " + String(errorCode) + " " + errorMessage);
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.println("<html lang='en'>");
  client.println("<head>");
  client.println("<meta charset='UTF-8'>");
  client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
  client.println("<title>Protonify Agent Handler</title>");
  client.println("<link rel='stylesheet' href='/css/style.css'>");
  client.println("</head>");
  client.println("<body>");
  client.println("<div class='error-container'>");
  client.println("<div class='card large'>");
  client.println("<div class='card-header'>URL no longer valid.</div>");
  client.println("<div class='card-body center'>");
  client.println("<div class='error-title'>" + String(errorCode) + "</div>");
  client.println("<div class='error-message'>" + errorMessage + "</div>");
  client.println("<a href='/home' class='error-button'>Home</a>");
  client.println("<a href='/' class='error-button'>Re-Login</a>");
  client.println(" </div></div></div>");
  client.print(ResourceHandler::getFooter());
  client.println("</body>");
  client.println("</html>");
  client.stop();
}

void AdminServerManager::handleReset(EthernetClient& client, const String& request,int contentLength, const String &authToken) {
  
  if(authToken != m_activeToken){
         LOG("UN-AUTHORIZED ACCESS /console/reset");
         handleError(client, 404, "Un-Authorized Access Request to /console/reset");
         return;
  }
   String m_resetType = extractValueFromPostData(request,"resetType");
    if (m_resetType == "soft") {
        PortManager::getInstance().writeToFlash();

    } else if (m_resetType == "factory") {
        PortManager::getInstance().initializeDefaults();
    }
    handleError(client, 200, "Resetting to " + m_resetType);
    client.stop();
    delay(3000); 
    NVIC_SystemReset();

}

void AdminServerManager::handleConsole(EthernetClient& client, const String& request,int contentLength, const String &authToken) {
    if(authToken != m_activeToken){
         LOG("UN-AUTHORIZED ACCESS /console");
         handleError(client, 404, "Un-Authorized Access Request to /console");
         return;
    }
  
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println("Set-Cookie: authToken=" + m_activeToken + "; Path=/; Max-Age=86400; SameSite=None;"); 
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html lang='en'>");
    client.print(ResourceHandler::getHeaderRefresh("Logger Console"));
    client.println("<body>");
    client.print(ResourceHandler::getHeaderMenu());
    client.println("<main class='main-content'>");
  
    String user_message = "Welcome to the console logger.";
    client.println("<br><p class='centered-message'>" + user_message + "</p><br>");

    client.println("<div style='height: 800px; overflow-y: scroll; border: 1px solid #ccc; padding: 5px;'>");
    for (int i = PortManager::getInstance().settings.currentLogIndex > 0 ? (PortManager::getInstance().settings.currentLogIndex - 1) : 0; i >= 0; i--) {
      client.print(PortManager::getInstance().settings.logEntries[i]);
      client.println("<br>");
    }
    client.println("</div>");
    client.println("</main>");
    client.print(ResourceHandler::getFooter());
    client.println("</body>");
    client.println("</html>");
     client.stop();
}

void AdminServerManager::handleAdmin(EthernetClient& client, const String& request, int contentLength, const String& authToken) {
    if (authToken != m_activeToken) {
        handleError(client, 404, "Un-Authorized Access Request to /admin");
        return;
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println("Set-Cookie: authToken=" + m_activeToken + "; Path=/; Max-Age=86400; SameSite=None;");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html lang='en'>");
    client.print(ResourceHandler::getHeaderRefresh("Admin Settings"));
    client.println("<body>");
    client.print(ResourceHandler::getHeaderMenu());
    client.println("<main class='main-content'>");
    client.println("<br><p class='centered-message' id='error-message'>Welcome please make sure to save your changes.</p><br>");
    String page = R"(
        <p class='centered-message'>Admin Settings</p>
        <div class='admin-container'>
            <form action='/admin' method='POST' class='admin-form' id='admin-form'>
                <!-- Form fields for settings -->
                <!-- Admin Credentials -->
                <div class='admin-card'>
                    <div class='admin-card-header'>Admin Credentials</div>
                    <div class='admin-card-content'>
                        <label for='Admin_USERNAME'>Username</label>
                        <input type='text' id='Admin_USERNAME' name='Admin_USERNAME' value='{{Admin_USERNAME}}' required>
                        <label for='Admin_PASSWORD'>Password</label>
                        <input type='password' id='Admin_PASSWORD' name='Admin_PASSWORD' value='{{Admin_PASSWORD}}' required>
                        <input type="checkbox" id="togglePassword"> <p class='centered-message'>Show Password</p>
                    </div>
                </div>
                <!-- WiFi Settings -->
                <div class='admin-card'>
                    <div class='admin-card-header'>WiFi Settings</div>
                    <div class='admin-card-content'>
                        <label for='WIFI_SSID'>SSID</label>
                        <input type='text' id='WIFI_SSID' name='WIFI_SSID' value='{{WIFI_SSID}}' required>
                        <label for='WIFI_PASSWORD'>Password</label>
                        <input type='password' id='WIFI_PASSWORD' name='WIFI_PASSWORD' value='{{WIFI_PASSWORD}}' required>
                        <input type="checkbox" id="toggleWifiPassword"> <p class='centered-message'>Show Password<p>
                    </div>
                </div>
                <!-- Network Settings -->
                <div class='admin-card'>
                    <div class='admin-card-header'>Network Settings</div>
                    <div class='admin-card-content'>
                        <label for='IP_ADDRESS'>IP Address</label>
                        <input type='text' id='IP_ADDRESS' name='IP_ADDRESS' value='{{IP_ADDRESS}}' required>
                        <label for='GATEWAY'>Gateway</label>
                        <input type='text' id='GATEWAY' name='GATEWAY' value='{{GATEWAY}}' required>
                        <label for='DNS'>DNS</label>
                        <input type='text' id='DNS' name='DNS' value='{{DNS}}' required>
                        <label for='SUBNET'>Subnet</label>
                        <input type='text' id='SUBNET' name='SUBNET' value='{{SUBNET}}' required>
                        <label for='MAC'>MAC Address</label>
                        <input type='text' id='MAC' name='MAC' value='{{MAC}}' required>
                    </div>
                </div>
                <!-- Device Information -->
                <div class='admin-card'>
                    <div class='admin-card-header'>Device Information</div>
                    <div class='admin-card-content'>
                        <label for='SERIAL_NUMBER'>Serial Number</label>
                        <input type='text' id='SERIAL_NUMBER' name='SERIAL_NUMBER' value='{{SERIAL_NUMBER}}' required>
                        <label for='MODEL'>Model</label>
                        <input type='text' id='MODEL' name='MODEL' value='{{MODEL}}' required>
                        <label for='REFRESH_RATE'>Refresh Rate</label>
                        <input type='number' id='REFRESH_RATE' name='REFRESH_RATE' value='{{REFRESH_RATE}}' required>
                        <label for='SHARED_SECRET'>Shared Secret</label>
                        <input type='text' id='SHARED_SECRET' name='SHARED_SECRET' value='{{SHARED_SECRET}}' required>
                        <label for='CALL_HOME_HOST'>Call Home Host IP</label>
                        <input type='text' id='CALL_HOME_HOST' name='CALL_HOME_HOST' value='{{CALL_HOME_HOST}}' required>
                        <label for='REGISTRATION_STATUS'>Registration Status</label>
                        <input type='checkbox' id='REGISTRATION_STATUS' name='REGISTRATION_STATUS' {{REGISTRATION_STATUS}} disabled>
                    </div>
                </div>
                <div class='center'>
                    <button type='submit' class='admin-submit-button'>Update Settings</button>
                </div>
            </form>
            <div class="cards-wrapper">
                <div class='admin-card'>
                    <div class='admin-card-header'>Test Server Connection</div>
                    <div class='admin-card-content'>
                        <button id='test-ssl-button' class='admin-submit-button'>Test Connection</button>
                        <p id='ssl-status-message' class='centered-message'></p>
                    </div>
                </div>
                
                <!-- Reset Card -->
                <form action='/console/reset' method='POST' class='reset-form'>
                    <div class='admin-card'>
                        <div class='admin-card-header'>Reset Options</div>
                        <div class='admin-card-content'>
                            <label for='resetType'>Select Reset Option:</label>
                            <select id='resetType' name='resetType' required>
                                <option value='soft'>Soft Reset</option>
                                <option value='factory'>Factory Reset</option>
                            </select>
                        </div>
                        <button type='submit' class='admin-submit-button'>Reset</button>
                    </div>
                </form>
                
                <!-- Register Card -->
                {{REGISTER_CARD}}
            </div>
            <br><br>
        </div>
    )";

    // Replace placeholders with actual values
    page.replace("{{Admin_USERNAME}}", String(PortManager::getInstance().settings.Admin_USERNAME));
    page.replace("{{Admin_PASSWORD}}", String(PortManager::getInstance().settings.Admin_PASSWORD));
    page.replace("{{WIFI_SSID}}", String(PortManager::getInstance().settings.WIFI_SSID));
    page.replace("{{WIFI_PASSWORD}}", String(PortManager::getInstance().settings.WIFI_PASSWORD));
    page.replace("{{IP_ADDRESS}}", PortManager::getInstance().ipToString(PortManager::getInstance().settings.IP_ADDRESS));
    page.replace("{{GATEWAY}}", PortManager::getInstance().ipToString(PortManager::getInstance().settings.GATEWAY));
    page.replace("{{DNS}}", PortManager::getInstance().ipToString(PortManager::getInstance().settings.DNS));
    page.replace("{{SUBNET}}", PortManager::getInstance().ipToString(PortManager::getInstance().settings.SUBNET));
    page.replace("{{MAC}}", PortManager::getInstance().macToString(PortManager::getInstance().settings.MAC));
    page.replace("{{SERIAL_NUMBER}}", String(PortManager::getInstance().settings.SERIAL_NUMBER));
    page.replace("{{MODEL}}", String(PortManager::getInstance().settings.MODEL));
    page.replace("{{REFRESH_RATE}}", String(PortManager::getInstance().settings.REFRESH_RATE));
    page.replace("{{SHARED_SECRET}}", String(PortManager::getInstance().settings.SHARED_SECRET));
    page.replace("{{CALL_HOME_HOST}}", String(PortManager::getInstance().settings.CALL_HOME_HOST));
    page.replace("{{REGISTRATION_STATUS}}", PortManager::getInstance().settings.REGISTRATION_STATUS ? "checked" : "");

    String registerCard = ResourceHandler::getRegisterCard(PortManager::getInstance().settings.REGISTRATION_STATUS);
    page.replace("{{REGISTER_CARD}}", registerCard);

    client.println(page);

    client.println("<br><br></main>");
    client.print(ResourceHandler::getFooter());
    client.println("</body>");
    client.println("</html>");
    client.stop();
}


void AdminServerManager::handleHomeLandingPage(EthernetClient& client, const String& request, String user_message){
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println("Set-Cookie: authToken=" + m_activeToken + "; Path=/; Max-Age=86400; SameSite=None; "); 
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html lang='en'>");

    client.print(ResourceHandler::getHeader("Agent Dashboard"));
    client.println("<body>");
    client.print(ResourceHandler::getHeaderMenu());
    
    client.println("<main class='main-content'>");
    client.println("<br><p class='centered-message'>" + user_message + "</p><br>");
    client.println("<div class='stats-container'>");
    client.print(ResourceHandler::getStatCard("Uptime", getUptimeString()));
    FlashIAPLimits limits = getFlashIAPLimits();
    client.print(ResourceHandler::getStatCard("Flash Size", String(limits.flash_size)));
    client.print(ResourceHandler::getStatCard("Flash Available", String(limits.available_size)));
  
    float flashPercentage = ((float)limits.available_size / (float)limits.flash_size) * 100;

    client.print(ResourceHandler::getStatCard("Flash %", String(flashPercentage)));
    
    mbed_stats_heap_t heap_stats;
    mbed_stats_heap_get(&heap_stats);

    float usagePercentage = ((float)heap_stats.current_size / (float)heap_stats.reserved_size) * 100.0;
    client.print(ResourceHandler::getStatCard("Heap Used", String(heap_stats.current_size)));
    client.print(ResourceHandler::getStatCard("Heap %", String(usagePercentage)));

    //ports
    client.print(ResourceHandler::getStatCard("Ports Enabled", String(PortManager::getInstance().getActivePortCount())));
    // Close stats container
    client.println("</div>");

    client.println("</main>");//close off main content
    //close page with footer
    client.print(ResourceHandler::getFooter());
    //close out body and page
    client.println("</body>");
    client.println("</html>");
    client.stop();
}

//handles a port update POST processing
void AdminServerManager::handlePortUpdate(EthernetClient& client, const String& request, int contentLength, const String &authToken) {
    LOG("PORT UPDATE REQUEST: " + request);
    // Check for authentication
    if(authToken != m_activeToken){
        LOG("UN-AUTHORIZED ACCESS /ports/update POST");
        sendErrorResponse(client, "Un-Authorized Access Request to /ports/update");
        return;
    }

    // Extract and validate the port index
    String portIndexStr = extractValueFromPostData(request, "portIndex");
    int portIndex = portIndexStr.toInt();
    if (portIndex < 0 || portIndex >= MAX_PORTS) {
        sendErrorResponse(client, "Invalid port index");
        return;
    }

    Ports& port = PortManager::getInstance().settings.ports[portIndex];
    String circuitTypeStr = extractValueFromPostData(request, "circuitType");
    if (circuitTypeStr == "ONOFF") port.circuitType = ONOFF;
    else if (circuitTypeStr == "MA420") port.circuitType = MA420;
    else if (circuitTypeStr == "CTEMP") port.circuitType = CTEMP;
    else if (circuitTypeStr == "VALVE") port.circuitType = VALVE;
    else if (circuitTypeStr == "FILL") port.circuitType = FILL;
    else if (circuitTypeStr == "PULSE") port.circuitType = PULSE;
    else {
        sendErrorResponse(client, "Invalid circuit type");
        return;
    }
    String URLpinDescription = extractValueFromPostData(request, "pinDescription");
    String pinDescription = urlDecode(URLpinDescription);
    if (pinDescription.length() > MAX_DESCRIPTION) {
        sendErrorResponse(client, "Pin description too long");
        return;
    }
    strncpy(port.pinDescription, pinDescription.c_str(), MAX_DESCRIPTION);
    String isActiveStr = extractValueFromPostData(request, "isActive");
    port.isActive = (isActiveStr == "on");
    String isSimulatedStr = extractValueFromPostData(request, "isSimulated");
    port.isSimulated = (isSimulatedStr == "on");

    if (port.isActive) {
        if (port.pinType == DIGITAL_INPUT || port.pinType == DIGITAL_OUTPUT) {
            // Specific validation for digital ports

            //NEED TO DEFINE RULES HERE


        } else if (port.pinType == ANALOG_INPUT || port.pinType == ANALOG_OUTPUT) {
            // Specific validation for analog ports

            //NEED TO DEFINE RULES HERE

        } else if (port.pinType == PROGRAMMABLE_IO) {
            // Specific validation for programmable IO ports

          //NEED TO DEFINE RULES HERE

        } else if (port.pinType == PTEMP) {
            // Specific validation for programmable IO ports

        //NEED TO DEFINE RULES HERE



        } else if (port.pinType == HMI) {
            // Specific validation for programmable IO ports


        //NEED TO DEFINE RULES HERE


        }else if (port.pinType == ENCODER) {
            // Specific validation for programmable IO ports


        //NEED TO DEFINE RULES HERE


        } else {
            sendErrorResponse(client, "Invalid pin type for activation");
            return;
        }
    }

    // Save the updated settings to flash or other persistent storage
    PortManager::getInstance().writeToFlash();
    LOG("Port settings validated and written to flash.");
    sendSuccessResponse(client, "SUCCESS: Port " + pinDescription + " updated!");
}


void AdminServerManager::sendSuccessResponse(EthernetClient& client, const String& message) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.println();
    client.println("{\"status\":\"SUCCESS\",\"message\":\"" + message + "\"}");
    client.stop();
}
void AdminServerManager::sendSuccessResponseData(EthernetClient& client, const String& message, const String& data) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.println();
    client.println("{\"status\":\"SUCCESS\",\"message\":\"" + message + "\",\"data\":" + data + "}");
    client.stop();
}

void AdminServerManager::sendErrorResponse(EthernetClient& client, const String& message) {
    client.println("HTTP/1.1 400 Bad Request");
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.println();
    client.println("{\"status\":\"FAILURE\",\"message\":\"" + message + "\"}");
    client.stop();
}

// Generic function to handle GET requests for port listings
void AdminServerManager::handleGetPorts(EthernetClient& client, const String& request, int contentLength, const String &authToken, PIN_TYPE pinType) {
    if(authToken != m_activeToken){
        sendErrorResponse(client, "FAILURE: Un-Authorized Access Request");
        return;
    }
    String response = "{\"activePorts\":[";
    bool firstActive = true;
    bool firstInactive = true;
    String inactivePorts = "],\"inactivePorts\":[";

    for (int i = 0; i < MAX_PORTS; ++i) {
        if (PortManager::getInstance().settings.ports[i].pinType == pinType) {
            String portJson = "{";
            portJson += "\"index\":" + String(i) + ",";
            portJson += "\"readPinNumber\":" + String(PortManager::getInstance().settings.ports[i].readPinNumber) + ",";
            portJson += "\"writePinNumber\":" + String(PortManager::getInstance().settings.ports[i].writePinNumber) + ",";
            portJson += "\"pinType\":\"" + PortManager::getInstance().pinTypeToString(PortManager::getInstance().settings.ports[i].pinType) + "\",";
            portJson += "\"circuitType\":\"" + PortManager::getInstance().circuitTypeToCode(PortManager::getInstance().settings.ports[i].circuitType) + "\",";
            portJson += "\"pinDescription\":\"" + String(PortManager::getInstance().settings.ports[i].pinDescription) + "\",";
            portJson += "\"isActive\":" + String(PortManager::getInstance().settings.ports[i].isActive ? "true" : "false") + ",";
            portJson += "\"isSimulated\":" + String(PortManager::getInstance().settings.ports[i].isSimulated ? "true" : "false") + ",";
            portJson += "\"currentReading\":" + String(PortManager::getInstance().settings.ports[i].currentReading) + ",";
            portJson += "\"lastReading\":" + String(PortManager::getInstance().settings.ports[i].lastReading) + ",";
            portJson += "\"state\":\"" + String(PortManager::getInstance().settings.ports[i].state) + "\",";
            portJson += "\"lastUpdated\":\"" + PortManager::getInstance().timeToString(PortManager::getInstance().settings.ports[i].lastUpdated) + "\"";
            portJson += "}";

            if (PortManager::getInstance().settings.ports[i].isActive) {
                if (!firstActive) {
                    response += ",";
                }
                response += portJson;
                firstActive = false;
            } else {
                if (!firstInactive) {
                    inactivePorts += ",";
                }
                inactivePorts += portJson;
                firstInactive = false;
            }
        }
    }
    response += inactivePorts + "]}";

    // Send the JSON response using the helper function
    sendSuccessResponseData(client, "SUCCESS: Ports retrieved.", response);
}

// Generic function to generate the HTML landing page for port types
void AdminServerManager::handlePortsPage(EthernetClient& client, const String& request, int contentLength, const String &authToken, const String& pageTitle) {
    if(authToken != m_activeToken){
        handleError(client, 404, "FAILURE: Un-Authorized Access Request");
        return;
    }

    // Build port landing page
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println("Set-Cookie: authToken=" + m_activeToken + "; Path=/; Max-Age=86400; SameSite=None;"); // This sets the token as a cookie valid for 24 hours
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html lang='en'>");
    client.print(ResourceHandler::getHeaderRefresh(pageTitle));
    client.println("<body>");
    client.print(ResourceHandler::getHeaderMenu());
    client.print(ResourceHandler::getPortMainContent());
    client.print(ResourceHandler::getFooter());
    client.println("</body>");
    client.println("</html>");
    client.stop();
}

// Handle Digital IN Ports
void AdminServerManager::handleDigitalINPorts(EthernetClient& client, const String& request, int contentLength, const String &authToken) {
    handlePortsPage(client, request, contentLength, authToken, "Digital IN Port Manager");
}

void AdminServerManager::handleGetDigitalINPorts(EthernetClient& client, const String& request, int contentLength, const String &authToken) {
    handleGetPorts(client, request, contentLength, authToken, DIGITAL_INPUT);
}

// Handle Digital OUT Ports
void AdminServerManager::handleDigitalOUTPorts(EthernetClient& client, const String& request, int contentLength, const String &authToken) {
    handlePortsPage(client, request, contentLength, authToken, "Digital OUT Port Manager");
}

void AdminServerManager::handleGetDigitalOUTPorts(EthernetClient& client, const String& request, int contentLength, const String &authToken) {
    handleGetPorts(client, request, contentLength, authToken, DIGITAL_OUTPUT);
}

// Handle Programmable IO Ports
void AdminServerManager::handleProgrammableIOPorts(EthernetClient& client, const String& request, int contentLength, const String &authToken) {
    handlePortsPage(client, request, contentLength, authToken, "Programmable IO Port Manager");
}

void AdminServerManager::handleGetProgrammableIOPorts(EthernetClient& client, const String& request, int contentLength, const String &authToken) {
    handleGetPorts(client, request, contentLength, authToken, PROGRAMMABLE_IO);
}

// Handle HMI Ports
void AdminServerManager::handleHMIPorts(EthernetClient& client, const String& request, int contentLength, const String &authToken) {
    handlePortsPage(client, request, contentLength, authToken, "HMI Port Manager");
}

void AdminServerManager::handleGetHMIPorts(EthernetClient& client, const String& request, int contentLength, const String &authToken) {
    handleGetPorts(client, request, contentLength, authToken, HMI);
}

// Handle Temperature Ports
void AdminServerManager::handlePTEMPPorts(EthernetClient& client, const String& request, int contentLength, const String &authToken) {
    handlePortsPage(client, request, contentLength, authToken, "Temperature Port Manager");
}

void AdminServerManager::handleGetPTEMPPorts(EthernetClient& client, const String& request, int contentLength, const String &authToken) {
    handleGetPorts(client, request, contentLength, authToken, PTEMP);
}

// Handle Encoder Ports
void AdminServerManager::handleENCODERPorts(EthernetClient& client, const String& request, int contentLength, const String &authToken) {
    handlePortsPage(client, request, contentLength, authToken, "Encoder Port Manager");
}

void AdminServerManager::handleGetENCODERPorts(EthernetClient& client, const String& request, int contentLength, const String &authToken) {
    handleGetPorts(client, request, contentLength, authToken, ENCODER);
}

void AdminServerManager::handleAnalogINPorts(EthernetClient& client, const String& request,int contentLength, const String &authToken) {
    handlePortsPage(client, request, contentLength, authToken, "Analog IN Port Manager");
}

void AdminServerManager::handleGetAnalogINPorts(EthernetClient& client, const String& request,int contentLength, const String &authToken) {
    // Similar to handleGetAnalogPorts but for Analog IN
    handleGetPorts(client, request, contentLength, authToken, ANALOG_INPUT);
}

void AdminServerManager::handleAnalogOUTPorts(EthernetClient& client, const String& request,int contentLength, const String &authToken) {
    handlePortsPage(client, request, contentLength, authToken, "Analog OUT Port Manager");
}

void AdminServerManager::handleGetAnalogOUTPorts(EthernetClient& client, const String& request,int contentLength, const String &authToken) {
    handleGetPorts(client, request, contentLength, authToken, ANALOG_OUTPUT);
}

void AdminServerManager::handleAllActivePorts(EthernetClient& client, const String& request,int contentLength, const String &authToken) {
    if(authToken != m_activeToken){
        handleError(client, 404, "FAILURE: Un-Authorized Access Request to /ports/active");
        return;
    }

    // Build the All Active Ports page
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println("Set-Cookie: authToken=" + m_activeToken + "; Path=/; Max-Age=86400; SameSite=None;"); // Set the token as a cookie valid for 24 hours
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html lang='en'>");
    client.print(ResourceHandler::getHeaderRefresh("All Active Port Manager"));
    client.println("<body>");
    client.print(ResourceHandler::getHeaderMenu());
    client.print(ResourceHandler::getActivePortMainContent());
    client.print(ResourceHandler::getFooter());
    client.println("</body>");
    client.println("</html>");
    client.stop();
}

void AdminServerManager::handleGetAllActivePorts(EthernetClient& client, const String& request, int contentLength, const String &authToken) {
    if(authToken != m_activeToken){
        sendErrorResponse(client, "FAILURE: Un-Authorized Access Request");
        return;
    }
    String response = "{\"activePorts\":[";
    bool firstActive = true;

    for (int i = 0; i < MAX_PORTS; ++i) {
        if (PortManager::getInstance().settings.ports[i].isActive) {
            String portJson = "{";
            portJson += "\"index\":" + String(i) + ",";
            portJson += "\"readPinNumber\":" + String(PortManager::getInstance().settings.ports[i].readPinNumber) + ",";
            portJson += "\"writePinNumber\":" + String(PortManager::getInstance().settings.ports[i].writePinNumber) + ",";
            portJson += "\"pinType\":\"" + PortManager::getInstance().pinTypeToString(PortManager::getInstance().settings.ports[i].pinType) + "\",";
            portJson += "\"circuitType\":\"" + PortManager::getInstance().circuitTypeToCode(PortManager::getInstance().settings.ports[i].circuitType) + "\",";
            portJson += "\"pinDescription\":\"" + String(PortManager::getInstance().settings.ports[i].pinDescription) + "\",";
            portJson += "\"isSimulated\":" + String(PortManager::getInstance().settings.ports[i].isSimulated ? "true" : "false") + ",";
            portJson += "\"isActive\":" + String(PortManager::getInstance().settings.ports[i].isActive ? "true" : "false") + ",";
            portJson += "\"currentReading\":" + String(PortManager::getInstance().settings.ports[i].currentReading) + ",";
            portJson += "\"lastReading\":" + String(PortManager::getInstance().settings.ports[i].lastReading) + ",";
            portJson += "\"state\":\"" + String(PortManager::getInstance().settings.ports[i].state) + "\",";
            portJson += "\"lastUpdated\":\"" + PortManager::getInstance().timeToString(PortManager::getInstance().settings.ports[i].lastUpdated) + "\"";
            portJson += "}";

            if (!firstActive) {
                response += ",";
            }
            response += portJson;
            firstActive = false;
        }
    }
    response += "]}";
    sendSuccessResponseData(client, "SUCCESS: All active ports retrieved.", response);
}
void AdminServerManager::handleHelp(EthernetClient& client, const String& request,int contentLength, const String &authToken){
if(authToken != m_activeToken){
        sendErrorResponse(client, "FAILURE: Un-Authorized Access Request.  Please login to view this file.");
        return;
    }
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println("Set-Cookie: authToken=" + m_activeToken + "; Path=/; Max-Age=86400; SameSite=None;");
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.println("<html lang='en'>");
  client.print(ResourceHandler::getHeader("Help"));
  client.println("<body>");
  client.print(ResourceHandler::getHeaderMenu());
  client.print(ResourceHandler::getHelpContent());
  client.print(ResourceHandler::getFooter());
  client.println("</body>");
  client.println("</html>");
  client.stop();
}

void AdminServerManager::handleTestConnection(EthernetClient& client, const String& request, int contentLength, const String& authToken) {
    LOG("SERVER CONNECTION TEST");
    if (authToken != m_activeToken) {
        handleError(client, 404, "Un-Authorized Access Request to /admin/test");
        return;
    }

    String host = String(PortManager::getInstance().settings.CALL_HOME_HOST);
    String path = "/api/test";

    String payload = "{}"; // Empty JSON payload

    String serverResponse = sendJsonToServer(host, path, payload);
    String message = getJSONValue(serverResponse, "message");
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.println();
    client.print("{\"message\":\"");
    client.print(message);
    client.println("\"}");
    client.stop();
}

String AdminServerManager::getJSONValue(const String& json, const String& key) {
    String searchKey = "\"" + key + "\":\"";
    int startIndex = json.indexOf(searchKey);
    if (startIndex == -1) return "";

    startIndex += searchKey.length();
    int endIndex = json.indexOf("\"", startIndex);
    if (endIndex == -1) return "";

    return json.substring(startIndex, endIndex);
}

String AdminServerManager::sendJsonToServer(const String& host, const String& path, const String& payload) {
    EthernetClient httpClient;
    String response = "{\"status\":\"error\",\"message\":\"Failed to connect to server\"}";

    if (httpClient.connect(host.c_str(), 80)) {
        // Send HTTP POST request
        httpClient.println("POST " + path + " HTTP/1.1");
        httpClient.println("Host: " + host);
        httpClient.println("Content-Type: application/json");
        httpClient.println("Content-Length: " + String(payload.length()));
        httpClient.println("Connection: close");
        httpClient.println();
        httpClient.println(payload);

        // Wait for server response
        while (httpClient.connected()) {
            if (httpClient.available()) {
                String line = httpClient.readStringUntil('\n');
                if (line == "\r") {
                    break; // Headers ended, body starts next
                }
            }
        }

        // Read the response body
        String responseBody;
        while (httpClient.available()) {
            char c = httpClient.read();
            responseBody += c;
        }

        if (responseBody.length() > 0) {
            response = responseBody;
        }

        httpClient.stop();
    }
    return response;
}

void AdminServerManager::handleRegister(EthernetClient& client, const String& request, int contentLength, const String& authToken) {
    if (authToken != m_activeToken) {
        handleError(client, 404, "Un-Authorized Access Request to /admin/register");
        return;
    }

    String payload = generateRegistrationPayload();
    String response = sendJsonToServer(PortManager::getInstance().settings.CALL_HOME_HOST, "/api/register", payload);

    String message = getJSONValue(response, "message");
    if (getJSONValue(response, "status") == "success") {
        PortManager::getInstance().settings.REGISTRATION_STATUS = true;
        PortManager::getInstance().writeToFlash();
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.println();
    client.print("{\"message\":\"");
    client.print(message);
    client.println("\"}");
    client.stop();
}

void AdminServerManager::handleUnregister(EthernetClient& client, const String& request, int contentLength, const String& authToken) {
    if (authToken != m_activeToken) {
        handleError(client, 404, "Un-Authorized Access Request to /admin/unregister");
        return;
    }

    // Construct payload for unregistering
    String payload = generateUnregistrationPayload();
    String response = sendJsonToServer(PortManager::getInstance().settings.CALL_HOME_HOST, "/api/unregister", payload);

    String message = getJSONValue(response, "message");
    if (getJSONValue(response, "status") == "success") {
        PortManager::getInstance().settings.REGISTRATION_STATUS = false;
        PortManager::getInstance().writeToFlash();
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.println();
    client.print("{\"message\":\"");
    client.print(message);
    client.println("\"}");
    client.stop();
}

String AdminServerManager::generateRegistrationPayload() {
    String payload = "{";
    payload += "\"serialNumber\":\"" + String(PortManager::getInstance().settings.SERIAL_NUMBER) + "\",";
    payload += "\"sharedSecret\":\"" + String(PortManager::getInstance().settings.SHARED_SECRET) + "\",";
    payload += "\"model\":\"" + String(PortManager::getInstance().settings.MODEL) + "\",";
    payload += "\"ipAddress\":\"" + PortManager::getInstance().ipToString(PortManager::getInstance().settings.IP_ADDRESS) + "\",";
    payload += "\"macAddress\":\"" + PortManager::getInstance().macToString(PortManager::getInstance().settings.MAC) + "\"";
    payload += "}";

    LOG("Sending Registration Payload: " + payload);

    return payload;
}


String AdminServerManager::generateUnregistrationPayload() {
    String payload = "{";
    payload += "\"serialNumber\":\"" + String(PortManager::getInstance().settings.SERIAL_NUMBER) + "\",";
    payload += "\"sharedSecret\":\"" + String(PortManager::getInstance().settings.SHARED_SECRET) + "\"";
    payload += "}";
    return payload;
}
