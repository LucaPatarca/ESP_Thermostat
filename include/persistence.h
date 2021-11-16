#pragma once

// #define PERSISTENCE_DEBUG

class PersistenceClass{
    public:
        void savePowerState(bool);
        bool loadPowerState();

        void saveTargetTemperature(float temp);
        float loadTargetTemperature();

        PersistenceClass();
};

extern PersistenceClass Persistence;