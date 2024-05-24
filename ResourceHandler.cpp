/*
  File: ResourceHandler.c00
  Date Created: May 11, 2024
  Authors: DaPhisha, ChatGPT, and Assistant
  Description: def file for the resource handler class.
  Last Modified: May 11, 2024
  Version 2.0.0
*/
#include "ResourceHandler.h"

const char* ResourceHandler::getCSS() {
    return R"(
    /* General Styles */
    /* General Styles */
body {
    font-family: Arial, sans-serif;
    margin: 0;
    padding: 0;
    box-sizing: border-box;
}

/* Header, Footer, and Button Colors */
:root {
    --header-bg-color: #03A89C;
    --footer-bg-color: #333;
    --button-bg-color: #03A89C;
    --button-text-color: white;
}

/* Header */
.header {
    background-color: var(--header-bg-color);
    color: white;
    padding: 10px 20px;
}

.navbar {
    display: flex;
    justify-content: space-between;
    align-items: center;
}

.navbar-menu {
    display: flex;
}

.navbar-menu a {
    color: white;
    padding: 0px 10px;
    text-decoration: none;
}
.navbar-menu .dropdown-toggle {
    padding: 10px 20px;
    cursor: pointer;
    color: white;
    text-decoration: none;
}

.navbar-menu .dropdown-toggle:hover {
    background-color: rgba(255, 255, 255, 0.1); /* Optional hover effect */
}


.navbar-toggle {
    display: none;
    background: none;
    border: none;
    color: white;
    font-size: 24px;
}
/* Dropdown Styles */
.dropdown {
    position: relative;
    display: inline-block;
}

.dropdown-menu {
    display: none;
    position: absolute;
    background-color: #f9f9f9;
    box-shadow: 0 8px 16px rgba(0, 0, 0, 0.2);
    z-index: 1;
    min-width: 160px;
}

.dropdown-menu a {
    color: black;
    padding: 12px 16px;
    text-decoration: none;
    display: block;
}

.dropdown-menu a:hover {
    background-color: #f1f1f1;
}

.dropdown:hover .dropdown-menu {
    display: block;
}

/* Main Content */
.main-content {
    padding: 20px;
}

/* Centered Message */
.centered-message {
    color: green;
    margin: 20px 0;
    text-align: center;
}

/* Login Form */
.login-container {
    max-width: 300px;
    margin: auto;
    padding: 20px;
    border: 1px solid #ddd;
    border-radius: 5px;
    box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
}

.login-form h2 {
    margin-bottom: 20px;
}

.login-form label {
    display: block;
    margin-bottom: 5px;
}

.login-form input {
    width: 100%;
    padding: 8px;
    margin-bottom: 10px;
    border: 1px solid #ddd;
    border-radius: 5px;
}

.login-form button {
    width: 100%;
    padding: 10px;
    background-color: var(--button-bg-color);
    color: var(--button-text-color);
    border: none;
    border-radius: 5px;
    cursor: pointer;
}

.login-form button:hover {
    background-color: #45a049;
}

/* Cards */
.cards-container {
    display: flex;
    flex-wrap: wrap;
    gap: 20px;
    justify-content: center;
}

.card {
    display: flex;
    flex-direction: column;
    border: 1px solid #ddd;
    border-radius: 5px;
    box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
    transition: box-shadow 0.3s ease, transform 0.3s ease;
}

.card:hover {
    box-shadow: 0 8px 15px rgba(0, 0, 0, 0.2);
    transform: translateY(-5px);
}

.card-header, .card-footer {
    padding: 10px;
    background-color: #b3b3b3;
    border-bottom: 1px solid #ddd;
}

.card-body {
    flex: 1; /* This makes the card-body grow to fit its content */
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
    padding: 10px;
}

.card.small {
    width: 150px;
}

.card.medium {
    width: 250px;
}

.card.large {
    width: 350px;
}

/* Footer */
.footer {
    background-color: var(--footer-bg-color);
    color: white;
    text-align: center;
    padding: 10px;
    position: fixed;
    bottom: 0;
    width: 100%;
}

/* Centering Classes */
.container {
    display: flex;
    justify-content: center;
    align-items: center;
    height: 100vh;
}

.center {
    display: flex;
    justify-content: center;
    align-items: center;
    text-align: center;
}

/* Responsive */
@media (max-width: 768px) {
    .navbar-menu {
        display: none;
        flex-direction: column;
    }
    .navbar-menu.active {
        display: flex;
    }
    .navbar-toggle {
        display: block;
    }
    .stat-card {
        flex: 1 1 100%;
    }
}

/* Additional Styles for 404 Page */
.error-container {
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
    height: 100vh;
    text-align: center;
}

.error-title {
    font-size: 72px;
    color: var(--header-bg-color);
    margin-bottom: 20px;
}

.error-message {
    font-size: 24px;
    color: #333;
    margin-bottom: 40px;
}

.error-button {
    padding: 10px 20px;
    margin: 10px;
    background-color: var(--button-bg-color);
    color: var(--button-text-color);
    border: none;
    border-radius: 5px;
    text-decoration: none;
    font-size: 18px;
    cursor: pointer;
    transition: background-color 0.3s ease;
}

.error-button:hover {
    background-color: #45a049;
}

/* Stat Cards Container */
.stats-container {
    display: flex;
    flex-wrap: wrap;
    gap: 20px;
    justify-content: center;
    padding: 20px;
}

/* Stat Card */
.stat-card {
    width: 250px;
    height: 250px;
    background-color: #fff;
    border: 1px solid #ddd;
    border-radius: 5px;
    box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
    display: flex;
    flex-direction: column;
    justify-content: flex-start; /* Changed to ensure header and content alignment */
    align-items: center;
    text-align: center;
    transition: box-shadow 0.3s ease, transform 0.3s ease;
    overflow: hidden; /* Ensure the card contents do not overflow */
}

.stat-card:hover {
    box-shadow: 0 8px 15px rgba(0, 0, 0, 0.2);
    transform: translateY(-5px);
}

/* Stat Card Header */
.stat-card-header {
    width: 100%;
    background-color: #b3b3b3;
    padding: 10px 0;
    border-bottom: 1px solid #ddd;
    text-align: center;
    font-weight: bold;
    position: sticky;
    top: 0;
    z-index: 1;
}

/* Stat Card Content */
.stat-card-content {
    padding: 10px;
    flex: 1;
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
}

/* Admin Form Container */
.admin-container {
    display: flex;
    flex-wrap: wrap;
    gap: 20px;
    justify-content: center;
    padding: 20px;
    align-items: flex-start; /* Ensures cards align to the top */
}

/* Admin Card */
.admin-card {
    width: 250px; /* Adjust width as needed */
    background-color: #fff;
    border: 1px solid #ddd;
    border-radius: 5px;
    box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
    display: inline-block;
    /*flex-direction: column;*/
    transition: box-shadow 0.3s ease, transform 0.3s ease;
    margin: 10px; /* Ensure there's spacing between cards */
}

.admin-card:hover {
    box-shadow: 0 8px 15px rgba(0, 0, 0, 0.2);
    transform: translateY(-5px);
}

/* Admin Card Header */
.admin-card-header {
    width: 100%;
    background-color: #b3b3b3;
    padding: 0px;
    border-bottom: 1px solid #ddd;
    text-align: center;
    font-weight: bold;
}

/* Admin Card Content */
.admin-card-content {
    padding: 10px;
    display: flex;
    flex-direction: column;
    gap: 10px;
}

.admin-card-content label {
    font-weight: bold;
}

.admin-card-content input[type='text'],
.admin-card-content input[type='password'],
.admin-card-content input[type='number'] {
    width: 100%;
    padding: 8px;
    border: 1px solid #ddd;
    border-radius: 5px;
}

.admin-card-content input[type='checkbox'] {
    margin-left: 10px;
}

/* Submit Button */
.admin-submit-button {
    padding: 10px 20px;
    background-color: var(--button-bg-color);
    color: var(--button-text-color);
    border: none;
    border-radius: 5px;
    cursor: pointer;
    font-size: 16px;
    margin-top: 20px;
    margin-bottom: 20px;
}

.admin-submit-button:hover {
    background-color: #45a049;
}

/* Ports Container */
.ports-container {
    display: flex;
    flex-direction: column;
    gap: 20px;
    padding: 20px;
}

/* Active and Inactive Ports */
.active-ports, .inactive-ports {
    display: flex;
    flex-wrap: wrap;
    gap: 20px;
    justify-content: center;
}

/* Port Card */
.port-card {
    width: 250px;
    height: auto;
    background-color: #fff;
    border: 1px solid #ddd;
    border-radius: 5px;
    box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
    display: flex;
    flex-direction: column;
    transition: box-shadow 0.3s ease, transform 0.3s ease;
    overflow: hidden;
}

.port-card:hover {
    box-shadow: 0 8px 15px rgba(0, 0, 0, 0.2);
    transform: translateY(-5px);
}

/* Port Card Header */
.port-card-header {
    width: 100%;
    background-color: #b3b3b3;
    padding: 10px;
    border-bottom: 1px solid #ddd;
    text-align: center;
    font-weight: bold;
}

/* Port Card Content */
.port-card-content {
    padding: 10px;
    display: flex;
    flex-direction: column;
    gap: 10px;
}

/* Port Card Fields */
.port-card-content label {
    font-weight: bold;
}

.port-card-content input[type='text'],
.port-card-content select {
    width: 100%;
    padding: 8px;
    border: 1px solid #ddd;
    border-radius: 5px;
}

.port-card-content input[type='checkbox'] {
    margin-left: 10px;
}

.active-port {
    background-color: #e0f7fa;
}

.inactive-port {
    background-color: #f0f0f0;
}


    )";
}
const char* ResourceHandler::getJavascript() {
    return R"(
    
    document.addEventListener('DOMContentLoaded', function() {
    const version = '1.0.5';
    console.log(`Script version: ${version}`);

    const navbarToggle = document.getElementById('navbarToggle');
    const navbarMenu = document.getElementById('navbarMenu');
    if (navbarToggle && navbarMenu) {
        navbarToggle.addEventListener('click', function() {
            navbarMenu.classList.toggle('active');
        });
    }

    const portsDropdown = document.getElementById('portsDropdown');
    const portsDropdownMenu = document.getElementById('portsDropdownMenu');
    if (portsDropdown && portsDropdownMenu) {
        portsDropdown.addEventListener('click', function() {
            portsDropdownMenu.classList.toggle('active');
        });
    }

    const togglePassword = document.getElementById('togglePassword');
    const passwordField = document.getElementById('Admin_PASSWORD');
    if (togglePassword && passwordField) {
        togglePassword.addEventListener('change', function() {
            passwordField.type = this.checked ? 'text' : 'password';
        });
    }

    const toggleWifiPassword = document.getElementById('toggleWifiPassword');
    const wifiPasswordField = document.getElementById('WIFI_PASSWORD');
    if (toggleWifiPassword && wifiPasswordField) {
        toggleWifiPassword.addEventListener('change', function() {
            wifiPasswordField.type = this.checked ? 'text' : 'password';
        });
    }

    const form = document.querySelector('.admin-form');
    if (form) {
        form.addEventListener('submit', function(event) {
            event.preventDefault();
            console.log('Admin Settings Update Request');

            const formData = new FormData(form);
            const params = new URLSearchParams();
            formData.forEach((value, key) => {
                //console.log(`Form data: ${key} = ${value}`);
                params.append(key, decodeURIComponent(value.replace(/\+/g, ' ')));
            });

            const xhr = new XMLHttpRequest();
            xhr.open('POST', form.action, true);
            xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
            xhr.onload = function() {
                const errorMessage = document.getElementById('error-message');
                try {
                    const response = JSON.parse(xhr.responseText);
                    console.log('Response received:', response);
                    errorMessage.textContent = response.message;
                    errorMessage.style.color = xhr.status === 200 ? 'green' : 'red';
                } catch (e) {
                    console.error('Error parsing JSON response:', e);
                    errorMessage.textContent = 'Error parsing response';
                    errorMessage.style.color = 'red';
                }
            };
            xhr.onerror = function() {
                const errorMessage = document.getElementById('error-message');
                errorMessage.textContent = 'Request failed';
                errorMessage.style.color = 'red';
                console.error('Request failed');
            };
            xhr.send(params.toString());
        });
    }

    // Function to attach event listeners to port update forms
    function attachPortFormListeners() {
        const portForms = document.querySelectorAll('.port-card form');
        portForms.forEach(form => {
            form.addEventListener('submit', function(event) {
                event.preventDefault();
                console.log('Port Update Request.');

                const formData = new FormData(form);
                const params = new URLSearchParams();
                formData.forEach((value, key) => {
                    //console.log(`Port form data: ${key} = ${value}`);
                    params.append(key, decodeURIComponent(value.replace(/\+/g, ' ')));
                });

                const xhr = new XMLHttpRequest();
                xhr.open('POST', form.action, true);
                xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
                xhr.onload = function() {
                    const errorMessage = document.getElementById('error-message');
                    try {
                        const response = JSON.parse(xhr.responseText);
                        console.log('Response received:', response);
                        errorMessage.textContent = response.message;
                        errorMessage.style.color = xhr.status === 200 ? 'green' : 'red';
                        if (xhr.status === 200 && response.status === 'SUCCESS') {
                            console.log('Port update successful');
                            updatePortLists(response.message); // Refresh the port lists if the update was successful
                        } else {
                            console.log('Port update failed');
                        }
                    } catch (e) {
                        console.error('Error parsing JSON response:', e);
                        errorMessage.textContent = 'Error parsing response';
                        errorMessage.style.color = 'red';
                    }
                };
                xhr.onerror = function() {
                    console.error('Request failed');
                    const errorMessage = document.getElementById('error-message');
                    errorMessage.textContent = 'Request failed';
                    errorMessage.style.color = 'red';
                };
                xhr.send(params.toString());
                console.log('Request sent with data:', params.toString());
            });
        });
    }

    // Initial fetch of port lists if on the ports page
    if (window.location.pathname.startsWith('/ports/')) {
        updatePortLists('Manage your analog ports below.');
    }

    function updatePortLists(message) {
        const xhr = new XMLHttpRequest();
        xhr.open('GET', '/ports/analog/list', true);
        xhr.onload = function() {
            const errorMessage = document.getElementById('error-message');
            console.log('Updating port lists.');
            if (xhr.status === 200) {
                try {
                    const response = JSON.parse(xhr.responseText);
                    errorMessage.textContent = message || response.message;
                    errorMessage.style.color = 'green';

                    const activePortsContainer = document.querySelector('.active-ports');
                    const inactivePortsContainer = document.querySelector('.inactive-ports');
                    activePortsContainer.innerHTML = '';
                    inactivePortsContainer.innerHTML = '';

                    response.data.activePorts.forEach(port => {
                        activePortsContainer.innerHTML += createPortCard(port, true);
                    });

                    response.data.inactivePorts.forEach(port => {
                        inactivePortsContainer.innerHTML += createPortCard(port, false);
                    });

                    // Attach event listeners to the newly created forms
                    attachPortFormListeners();
                } catch (e) {
                    console.error('Error parsing JSON response:', e);
                    errorMessage.textContent = 'Error parsing JSON response';
                    errorMessage.style.color = 'red';
                }
            } else {
                errorMessage.textContent = 'Failed to update port lists';
                errorMessage.style.color = 'red';
            }
        };
        xhr.onerror = function() {
            console.error('Request to update port lists failed');
            const errorMessage = document.getElementById('error-message');
            errorMessage.textContent = 'Request to update port lists failed';
            errorMessage.style.color = 'red';
        };
        xhr.send();
    }

    function createPortCard(port, isActive) {
        return `
            <div class='port-card ${isActive ? 'active-port' : 'inactive-port'}'>
                <div class='port-card-header'>${port.pinDescription}</div>
                <div class='port-card-content'>
                    <form action='/ports/update' method='POST'>
                        <label for='circuitType'>Circuit Type</label>
                        <select id='circuitType' name='circuitType'>
                            <option value='ONOFF' ${port.circuitType === 'ONOFF' ? 'selected' : ''}>ON/OFF</option>
                            <option value='MA420' ${port.circuitType === 'MA420' ? 'selected' : ''}>4-20mA</option>
                            <option value='CTEMP' ${port.circuitType === 'CTEMP' ? 'selected' : ''}>Temperature</option>
                            <option value='VALVE' ${port.circuitType === 'VALVE' ? 'selected' : ''}>Valve</option>
                            <option value='FILL' ${port.circuitType === 'FILL' ? 'selected' : ''}>Fill</option>
                            <option value='PULSE' ${port.circuitType === 'PULSE' ? 'selected' : ''}>Pulse</option>
                        </select>
                        <label for='pinDescription'>Description</label>
                        <input type='text' id='pinDescription' name='pinDescription' value='${decodeURIComponent(port.pinDescription)}' required>
                        <label for='isActive'>Active</label>
                        <input type='checkbox' id='isActive' name='isActive' ${port.isActive ? 'checked' : ''}>
                        <label for='isSimulated'>Simulated</label>
                        <input type='checkbox' id='isSimulated' name='isSimulated' ${port.isSimulated ? 'checked' : ''}>
                        <input type='hidden' name='portIndex' value='${port.index}'>
                        <button type='submit'>Update</button>
                    </form>
                </div>
            </div>
        `;
    }
});

    
    )";
}

