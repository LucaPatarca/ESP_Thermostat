/*
 * Example for how to use Lock device as garage door opener
 * 
 * If you encounter any issues:
 * - check the readme.md at https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md
 * - ensure all dependent libraries are installed
 *   - see https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md#arduinoide
 *   - see https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md#dependencies
 * - open serial monitor and check whats happening
 * - check full user documentation at https://sinricpro.github.io/esp8266-esp32-sdk
 * - visit https://github.com/sinricpro/esp8266-esp32-sdk/issues and check for existing issues or open a new one
 */

// Uncomment the following line to enable serial debug output
// #define ENABLE_DEBUG

#include <Arduino.h>
#include <MainController.h>

#define BAUD_RATE 9600

MainController *controller;

void setup()
{
  Serial.begin(BAUD_RATE);
  Serial.printf("\r\n\r\n");

  controller = new MainController();
}

void loop()
{
  controller->handle();
  delay(50);
}