
#include <WebDav4WebServer.h>


#if WEBSERVER_HAS_HOOK
#if defined(ARDUINO_ARCH_ESP8266)
ESP8266WebServer::HookFunction hookWebDAVForWebserver(const String& davRootDir, ESPWebDAVCore& dav)
{
    return [&dav, davRootDir](const String & method, const String & url, WiFiClient * client, ESP8266WebServer::ContentTypeFunction contentType)
    {
        return
            url.indexOf(davRootDir) != 0 ? ESP8266WebServer::CLIENT_REQUEST_CAN_CONTINUE :
            dav.parseRequest(method, url, client, contentType) ? ESP8266WebServer::CLIENT_REQUEST_IS_HANDLED :
            ESP8266WebServer::CLIENT_MUST_STOP;
    };
}
#endif //ARDUINO_ARCH_ESP8266
#if defined(ARDUINO_ARCH_ESP32)
WebServer::HookFunction hookWebDAVForWebserver(const String& davRootDir, ESPWebDAVCore& dav)
{
    return [&dav, davRootDir](const String & method, const String & url, WiFiClient * client, WebServer::ContentTypeFunction contentType)
    {
        return
            url.indexOf(davRootDir) != 0 ? CLIENT_REQUEST_CAN_CONTINUE :
            dav.parseRequest(method, url, client, contentType) ? CLIENT_REQUEST_IS_HANDLED :
            CLIENT_MUST_STOP;
    };
}
#endif //ARDUINO_ARCH_ESP32


#endif // WEBSERVER_HAS_HOOK
