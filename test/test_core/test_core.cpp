#include <Arduino.h>
#include <state.h>
#include <boiler.h>
#include <unity.h>

namespace StateTest
{
    class TestClass : StateListener
    {
    public:
        TestClass() { state.addListener(this); }

        State &state = State::Instance();

        bool targetChanged = false;
        bool powerChanged = false;
        bool modeChanged = false;
        bool boilerChanged = false;
        bool temperatureChanged = false;
        bool wifiChanged = false;

        void targetTemperatureChanged(Cause cause) { targetChanged = true; }
        void powerStateChanged(Cause cause) { powerChanged = true; }
        void thermostatModeChanged(Cause cause) { modeChanged = true; }
        void boilerStateChanged() { boilerChanged = true; }
        void currentTemperatureChanged() { temperatureChanged = true; }
        void wifiStatusChanged() { wifiChanged = true; }

        void reset()
        {
            targetChanged = false;
            powerChanged = false;
            modeChanged = false;
            boilerChanged = false;
            temperatureChanged = false;
            wifiChanged = false;
        }
    };

    TestClass *test;

    void stateCallsTargetTemperatureCallback()
    {
        test->reset();
        test->state.reset();
        test->state.setTargetTemperature(Cause::MANUAL, 20.0);
        TEST_ASSERT_TRUE(test->targetChanged);
        TEST_ASSERT_TRUE(test->modeChanged);
        TEST_ASSERT_TRUE(test->powerChanged);
        TEST_ASSERT_EQUAL_FLOAT(20.0, test->state.getTargetTemperature());
        test->reset();
        test->state.setTargetTemperature(Cause::ALEXA, 20.0);
        TEST_ASSERT_FALSE(test->targetChanged);
        TEST_ASSERT_FALSE(test->modeChanged);
        TEST_ASSERT_FALSE(test->powerChanged);
        TEST_ASSERT_EQUAL_FLOAT(20.0, test->state.getTargetTemperature());
        test->reset();
        test->state.setTargetTemperature(Cause::ALEXA, 20.1);
        TEST_ASSERT_TRUE(test->targetChanged);
        TEST_ASSERT_FALSE(test->modeChanged);
        TEST_ASSERT_FALSE(test->powerChanged);
        TEST_ASSERT_EQUAL_FLOAT(20.1, test->state.getTargetTemperature());
    }

    void stateCallsPowerStateCallback()
    {
        test->reset();
        test->state.reset();
        test->state.setPowerState(Cause::ALEXA, true);
        TEST_ASSERT_TRUE(test->powerChanged);
        TEST_ASSERT_TRUE(test->modeChanged);
        TEST_ASSERT_EQUAL(Mode::ON, test->state.getThermostatMode());
        TEST_ASSERT_EQUAL(true, test->state.getPowerState());
        test->reset();
        test->state.setPowerState(Cause::ALEXA, true);
        TEST_ASSERT_FALSE(test->modeChanged);
        TEST_ASSERT_FALSE(test->powerChanged);
        TEST_ASSERT_EQUAL(Mode::ON, test->state.getThermostatMode());
        TEST_ASSERT_EQUAL(true, test->state.getPowerState());
        test->reset();
        test->state.setPowerState(Cause::ALEXA, false);
        TEST_ASSERT_TRUE(test->powerChanged);
        TEST_ASSERT_TRUE(test->modeChanged);
        TEST_ASSERT_EQUAL(Mode::OFF, test->state.getThermostatMode());
        TEST_ASSERT_EQUAL(false, test->state.getPowerState());
        test->state.setThermostatMode(Cause::MANUAL, Mode::PROGRAM);
        test->reset();
        test->state.setPowerState(Cause::SCHEDULE, true);
        TEST_ASSERT_TRUE(test->powerChanged);
        TEST_ASSERT_FALSE(test->modeChanged);
        TEST_ASSERT_EQUAL(Mode::PROGRAM, test->state.getThermostatMode());
        TEST_ASSERT_EQUAL(true, test->state.getPowerState());
    }

    void stateCallsBoilerStateCallback()
    {
        test->reset();
        test->state.reset();
        test->state.setBoilerState(true);
        TEST_ASSERT_TRUE(test->boilerChanged);
        TEST_ASSERT_EQUAL(true, test->state.getBoilerState());
        test->reset();
        test->state.setBoilerState(true);
        TEST_ASSERT_FALSE(test->boilerChanged);
        TEST_ASSERT_EQUAL(true, test->state.getBoilerState());
        test->reset();
        test->state.setBoilerState(false);
        TEST_ASSERT_TRUE(test->boilerChanged);
        TEST_ASSERT_EQUAL(false, test->state.getBoilerState());
        test->reset();
    }

