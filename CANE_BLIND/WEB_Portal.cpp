/******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include "Generic_API.h"
#include "WEB_Portal.h"

/******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************/
IPAddress apIP(192,168,4,1);
IPAddress nm(255,255,255,0);

/******************************************************************************
 * API
 ******************************************************************************/
/*================================================================================================*/
/**
* @brief        Initializes the WebPortal object and binds the Wi-Fi Manager.
* @details      The constructor initializes the internal ESP8266WebServer object on port 80 
*               and stores a pointer to the custom WifiManager to handle configuration saving 
*               after the portal is run.
*
* @param[in]    wm         Pointer to the custom WifiManager (WifiManagerCustom) instance.
* @param[out]   None
*
* @return       N/A
*
* @api
*/
/*================================================================================================*/
WebPortal::WebPortal(WifiManagerCustom* wm) : server(80) {
    wifi = wm;
}

/*================================================================================================*/
/**
* @brief        Sets up the device as a Wi-Fi Access Point and starts the Web Configuration Server.
* @details      This function enters the Captive Portal mode by setting the device to WIFI_AP, 
*               configuring its IP, starting the SoftAP, and launching the DNS server to redirect 
*               all traffic. It then defines the necessary routes ('/', '/save', '/style.css') 
*               before commencing the HTTP server.
*
* @param[in]    None
* @param[out]   None
*
* @return       void
*
* @api
*/
/*================================================================================================*/
void WebPortal::startPortal() {
    /* Set the Wi-Fi module to operate as an Access Point only. */
    WiFi.mode(WIFI_AP);                                         
    
    /* Configure the AP's static IP address (apIP), Gateway (apIP), and Subnet Mask (nm). */
    WiFi.softAPConfig(apIP, apIP, nm);                          
    
    /* Start the AP using the predefined credentials (AP_SSID, AP_PASS). */
    WiFi.softAP(AP_SSID, AP_PASS);                              

    /* Start the DNS server on port 53. It forces all DNS requests ('*') 
       to resolve to the AP's IP address (apIP), enabling the Captive Portal effect. */
    dns.start(53, "*", apIP);                                   

    /* Handle HTTP GET requests for the root path ('/') to display the configuration page. */
    server.on("/", HTTP_GET, std::bind(&WebPortal::handleRoot, this)); 
    
    /* Handle HTTP POST requests to '/save' for submitting and saving new Wi-Fi credentials. */
    server.on("/save", HTTP_POST, std::bind(&WebPortal::handleSave, this)); 
    
    /* Handle HTTP GET requests for external style sheets. */
    server.on("/style.css", HTTP_GET, std::bind(&WebPortal::handleCSS, this)); 

    /* Start the web server, making it listen for client connections on port 80. */
    server.begin();                                             
}

/*================================================================================================*/
/**
* @brief        Handles all incoming client requests for the Captive Portal.
* @details      This function must be called repeatedly inside the main application loop (e.g., Arduino loop()).
*               It processes DNS requests first to maintain the Captive Portal behavior, and then 
*               processes any pending HTTP requests for the web server.
*
* @param[in]    None
* @param[out]   None
*
* @return       void
*
* @api
*/
/*================================================================================================*/
void WebPortal::handleClient() {
    /* Processes the next incoming DNS query. This is crucial for the Captive Portal 
       to redirect web requests to the configuration page. */
    dns.processNextRequest();

    /* Handles any pending incoming HTTP requests on the web server (e.g., loading the page or submitting the form). */
    server.handleClient();
}

