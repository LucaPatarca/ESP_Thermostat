#pragma once

#define LOG_LEVEL_OFF 0
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_WARN 2
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL_FINE 4
#define LOG_LEVEL LOG_LEVEL_INFO

#if LOG_LEVEL > LOG_LEVEL_OFF
#define LOG(severity, ...) Serial.printf("% 9.3fs [%s]  %15s  : ", millis()/1000.0f, severity, __FILE__);\
    Serial.printf(__VA_ARGS__);\
    Serial.print("\n")
#else
#define LOG(severity, ...)
#endif

#if LOG_LEVEL > LOG_LEVEL_INFO
#define FINE(...) LOG("FINE", __VA_ARGS__)
#else
#define FINE(...)
#endif
#if LOG_LEVEL > LOG_LEVEL_WARN
#define INFO(...) LOG("INFO", __VA_ARGS__)
#else
#define INFO(...)
#endif
#if LOG_LEVEL > LOG_LEVEL_ERROR
#define WARN(...) LOG("WARN", __VA_ARGS__)
#else
#define WARN(...)
#endif
#define ERROR(...) LOG("ERROR", __VA_ARGS__)
