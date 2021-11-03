#pragma once

#include "SinricProNamespace.h"
#include <avr/pgmspace.h>
namespace SINRICPRO_NAMESPACE {

/**
 * @brief FSTR(x, y) Helper macro to define const char* by a given structure
 * Example:
 *   FSTR(SINRICPRO, test);
 * results in 
 *   const char* FSTR_SINRICPRO_test PROGMEM = "test";
 * 
 */
#define FSTR(x, y) const char* FSTR_ ##x ##_ ##y PROGMEM = #y
#define EXTERN_FSTR(x, y) extern const char* FSTR_##x ##_ ##y 

EXTERN_FSTR(SINRICPRO, instanceId);              // "instanceId"
EXTERN_FSTR(SINRICPRO, message);                 // "message"
EXTERN_FSTR(SINRICPRO, success);                 // "success"
EXTERN_FSTR(SINRICPRO, response);                // "response"
EXTERN_FSTR(SINRICPRO, header);                  // "header"
EXTERN_FSTR(SINRICPRO, payloadVersion);          // "payloadVersion"
EXTERN_FSTR(SINRICPRO, signatureVersion);        // "signatureVersion"
EXTERN_FSTR(SINRICPRO, payload);                 // "payload"
EXTERN_FSTR(SINRICPRO, action);                  // "action"
EXTERN_FSTR(SINRICPRO, cause);                   // "cause"
EXTERN_FSTR(SINRICPRO, type);                    // "type"
EXTERN_FSTR(SINRICPRO, createdAt);               // "createdAt"
EXTERN_FSTR(SINRICPRO, deviceId);                // "deviceId"
EXTERN_FSTR(SINRICPRO, replyToken);              // "replyToken"
EXTERN_FSTR(SINRICPRO, event);                   // "event"
EXTERN_FSTR(SINRICPRO, value);                   // "value"
EXTERN_FSTR(SINRICPRO, clientId);                // "clientId"
EXTERN_FSTR(SINRICPRO, request);                 // "request"
EXTERN_FSTR(SINRICPRO, signature);               // "signature"
EXTERN_FSTR(SINRICPRO, HMAC);                    // "HMAC"
EXTERN_FSTR(SINRICPRO, timestamp);               // "timestamp"
EXTERN_FSTR(SINRICPRO, PERIODIC_POLL);           // "PERIODIC_POLL"
EXTERN_FSTR(SINRICPRO, PHYSICAL_INTERACTION);    // "PHYSICAL_INTERACTION"
EXTERN_FSTR(SINRICPRO, OK);                      // "OK"

} // SINRICPRO_NAMESPACE