#ifndef SMS_FEATURE_H
#define SMS_FEATURE_H

/******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include "Generic_API.h"

/******************************************************************************
 * MACROS
 ******************************************************************************/
/* Define default delays for SMS sending (in milliseconds) */
#define GSM_SMS_COMMAND_DELAY_MS 500
#define GSM_SMS_TEXT_DELAY_MS 300
#define GSM_SMS_SEND_TIMEOUT_MS 3000

/******************************************************************************
 * FUNCTIONS PROTOTYPES
 ******************************************************************************/
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
void sendTextMessage(String phoneNumber, String messageText);

#endif /* SMS_FEATURE_H */