/*================================================================================================*/
/**
* @brief        Handles HTTP GET requests for the stylesheet ('/style.css').
* @details      This function serves the embedded CSS content for the configuration page. 
*               It uses a Raw String Literal (R"()") to embed a modern, responsive stylesheet, 
*               ensuring the Content-Type is correctly set to 'text/css'.
*
* @param[in]    None
* @param[out]   None
*
* @return       void
*
* @api
*/
/*================================================================================================*/
void WebPortal::handleCSS() {
    /* Define the Cascading Style Sheet (CSS) content using a C++ Raw String Literal (R"()"). */
    String css = R"(
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
        }
        
        body {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
            padding: 20px;
        }
        
        .container {
            background: white;
            border-radius: 20px;
            box-shadow: 0 20px 60px rgba(0, 0, 0, 0.3);
            width: 100%;
            max-width: 480px;
            padding: 40px;
            animation: fadeIn 0.6s ease-out;
        }
        
        @keyframes fadeIn {
            from { opacity: 0; transform: translateY(20px); }
            to { opacity: 1; transform: translateY(0); }
        }
        
        .header {
            text-align: center;
            margin-bottom: 30px;
        }
        
        .logo {
            font-size: 42px;
            color: #667eea;
            margin-bottom: 15px;
        }
        
        h2 {
            color: #333;
            font-size: 28px;
            font-weight: 600;
            margin-bottom: 5px;
        }
        
        .subtitle {
            color: #666;
            font-size: 14px;
            margin-bottom: 20px;
        }
        
        .current-network {
            background: #e8f5e9;
            border-radius: 12px;
            padding: 12px;
            margin-bottom: 25px;
            text-align: center;
            font-weight: 500;
            color: #2e7d32;
        }
        
        .form-group {
            margin-bottom: 20px;
        }
        
        label {
            display: block;
            margin-bottom: 8px;
            color: #444;
            font-weight: 500;
            font-size: 14px;
        }
        
        input[type="text"],
        input[type="password"] {
            width: 100%;
            padding: 14px;
            border: 2px solid #e0e0e0;
            border-radius: 10px;
            font-size: 16px;
            transition: all 0.3s ease;
            background: #fff;
        }
        
        input[type="text"]:focus,
        input[type="password"]:focus {
            outline: none;
            border-color: #667eea;
            box-shadow: 0 0 0 3px rgba(102, 126, 234, 0.1);
        }
        
        .btn {
            width: 100%;
            padding: 16px;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            border: none;
            border-radius: 10px;
            font-size: 16px;
            font-weight: 600;
            cursor: pointer;
            transition: all 0.3s ease;
        }
        
        .btn:hover {
            transform: translateY(-2px);
            box-shadow: 0 10px 25px rgba(102, 126, 234, 0.4);
        }
        
        .footer {
            text-align: center;
            margin-top: 25px;
            padding-top: 20px;
            border-top: 1px solid #eee;
            color: #888;
            font-size: 12px;
        }
        
        @media (max-width: 480px) {
            .container {
                padding: 25px;
            }
            
            h2 {
                font-size: 24px;
            }
        }
    )";
    
    /* Send the CSS content to the client with the correct Content-Type (text/css) and HTTP status code (200 OK). */
    server.send(200, "text/css", css);
}

/*================================================================================================*/
/**
* @brief        Handles HTTP GET requests for the root path ('/').
* @details      This function serves the main HTML page containing the Wi-Fi configuration form. 
*               It dynamically injects the currently saved SSID into the HTML before sending the response.
*
* @param[in]    None
* @param[out]   None
*
* @return       void
*
* @api
*/
/*================================================================================================*/
void WebPortal::handleRoot() {
    /* Define the start of the HTML page content using a Raw String Literal. */
    String html = R"(
        <!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>ESP32 WiFi Configuration</title>
            <link rel="stylesheet" href="/style.css">
            <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css">
        </head>
        <body>
            <div class="container">
                <div class="header">
                    <div class="logo">
                        <i class="fas fa-wifi"></i>
                    </div>
                    <h2>WiFi Configuration</h2>
                    <p class="subtitle">Connect your ESP32 device to WiFi network</p>
                </div>
                
                <div class="current-network">
                    <i class="fas fa-network-wired"></i>
                    Currently configured: <strong>)";
    
    /* Dynamically append the currently saved SSID from the WifiManager object. */
    html += wifi->savedSSID;
    
    /* Define the remainder of the HTML content, including the form and closing tags. */
    html += R"(</strong>
                </div>
                
                <form method='POST' action='/save'>
                    <div class="form-group">
                        <label for="ssid">
                            <i class="fas fa-network-wired"></i>
                            WiFi Network Name (SSID)
                        </label>
                        <input type="text" id="ssid" name="ssid" 
                               placeholder="Enter your WiFi network name" 
                               required autofocus>
                    </div>
                    
                    <div class="form-group">
                        <label for="pass">
                            <i class="fas fa-lock"></i>
                            WiFi Password
                        </label>
                        <input type="password" id="pass" name="pass" 
                               placeholder="Enter your WiFi password"
                               required>
                    </div>
                    
                    <button type="submit" class="btn">
                        <i class="fas fa-save"></i>
                        Save & Connect to WiFi
                    </button>
                </form>
                
                <div class="footer">
                    <p>
                        <i class="fas fa-microchip"></i>
                        ESP32 Web Portal v2.0
                    </p>
                </div>
            </div>
        </body>
        </html>
    )";
    
    /* Send the HTML content to the client with the Content-Type (text/html) and status code (200 OK). */
    server.send(200, "text/html", html);
}

