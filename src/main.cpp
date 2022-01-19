#include <Arduino.h>
#include <controller.h>
#include <sdebug.h>

#define USE_PHISICAL_INPUT
#define BAUD_RATE 9600

//TODO add documentation

MainController& controller = MainController::Instance();

void setup()
{
  Serial.begin(BAUD_RATE);
  Serial.print("\n\r");
  INFO("setup started");
  controller.setup();
  INFO("setup completed");
}

void loop()
{
  controller.handle();
  delay(50);
}