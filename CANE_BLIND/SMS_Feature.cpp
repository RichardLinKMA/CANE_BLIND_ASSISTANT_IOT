/******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include "SMS_Feature.h"

/*================================================================================================*/
/**
* @brief        Sends a text message via the GSM module.
* @details      Configures the GSM module in text mode, sends the recipient's phone number,
*               writes the message content, and finally sends the SMS by transmitting Ctrl+Z.
*               Waits appropriate delays between commands and reads the module's response.
*
* @param[in]    phoneNumber     The recipient's phone number as a String.
* @param[in]    messageText     The content of the SMS as a String.
* @param[out]   None
*
* @return       None
*
* @note         Uses gsmSerialPort as the GSM UART port and macros for delay/timeout values.
*/
/*================================================================================================*/
void sendTextMessage(String phoneNumber, String messageText) {
    /* Set GSM module to Text Mode for SMS */
    sendGsmCommand("AT+CMGF=1", GSM_COMMAND_WAIT_MS);                        

    /* Start sending SMS command with recipient's phone number */
    gsmSerialPort.print("AT+CMGS=\"");
    gsmSerialPort.print(phoneNumber);
    gsmSerialPort.println("\"");
    delay(GSM_SMS_COMMAND_DELAY_MS);

    /* Send the SMS content */
    gsmSerialPort.print(messageText);
    delay(GSM_SMS_TEXT_DELAY_MS);

    /* Send Ctrl+Z to finalize SMS sending */
    gsmSerialPort.write(26); 
    delay(GSM_SMS_SEND_TIMEOUT_MS);

    /* Read and optionally print the response from GSM module */
    readGsmResponse(GSM_SMS_SEND_TIMEOUT_MS);

   /* Print confirmation to Serial Monitor */
    Serial.println("SMS send complete!");
}

