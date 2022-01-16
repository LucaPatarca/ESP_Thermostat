#ifdef ENABLE_LOG

#include <logger.h>

void Logger::_sendLog()
{
    _http.begin(_client, "http://192.168.5.99:9000/log/");
    _http.addHeader("Content-Type", "application/json");
    char json[512];
    String s[] = {"D", "R", "S"};
    sprintf(json, "{\"temp\":%.2f,\"humidity\":%.2f,\"trend\":\"%s\",\"coeff\":%.4f,\"power\":%s,\"target\":%.2f,\"boiler\":%s}",
            _temp.temp,
            _temp.humidity,
            s[_temp.trend].c_str(),
            _temp.coefficient,
            _power ? "true" : "false",
            _target,
            _boiler ? "true" : "false");
#ifdef LOGGER_DEBUG
    Serial.printf("[Logger] sending: \n%s\n", json);
#endif
    _http.POST(json);
}

void Logger::onBoilerState(bool state)
{
    _boiler = state;
}

void Logger::onCurrentTemperature(Temperature temp)
{
    _temp = temp;
    _sendLog();
}

void Logger::onPowerState(bool state)
{
    _power = state;
}

void Logger::onTargetTemperature(float temp)
{
    _target = temp;
}

void Logger::onThermostatMode(Mode mode){
    
}

void Logger::onSetSetting(String key, String value){
    //nop
}

#endif