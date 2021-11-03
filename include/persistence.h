#pragma once

#ifdef ENABLE_DEBUG
#define PERSISTENCE_DEBUG
#endif

class PersistenceClass{
    public:
        void savePowerState(bool);
        bool loadPowerState();

        void saveTargetTemperature(float temp);
        float loadTargetTemperature();

        PersistenceClass();
};

extern PersistenceClass Persistence;