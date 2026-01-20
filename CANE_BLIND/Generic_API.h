#ifndef GENERIC_API_H
#define GENERIC_API_H

/******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>


/******************************************************************************
 * MACROS
 ******************************************************************************/
/* Pin mapping for GSM/GNSS module (UART and control pins) */
#define PIN_GSM_UART_RECEIVE           33  /* UART RX pin for GSM/GNSS module */
#define PIN_GSM_UART_TRANSMIT          25  /* UART TX pin for GSM/GNSS module */

/* Debug mode enable flag - Set to true to enable debug prints */
#define DEBUG_MODE_ENABLED   true

/* Pin mapping for the buzzer */
#define BUZZER_PIN           26

/* SOS phone number in international format */
#define SOS_PHONE_NUMBER   "0387695355"

/* Define a default timeout value for reading GSM response (in milliseconds) */
#define GSM_RESPONSE_TIMEOUT_MS 1000

/* Define default delay after sending AT command (in milliseconds) */
#define GSM_COMMAND_WAIT_MS 800

/* Task stack size (bytes) */
#define TASK_STACK_SIZE 2048

#define TASK_STACK_SIZE_MAX30100 2048

/* Default Wi-Fi Network Name (SSID) */
#define DEFAULT_SSID   "P5B"
/* Default Password for the Wi-Fi Network */
#define DEFAULT_PASS   "Nambkav123"

/* Network Name (SSID) for the ESP32 Access Point mode */
#define AP_SSID        "ESP32-Setup"
/* Password for the Access Point mode */
#define AP_PASS        "12345678"

/* Maximum number of times to try connecting to Wi-Fi */
#define MAX_TRIES      5
/* Delay (in milliseconds) between connection attempts */
#define CONNECT_DELAY  2000


/* I2C Clock */
#define I2C_CLOCK 100000
/******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************/
/* Externally defined HardwareSerial instance for GSM/GNSS communication */
extern HardwareSerial gsmSerialPort;

/******************************************************************************
 * FUNCTIONS PROTOTYPES
 ******************************************************************************/
/*================================================================================================*/
/**
* @brief        Sends an AT command to the GSM module and triggers response reading.
* @details      Transmits the specified AT command to the GSM module through the GSM UART port,
*               waits a configurable delay to allow the module to process the command, and then
*               invokes readGsmResponse() to retrieve the response from the module.
*               The function does not return the response directly; instead, response handling
*               is deferred to the readGsmResponse() implementation.
*
* @param[in]    atCommand       The AT command string to send to the GSM module.
* @param[in]    waitMs          Delay time (in milliseconds) after sending the command before
*                               attempting to read the response.
*
* @return       void
*
* @note         Uses the GSM UART interface referenced as gsmSerialPort.
*/
/*================================================================================================*/

void sendGsmCommand(String atCommand, uint16_t waitMs);

/*================================================================================================*/
/**
* @brief        Reads the response from the GSM module over the serial port.
* @details      Continuously reads incoming characters from the GSM module via the 
*               specified UART port until the specified timeout expires. All received 
*               characters are accumulated in a String and optionally printed to the 
*               debug Serial monitor if enabled.
*
* @param[in]    timeoutMs       Maximum waiting time in milliseconds for the response.
* @param[in]    enableDebug     If true, prints each received character to the Serial monitor.
*
* @return       String          The full response collected from the GSM module.
*
* @note         Non-blocking inside timeout window; relies on gsmSerialPort.available().
*/
/*================================================================================================*/
String readGsmResponse(uint16_t timeoutMs);

/*================================================================================================*/
/**
* @brief        Activates the buzzer for a specified number of "beep" sounds.
* @details      Turns the buzzer on, waits for a specified duration, then turns it off,
*               and waits again to create a distinguishable "beep" sound pattern.
*               Repeats this process for the given number of times.
*
* @param[in]    repeatCount    The number of times the "beep" sound should be repeated.
* @param[out]   None
*
* @return       None
*
* @api
*/
/*================================================================================================*/
void beepBuzzer(int repeatCount);
#endif