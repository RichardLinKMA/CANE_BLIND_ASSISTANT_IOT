/******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include "Generic_API.h"

/******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************/
/* Global system timestamp (milliseconds) */
unsigned long systemCurrentTimeMs =0U;

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

void sendGsmCommand(String atCommand, uint16_t waitMs) {
    /* Send the AT command to the GSM module */
    gsmSerialPort.println(atCommand);

    /* Wait for the module to process the command */
    delay(waitMs);

    /* Read and optionally print the response from the GSM module */
    readGsmResponse(GSM_RESPONSE_TIMEOUT_MS);
}

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
String readGsmResponse(uint16_t timeoutMs) {
    /* Record the starting time in milliseconds */
    uint32_t startTime = millis(); 

    /* String buffer to accumulate the received response */
    String response = "";

    /* Loop until the elapsed time reaches the timeout */
    while (millis() - startTime < timeoutMs) {
        
        /* Check if the GSM module has data available */
        while (gsmSerialPort.available()) {
            
            /* Read one character from the GSM module */
            char receivedChar = gsmSerialPort.read(); 
            
            /* Append the received character to the response buffer */
            response += receivedChar;

            /* Optionally print the received character to the debug Serial monitor */
            if (1) {
                Serial.print(receivedChar);
            }
        }
    }

    /* Return the accumulated response */
    return response;
}

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
void beepBuzzer(int repeatCount) {
    for (int i = 0; i < repeatCount; i++) {
        /* Turn the buzzer on */
        digitalWrite(BUZZER_PIN, HIGH);

        /* Wait for 200 milliseconds */
        delay(200);

        /* Turn the buzzer off */
        digitalWrite(BUZZER_PIN, LOW);

        /* Wait for another 200 milliseconds */
        delay(200);
    }
}