    void stateCallsThermostatModeCallback()
    {
        test->reset();
        test->state.reset();
        test->state.setThermostatMode(Cause::MANUAL, Mode::ON);
        TEST_ASSERT_TRUE(test->modeChanged);
        TEST_ASSERT_TRUE(test->powerChanged);
        TEST_ASSERT_EQUAL(Mode::ON, test->state.getThermostatMode());
        TEST_ASSERT_EQUAL(true, test->state.getPowerState());
        test->reset();
        test->state.setThermostatMode(Cause::MANUAL, Mode::ON);
        TEST_ASSERT_FALSE(test->modeChanged);
        TEST_ASSERT_FALSE(test->powerChanged);
        TEST_ASSERT_EQUAL(Mode::ON, test->state.getThermostatMode());
        TEST_ASSERT_EQUAL(true, test->state.getPowerState());
        test->reset();
        test->state.setThermostatMode(Cause::MANUAL, Mode::OFF);
        TEST_ASSERT_TRUE(test->modeChanged);
        TEST_ASSERT_TRUE(test->powerChanged);
        TEST_ASSERT_EQUAL(Mode::OFF, test->state.getThermostatMode());
        TEST_ASSERT_EQUAL(false, test->state.getPowerState());
        test->reset();
        test->state.setThermostatMode(Cause::MANUAL, Mode::PROGRAM);
        TEST_ASSERT_TRUE(test->modeChanged);
        TEST_ASSERT_FALSE(test->powerChanged);
        TEST_ASSERT_EQUAL(Mode::PROGRAM, test->state.getThermostatMode());
        TEST_ASSERT_EQUAL(false, test->state.getPowerState());
    }

    void stateCallsCurrentTemperatureCallback()
    {
        test->reset();
        test->state.reset();
        Temperature_t t = Temperature_t{20.23, 55.63, TemperatureTrend::STABLE, 0.0};
        test->state.setCurrentTemperature(t);
        TEST_ASSERT_TRUE(test->temperatureChanged);
        TEST_ASSERT_TRUE(t == test->state.getCurrentTemperature());
        test->reset();
        t = Temperature_t{20.25, 55.65, TemperatureTrend::STABLE, 0.001};
        test->state.setCurrentTemperature(t);
        TEST_ASSERT_FALSE(test->temperatureChanged);
        TEST_ASSERT_TRUE(t == test->state.getCurrentTemperature());
        test->reset();
        t = Temperature_t{20.21, 55.61, TemperatureTrend::STABLE, 0.001};
        test->state.setCurrentTemperature(t);
        TEST_ASSERT_FALSE(test->temperatureChanged);
        TEST_ASSERT_TRUE(t == test->state.getCurrentTemperature());
        test->reset();
        t = Temperature_t{20.10, 55.65, TemperatureTrend::STABLE, 0.001};
        test->state.setCurrentTemperature(t);
        TEST_ASSERT_TRUE(test->temperatureChanged);
        TEST_ASSERT_TRUE(t == test->state.getCurrentTemperature());
        test->reset();
        t = Temperature_t{20.10, 55.10, TemperatureTrend::STABLE, 0.001};
        test->state.setCurrentTemperature(t);
        TEST_ASSERT_TRUE(test->temperatureChanged);
        TEST_ASSERT_TRUE(t == test->state.getCurrentTemperature());
        test->reset();
        t = Temperature_t{20.10, 55.50, TemperatureTrend::STABLE, 0.08};
        test->state.setCurrentTemperature(t);
        TEST_ASSERT_TRUE(test->temperatureChanged);
        TEST_ASSERT_TRUE(t == test->state.getCurrentTemperature());
        test->reset();
        t = Temperature_t{20.22, 55.50, TemperatureTrend::STABLE, 0.08};
        test->state.setCurrentTemperature(t);
        TEST_ASSERT_TRUE(test->temperatureChanged);
        TEST_ASSERT_TRUE(t == test->state.getCurrentTemperature());
        test->reset();
        t = Temperature_t{20.22, 56.02, TemperatureTrend::STABLE, 0.08};
        test->state.setCurrentTemperature(t);
        TEST_ASSERT_TRUE(test->temperatureChanged);
        TEST_ASSERT_TRUE(t == test->state.getCurrentTemperature());
        test->reset();
        t = Temperature_t{20.22, 55.62, TemperatureTrend::STABLE, 0.00};
        test->state.setCurrentTemperature(t);
        TEST_ASSERT_TRUE(test->temperatureChanged);
        TEST_ASSERT_TRUE(t == test->state.getCurrentTemperature());
        test->reset();
        t = Temperature_t{20.22, 55.62, TemperatureTrend::DROP, 0.00};
        test->state.setCurrentTemperature(t);
        TEST_ASSERT_TRUE(test->temperatureChanged);
        TEST_ASSERT_TRUE(t == test->state.getCurrentTemperature());
        test->reset();
        t = Temperature_t{20.22, 55.62, TemperatureTrend::RISE, 0.00};
        test->state.setCurrentTemperature(t);
        TEST_ASSERT_TRUE(test->temperatureChanged);
        TEST_ASSERT_TRUE(t == test->state.getCurrentTemperature());
    }