const char* ResourceHandler::getHeader(String title) {
    String header = R"(
      <head>
<meta charset='UTF-8'>
<meta name='viewport' content='width=device-width, initial-scale=1.0'>
<title>)";
    header += title;
    header += R"(</title>
<link rel='stylesheet' href='/css/style.css'>
</head>
    )";
    return header.c_str();
}

const char* ResourceHandler::getHeaderMenu(){
return R"(
<header class='header'>
    <nav class='navbar'>
        <div class='navbar-brand'>Protonify Agent 1.0</div>
        <div class='navbar-menu' id='navbarMenu'>
            <a href='/home'>Home</a>
            <a href='/admin'>Admin</a>
            <div class='dropdown'>
                <span class='dropdown-toggle' id='portsDropdown'>Ports</span>
                <div class='dropdown-menu' id='portsDropdownMenu'>
                    <a href='/ports/analog'>Analog</a>
                    <a href='/ports/digitalIN'>Digital IN</a>
                    <a href='/ports/digitalOUT'>Digital OUT</a>
                    <a href='/ports/programmableIO'>Programmable IO</a>
                    <a href='/ports/HMI'>HMI</a>
                    <a href='/ports/temperature'>Temperature</a>
                    <a href='/ports/encoders'>Encoders</a>
                </div>
            </div>
            <a href='/console'>Console</a>
            <a href='/logout'>Logout</a>
        </div>
        <button class='navbar-toggle' id='navbarToggle'>☰</button>
    </nav>
</header>


  )";
}
const char* ResourceHandler::getResetCard(){
return R"(
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
         )";
}