/*================================================================================================*/
/**
* @brief        Handles HTTP POST requests from the configuration form ('/save').
* @details      This function extracts the submitted SSID and Password, saves them using the 
*               WifiManager, and serves a success page before initiating a device restart 
*               to attempt connection with the new credentials. 
*
* @param[in]    None (Data retrieved via server.arg())
* @param[out]   None (Side effect: Saves credentials and restarts device)
*
* @return       void
*
* @api
*/
/*================================================================================================*/
void WebPortal::handleSave() {
    /* Retrieve the submitted SSID from the POST request arguments. */
    String ssid = server.arg("ssid");
    /* Retrieve the submitted Password from the POST request arguments. */
    String pass = server.arg("pass");
    
    /* Call the external WifiManager function to save the new credentials (e.g., to EEPROM or SPIFFS). */
    wifi->saveCredentials(ssid, pass);
    
    /* Define the success HTML page content. */
    String html = R"(
        <!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>Settings Saved</title>
            <style>
                * {
                    margin: 0;
                    padding: 0;
                    box-sizing: border-box;
                    font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
                }
                
                body {
                    background: linear-gradient(135deg, #4CAF50 0%, #2E7D32 100%);
                    min-height: 100vh;
                    display: flex;
                    justify-content: center;
                    align-items: center;
                    padding: 20px;
                }
                
                .success-card {
                    background: white;
                    border-radius: 20px;
                    padding: 50px 40px;
                    text-align: center;
                    max-width: 500px;
                    width: 100%;
                    box-shadow: 0 20px 60px rgba(0, 0, 0, 0.2);
                }
                
                .success-icon {
                    font-size: 80px;
                    color: #4CAF50;
                    margin-bottom: 20px;
                }
                
                h2 {
                    color: #333;
                    margin-bottom: 15px;
                    font-size: 28px;
                }
                
                .message {
                    color: #666;
                    font-size: 16px;
                    line-height: 1.6;
                    margin-bottom: 25px;
                }
                
                .countdown {
                    font-size: 14px;
                    color: #888;
                    margin-top: 20px;
                }
            </style>
            <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css">
        </head>
        <body>
            <div class="success-card">
                <div class="success-icon">
                    <i class="fas fa-check-circle"></i>
                </div>
                
                <h2>Settings Saved Successfully!</h2>
                
                <div class="message">
                    WiFi credentials have been saved.<br>
                    Device is restarting...
                </div>
                
                <div class="countdown" id="countdown">
                    Restarting in 3 seconds...
                </div>
            </div>
            
            <script>
                let seconds = 3;
                const countdown = document.getElementById('countdown');
                
                const interval = setInterval(() => {
                    seconds--;
                    countdown.textContent = `Restarting in ${seconds} second${seconds !== 1 ? 's' : ''}...`;
                    
                    if (seconds <= 0) {
                        clearInterval(interval);
                    }
                }, 1000);
            </script>
        </body>
        </html>
    )";
    
    /* Send the success page to the client. The client will start a countdown. */
    server.send(200, "text/html", html);
    
    /* Wait briefly to allow the web server time to finish transmitting the response to the client. */
    delay(1500);
    
    /* Initiate a hardware reset/reboot. The device will restart and attempt to connect to the new Wi-Fi network. */
    ESP.restart();
}