    void stateCallsWifiStatusCallback()
    {
        test->reset();
        test->state.reset();
        test->state.setwWifiStatus(WiFiStatus::CONNECTING);
        TEST_ASSERT_TRUE(test->wifiChanged);
        TEST_ASSERT_EQUAL(WiFiStatus::CONNECTING, test->state.getWifiStatus());
        test->reset();
        test->state.setwWifiStatus(WiFiStatus::CONNECTING);
        TEST_ASSERT_FALSE(test->wifiChanged);
        TEST_ASSERT_EQUAL(WiFiStatus::CONNECTING, test->state.getWifiStatus());
        test->reset();
        test->state.setwWifiStatus(WiFiStatus::CONNECTED);
        TEST_ASSERT_TRUE(test->wifiChanged);
        TEST_ASSERT_EQUAL(WiFiStatus::CONNECTED, test->state.getWifiStatus());
        test->reset();
        test->state.setwWifiStatus(WiFiStatus::DISCONNECTED);
        TEST_ASSERT_TRUE(test->wifiChanged);
        TEST_ASSERT_EQUAL(WiFiStatus::DISCONNECTED, test->state.getWifiStatus());
    }
}

namespace BoilerTest
{
    class TestClass : StateListener
    {
    public:
        TestClass() { state.addListener(this); }

        State &state = State::Instance();
        BoilerController &boiler = BoilerController::Instance();

        void targetTemperatureChanged(Cause cause) { boiler.targetTemperatureChanged(); }
        void powerStateChanged(Cause cause) { boiler.powerStateChanged(); }
        void thermostatModeChanged(Cause cause) {}
        void boilerStateChanged() {}
        void currentTemperatureChanged() { boiler.currentTemperatureChanged(); }
        void wifiStatusChanged() {}
    };

    TestClass *test;

    void testPowerState()
    {
        test->state.reset();
        TEST_ASSERT_FALSE(test->state.getBoilerState());
        test->state.setCurrentTemperature(Temperature_t{20.0, 60.0, TemperatureTrend::STABLE, 0});
        test->state.setTargetTemperature(Cause::MANUAL, 21.0);
        test->state.setPowerState(Cause::ALEXA, true);
        TEST_ASSERT_TRUE(test->state.getBoilerState());
        test->state.setPowerState(Cause::ALEXA, false);
        TEST_ASSERT_FALSE(test->state.getBoilerState());

        test->state.setCurrentTemperature(Temperature_t{15.5, 60.0, TemperatureTrend::STABLE, 0});
        TEST_ASSERT_FALSE(test->state.getBoilerState());
        test->state.setCurrentTemperature(Temperature_t{14.9, 60.0, TemperatureTrend::STABLE, 0});
        TEST_ASSERT_TRUE(test->state.getBoilerState());
        test->state.setCurrentTemperature(Temperature_t{15.5, 60.0, TemperatureTrend::STABLE, 0});
        TEST_ASSERT_TRUE(test->state.getBoilerState());
        test->state.setCurrentTemperature(Temperature_t{16.5, 60.0, TemperatureTrend::STABLE, 0});
        TEST_ASSERT_FALSE(test->state.getBoilerState());
    }

    void testTargetTemperature(){
        test->state.reset();
        TEST_ASSERT_FALSE(test->state.getBoilerState());
        test->state.setCurrentTemperature(Temperature_t{20.0, 60.0, TemperatureTrend::STABLE, 0});
        test->state.setPowerState(Cause::ALEXA, true);
        test->state.setTargetTemperature(Cause::MANUAL, 21.0);
        TEST_ASSERT_TRUE(test->state.getBoilerState());
        test->state.setTargetTemperature(Cause::MANUAL, 19.1);
        TEST_ASSERT_FALSE(test->state.getBoilerState());
        test->state.setTargetTemperature(Cause::MANUAL, 20.5);
        TEST_ASSERT_TRUE(test->state.getBoilerState());
    }

