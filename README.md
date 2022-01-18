# ESP_Thermostat

ESP_Thermostat is a full-featured Smart Thermostat created using an ESP8266 board and other common parts.

## Main Features

- Smart Home integration
- Week based schedule
- Controlled from everywhere
- Works offline
- OTA firmware updates

#### Smart Home Integration

ESP_Thermostat is compatible with most smart assistants like Alexa and Google Home.

#### Schedule

It can be programmed with a schedule where you can specify a different temperature for every half an hour of every day in a week.

#### Controlled From Everywhere

You can control the thermostat directly from Alexa and Google Home, however, both apps do not support the thermostat schedule feature. To fix this I've made [an app](https://github.com/LucaPatarca/ESPThermostatApp) specifically to support this feature on ESP_Thermostat.

#### It Works Offline

ESP_Thermostat is designed to operate offline with a display and 3 physical buttons, one for thermostat mode and two for temperature up and down. The only thing that cannot be changed without the internet is the schedule (but that's going to change soon).

#### OTA Updates

The firmware can be upgraded over the local network, meaning that it doesn't have to be unmounted from the wall to flash a new firmware.

## Assembling

Parts needed to assemble it:

- ESP8266 development board (NodeMCU, WemosD1, etc..)
- DHT22 temperature and humidity sensor
- A relay appropriate for your board 
- SSD1306 128x64 0.96" Display (**optional**)
- 3 push buttons (**optional**)

## Firmware

The easiest way to build and flash the firmware is probably to use VSCode and the PlatformIO extension. If you don't have all the optional parts assembled, before building the firmware, you need to [add a few build flags](https://docs.platformio.org/en/latest/projectconf/section_env_build.html#id2):  
- `-D NO_DISPLAY` if you don't have a display
- `-D NO_BUTTON_INPUT` if you don't have the 3 buttons

## Current State

The project is mostly working and will be ready to be used soon.

#### Major Problems

Currently, it only works on one type of boiler, which is the type that has a single contact to control it.
