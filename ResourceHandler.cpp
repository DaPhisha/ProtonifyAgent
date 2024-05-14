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
body {
    font-family: Arial, sans-serif;
    margin: 0;
    padding: 0;
    box-sizing: border-box;
}

/* Header, Footer, and Button Colors */
:root {
    --header-bg-color: #4CAF50;
    --footer-bg-color: #333;
    --button-bg-color: #4CAF50;
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
    background-color: #f4f4f4;
    border-bottom: 1px solid #ddd;
}

.card-body {
    padding: 20px;
}

.card.small {
    width: 150px;
}

.card.medium {
    width: 300px;
}

.card.large {
    width: 450px;
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

const char* ResourceHandler::getHeader() {
    return R"(

    )";
}

const char* ResourceHandler::getLoginCard(){
return R"(
        <div class='card large center'>
          <div class='card-body'>
            <main class='main-content'>
            <!-- Login Form -->
            <div class='login-container' style='background-color: white;'>
              <form class='login-form' action='/login' method='post'>
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
