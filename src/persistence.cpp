#include "persistence.h"
#include <Arduino.h>
#include <ESP_EEPROM.h>

void PersistenceClass::savePowerState(bool powerState){
    EEPROM.put(0, powerState);
    if(!EEPROM.commit()){
        Serial.printf("Errore di scrittura!\n");
    }
}

bool PersistenceClass::loadPowerState(){
    bool result;
    return EEPROM.get(0, result);
}

PersistenceClass::PersistenceClass(){
    EEPROM.begin(16);
}

void PersistenceClass::saveTargetTemperature(float temp){
    EEPROM.put(1, temp);
    if(!EEPROM.commit()){
        Serial.printf("Errore di scrittura!\n");
    }
}

float PersistenceClass::loadTargetTemperature(){
    float result;
    return EEPROM.get(1, result);
}

PersistenceClass Persistence;