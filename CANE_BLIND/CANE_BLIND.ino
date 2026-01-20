/*==================================================================================================
*                          INCLUDES
==================================================================================================*/
#include <Arduino.h>
#include "GPS_Feature.h"
#include "CALL_SOS_Feature.h"
#include "SMS_Feature.h"
/*==================================================================================================
*                          GLOBAL VARIABLES
==================================================================================================*/
/* Hardware serial instance for communication with GSM/GNSS module */
HardwareSerial gsmSerialPort(1);

/* Global variable to track WiFi connection status */
bool wifiConnectedStatus = false;
String connectedSSID = "";

/****************************************************************************************
 *  Function Name    : setup
 *  Description      : Arduino system initialization function. It is executed once at 
 *                     startup and performs initialization of serial ports, GSM/GNSS 
 *                     hardware control pins, and power-on sequence. The function also 
 *                     sends initial AT commands to verify communication with the GSM/GNSS 
 *                     module and starts the GNSS (GPS) subsystem for location testing.
 *
 *  Input Parameters :
 *    None
 *
 *  Return Value     :
 *    None
 ****************************************************************************************/
void setup() {

  /* Initialize the debug serial port at baud rate 115200 */
  Serial.begin(115200);

  Wire.begin(21, 22); /* Init I2C with SDA=21, SCL=22 */

  /* Configure the button pin as input with an internal  pull-up resistor */
  pinMode(BUTTON_PIN, INPUT);

  /* Configure the buzzer pin as output and set its initial state to LOW */
  pinMode(BUZZER_PIN, OUTPUT); /* Set buzzer pin as output */

  digitalWrite(BUZZER_PIN, LOW); /* Turn off buzzer initially */

  /* Beep the buzzer once to indicate the start of setup process */
  beepBuzzer(2);

  /* Print initial startup message on debug serial */
  Serial.print(F("Hello! AT command Start Init"));


  /* Initialize the GSM/GNSS serial port (UART2) with baud rate 115200, 
       8 data bits, no parity, 1 stop bit, and assigned RX/TX pins */
  gsmSerialPort.begin(115200, SERIAL_8N1, PIN_GSM_UART_RECEIVE, PIN_GSM_UART_TRANSMIT);

  /* Test communication with GSM module */
  sendGsmCommand("AT", GSM_COMMAND_WAIT_MS);

  /* Disable command echo */
  sendGsmCommand("ATE0", GSM_COMMAND_WAIT_MS);

  /* Set character set to GSM */
  sendGsmCommand("AT+CSCS=\"GSM\"", GSM_COMMAND_WAIT_MS);

  /* Set SMS mode to text */
  sendGsmCommand("AT+CMGF=1", GSM_COMMAND_WAIT_MS);

  /* Configure new message indications: display directly on Serial */
  sendGsmCommand("AT+CNMI=2,2,0,0,0", GSM_COMMAND_WAIT_MS);

  /* Enable Caller Line Identification Presentation (CLIP) */
  sendGsmCommand("AT+CLIP=1", GSM_COMMAND_WAIT_MS);

  /* Test signal quality */
  sendGsmCommand("AT+CSQ", GSM_COMMAND_WAIT_MS);

  /* Print message indicating the start of LTE CAT1 test */
  Serial.println("ESP32-S3 4G LTE CAT1 complete init!");
  /* Get the number of milliseconds since the program started and store it in systemCurrentTimeMs */
  systemCurrentTimeMs = millis();

  /* Beep the buzzer twice to indicate system initialization is complete */
  beepBuzzer(10);
}

/****************************************************************************************
 *  Function Name    : loop
 *  Description      : Arduino main execution loop. This function runs continuously after 
 *                     system initialization. It periodically requests GNSS (GPS) information 
 *                     from the GSM/GNSS module every 30 seconds and generates a Google Maps 
 *                     URL from the response. In addition, it forwards data between the debug 
 *                     Serial (USB) and the GSM serial port, enabling transparent communication 
 *                     for debugging and testing.
 *
 *  Input Parameters :
 *    None
 *
 *  Return Value     :
 *    None
 ****************************************************************************************/
void loop() {

  /* Call the GPS request handler function */
  requestGpsLocation(systemCurrentTimeMs, 30000UL, DEBUG_MODE_ENABLED);

 handleCallAndATPassthrough(BUTTON_PIN, DEBUG_MODE_ENABLED, 
                               wifiConnectedStatus, connectedSSID.c_str());
    
    delay(175);
}