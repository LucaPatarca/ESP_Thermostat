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
#include <wifi.h>
#include <BoilerController.h>
#include <hwio.h>
#include <alexa.h>
#include <ota.h>
#include <temperature.h>
#include <program.h>

#define BAUD_RATE 9600
// #define ENABLE_LOG

#ifdef ENABLE_LOG
#include <logger.h>
#endif

BoilerController *thermostat;
HWIOController *hwio;
AlexaController *alexa;
OTAController *ota;
TemperatureController *temperature;
WifiController *wifi;
ProgramController *program;

#ifdef ENABLE_LOG
  Logger *logger;
#endif

void setup()
{
  Serial.begin(BAUD_RATE);
  Serial.printf("\r\n\r\n");

  hwio = new HWIOController();
  thermostat = new BoilerController();
  temperature = new TemperatureController();
  wifi = new WifiController();
  alexa = new AlexaController();
  ota = new OTAController();
  program = new ProgramController();

  wifi->addListener(hwio);
  wifi->addListener(alexa);
  wifi->addListener(ota);
  wifi->connect();

  thermostat->addListener(hwio);

  temperature->addListener(alexa);
  temperature->addListener(hwio);
  temperature->addListener(thermostat);

  alexa->addListener(hwio);
  alexa->addListener(thermostat);
  alexa->addListener(program);

  ota->addListener(alexa);
  ota->addListener(hwio);

  program->addListener(hwio);
  program->addListener(thermostat);
  program->addListener(alexa);

#ifdef ENABLE_LOG
  logger = new Logger();
  thermostat->addListener(logger);
  alexa->addListener(logger);
  temperature->addListener(logger);
#endif

  hwio->init();
}

void loop()
{
  alexa->handle();
  ota->handle();
  temperature->handle();
  program->handle();
  hwio->handle();
  delay(100);
}