const char* ResourceHandler::getMessageDiv(String message){
    String html = "<br><p class='centered-message' id='error-message'>" + message + "</p><br>";
    char* htmlCStr = new char[html.length() + 1];
    strcpy(htmlCStr, html.c_str());
    return htmlCStr; 
}

const char* ResourceHandler::getPortMainContent(){
return R"(
<main class='main-content'>
    <br><p class='centered-message' id='error-message'>Manage your analog ports below.</p><br>
    <div class='ports-container'>
        <h2>Active Ports</h2>
        <div class='active-ports'>
            <!-- Active port cards will be inserted here -->
        </div>
        <h2>Inactive Ports</h2>
        <div class='inactive-ports'>
            <!-- Inactive port cards will be inserted here -->
        </div>
    </div>
</main>
   )";
}

const char* ResourceHandler::getLoginCard(){
return R"(
        <div class='card large center'>
          <div class='card-body'>
            <main class='main-content'>
            <!-- Login Form -->
            <div class='login-container' style='background-color: white;'>
              <form class='login-form' action='/login' method='POST'>
                <h2>Login</h2>
                <label for='username'>Username:</label>
                <input type='text' id='username' name='username' required>
                <label for='password'>Password:</label>
                <input type='password' id='password' name='password' required>
                <button type='submit'>Login</button>
              </form>
            </div>
          </div>
        </div><!-- End Large Card  -->
    )";


}

const char* ResourceHandler::getFooter() {
    return R"(
        <footer class='footer'>
        <p>Protonify &copy; 2024 Powered by DaPhisha</p>
        <script src="/js/script.js"></script>
        </footer>
    )";
}

const char* ResourceHandler::getStatCard(String title, String value) {
    String statCard = R"(
        <div class='stat-card'>
          <div class='stat-card-header'>System Stats</div>
            <div class='stat-card-content'>
              <h3>)";
                statCard += title;
                statCard += R"(</h3>
                    <p>)";
                statCard += value;
                statCard += R"(</p>
            </div>
        </div>
    )";
    return statCard.c_str();
}


