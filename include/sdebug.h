#define LOG_LEVEL_OFF 0
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_WARN 2
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL LOG_LEVEL_WARN

//TODO add fine

#if LOG_LEVEL > LOG_LEVEL_OFF
#define FLOG(severity, ...) Serial.printf("% 8.3fs %15s  [%s]  ", millis()/1000.0f, __FILE__, severity);\
    Serial.printf(__VA_ARGS__);\
    Serial.printf("\n")
#define LOG(severity, string) Serial.printf("% 8.3fs %15s  [%s]  ", millis()/1000.0f, __FILE__, severity);\
    Serial.print(string);\
    Serial.print("\n")
#else
#define LOG(severity, ...)
#define FLOG(severity, ...)
#endif

#if LOG_LEVEL > LOG_LEVEL_WARN
#define FINFO(...) FLOG("INFO", __VA_ARGS__)
#define INFO(string) LOG("INFO", string)
#else
#define FINFO(...)
#define INFO(...)
#endif
#if LOG_LEVEL > LOG_LEVEL_ERROR
#define FWARN(...) FLOG("WARN", __VA_ARGS__)
#define WARN(string) LOG("WARN", string)
#else
#define FWARN(...)
#define WARN(...)
#endif
#define FERROR(...) FLOG("ERROR", __VA_ARGS__)
#define ERROR(string) LOG("ERROR", string)