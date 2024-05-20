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
    padding: 10px 20px;
    text-decoration: none;
}

.navbar-toggle {
    display: none;
    background: none;
    border: none;
    color: white;
    font-size: 24px;
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
    padding: 10px;
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
}

.admin-submit-button:hover {
    background-color: #45a049;
}

    )";
}
const char* ResourceHandler::getJavascript() {
    return R"(
      document.addEventListener('DOMContentLoaded', function() {
        const navbarToggle = document.getElementById('navbarToggle');
        const navbarMenu = document.getElementById('navbarMenu');
        navbarToggle.addEventListener('click', function() {
          navbarMenu.classList.toggle('active');
        });
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
            <a href='/settings'>Settings</a>
             <a href='/console'>Console</a>
            <a href='/logout'>Logout</a>
        </div>
        <button class='navbar-toggle' id='navbarToggle'>☰</button>
    </nav>
</header>
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


