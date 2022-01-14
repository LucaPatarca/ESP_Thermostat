#include <Arduino.h>
#include <controller.h>

#define USE_PHISICAL_INPUT
#define BAUD_RATE 9600

MainController& controller = MainController::Instance();

void setup()
{
  Serial.begin(BAUD_RATE);
  controller.setup();
}

void loop()
{
  controller.handle();
  delay(100);
}