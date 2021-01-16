
#ifndef __WEBDAV4WEBSERVER
#define __WEBDAV4WEBSERVER

#include <ESPWebDAV.h>
#if defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WebServer.h>
#endif //ARDUINO_ARCH_ESP8266
#if defined(ARDUINO_ARCH_ESP32)
#include <WebServer.h>
#endif //ARDUINO_ARCH_ESP32

#if WEBSERVER_HAS_HOOK

extern "C"
#if defined(ARDUINO_ARCH_ESP8266)
ESP8266WebServer::HookFunction hookWebDAVForWebserver(const String& davRootDir, ESPWebDAVCore& dav);
#endif //ARDUINO_ARCH_ESP8266
#if defined(ARDUINO_ARCH_ESP32)
WebServer::HookFunction hookWebDAVForWebserver(const String& davRootDir, ESPWebDAVCore& dav);
#endif //ARDUINO_ARCH_ESP32

#endif // WEBSERVER_HAS_HOOK

#endif // __WEBDAV4WEBSERVER