    void testTemperatureVariation(){
        test->state.reset();
        TEST_ASSERT_FALSE(test->state.getBoilerState());
        test->state.setPowerState(Cause::ALEXA, true);
        test->state.setTargetTemperature(Cause::MANUAL, 21.0);

        test->state.setCurrentTemperature(Temperature_t{19.5, 60.0, TemperatureTrend::RISE, 0});
        TEST_ASSERT_TRUE(test->state.getBoilerState());
        test->state.setCurrentTemperature(Temperature_t{19.5, 60.0, TemperatureTrend::STABLE, 0});
        TEST_ASSERT_TRUE(test->state.getBoilerState());
        test->state.setCurrentTemperature(Temperature_t{19.5, 60.0, TemperatureTrend::DROP, 0});
        TEST_ASSERT_TRUE(test->state.getBoilerState());

        test->state.setCurrentTemperature(Temperature_t{20.5, 60.0, TemperatureTrend::RISE, 0});
        TEST_ASSERT_FALSE(test->state.getBoilerState());
        test->state.setCurrentTemperature(Temperature_t{20.5, 60.0, TemperatureTrend::STABLE, 0});
        TEST_ASSERT_TRUE(test->state.getBoilerState());
        test->state.setCurrentTemperature(Temperature_t{20.5, 60.0, TemperatureTrend::DROP, 0});
        TEST_ASSERT_TRUE(test->state.getBoilerState());

        test->state.setCurrentTemperature(Temperature_t{20.9, 60.0, TemperatureTrend::RISE, 0});
        TEST_ASSERT_FALSE(test->state.getBoilerState());
        test->state.setCurrentTemperature(Temperature_t{20.9, 60.0, TemperatureTrend::STABLE, 0});
        TEST_ASSERT_FALSE(test->state.getBoilerState());
        test->state.setCurrentTemperature(Temperature_t{20.9, 60.0, TemperatureTrend::DROP, 0});
        TEST_ASSERT_TRUE(test->state.getBoilerState());

        test->state.setCurrentTemperature(Temperature_t{21.2, 60.0, TemperatureTrend::RISE, 0});
        TEST_ASSERT_FALSE(test->state.getBoilerState());
        test->state.setCurrentTemperature(Temperature_t{21.2, 60.0, TemperatureTrend::STABLE, 0});
        TEST_ASSERT_FALSE(test->state.getBoilerState());
        test->state.setCurrentTemperature(Temperature_t{21.2, 60.0, TemperatureTrend::DROP, 0});
        TEST_ASSERT_FALSE(test->state.getBoilerState());

        test->state.setCurrentTemperature(Temperature_t{20.9, 60.0, TemperatureTrend::RISE, 0});
        TEST_ASSERT_FALSE(test->state.getBoilerState());
        test->state.setCurrentTemperature(Temperature_t{20.9, 60.0, TemperatureTrend::STABLE, 0});
        TEST_ASSERT_FALSE(test->state.getBoilerState());
        test->state.setCurrentTemperature(Temperature_t{20.9, 60.0, TemperatureTrend::DROP, 0});
        TEST_ASSERT_TRUE(test->state.getBoilerState());

        test->state.setCurrentTemperature(Temperature_t{20.5, 60.0, TemperatureTrend::RISE, 0});
        TEST_ASSERT_FALSE(test->state.getBoilerState());
        test->state.setCurrentTemperature(Temperature_t{20.5, 60.0, TemperatureTrend::STABLE, 0});
        TEST_ASSERT_TRUE(test->state.getBoilerState());
        test->state.setCurrentTemperature(Temperature_t{20.5, 60.0, TemperatureTrend::DROP, 0});
        TEST_ASSERT_TRUE(test->state.getBoilerState());

        test->state.setCurrentTemperature(Temperature_t{19.5, 60.0, TemperatureTrend::RISE, 0});
        TEST_ASSERT_TRUE(test->state.getBoilerState());
        test->state.setCurrentTemperature(Temperature_t{19.5, 60.0, TemperatureTrend::STABLE, 0});
        TEST_ASSERT_TRUE(test->state.getBoilerState());
        test->state.setCurrentTemperature(Temperature_t{19.5, 60.0, TemperatureTrend::DROP, 0});
        TEST_ASSERT_TRUE(test->state.getBoilerState());
    }
}

void setup()
{
    delay(2000);

    UNITY_BEGIN();

    StateTest::test = new StateTest::TestClass();
    RUN_TEST(StateTest::stateCallsTargetTemperatureCallback);
    RUN_TEST(StateTest::stateCallsPowerStateCallback);
    RUN_TEST(StateTest::stateCallsBoilerStateCallback);
    RUN_TEST(StateTest::stateCallsThermostatModeCallback);
    RUN_TEST(StateTest::stateCallsCurrentTemperatureCallback);
    RUN_TEST(StateTest::stateCallsWifiStatusCallback);
    delete StateTest::test;

    BoilerTest::test = new BoilerTest::TestClass();
    RUN_TEST(BoilerTest::testPowerState);
    RUN_TEST(BoilerTest::testTargetTemperature);
    RUN_TEST(BoilerTest::testTemperatureVariation);
    delete BoilerTest::test;

    UNITY_END();

    pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
}