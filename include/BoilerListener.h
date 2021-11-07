#pragma once

class BoilerListener{
    public:
        virtual void onBoilerState(bool) = 0;
};