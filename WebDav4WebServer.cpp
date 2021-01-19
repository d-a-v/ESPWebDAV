
#include <WebDav4WebServer.h>


#if WEBSERVER_HAS_HOOK
#if defined(ARDUINO_ARCH_ESP8266)
ESP8266WebServer::HookFunction hookWebDAVForWebserver(const String& davRootDir, ESPWebDAVCore& dav)
{
    return [&dav, davRootDir](const String & method, const String & url, WiFiClient * client, ESP8266WebServer::ContentTypeFunction contentType)
    {
        if (url.indexOf(davRootDir) != 0) {
            DBG_PRINT ("CLIENT_REQUEST_CAN_CONTINUE, %s is not seen in %s", davRootDir.c_str(), url.c_str());
            return ESP8266WebServer::CLIENT_REQUEST_CAN_CONTINUE;
        } else {
            if (dav.parseRequest(method, url, client, contentType)){
                DBG_PRINT ("CLIENT_REQUEST_IS_HANDLED");
                return ESP8266WebServer::CLIENT_REQUEST_IS_HANDLED;
            } else {
                DBG_PRINT ("CLIENT_MUST_STOP");
                return ESP8266WebServer::CLIENT_MUST_STOP;
            }
        }
    };
}
#endif //ARDUINO_ARCH_ESP8266
#if defined(ARDUINO_ARCH_ESP32)
WebServer::HookFunction hookWebDAVForWebserver(const String& davRootDir, ESPWebDAVCore& dav)
{
    return [&dav, davRootDir](const String & method, const String & url, WiFiClient * client, WebServer::ContentTypeFunction contentType)
    {
        if (url.indexOf(davRootDir) != 0) {
            DBG_PRINT ("CLIENT_REQUEST_CAN_CONTINUE, %s is not seen in %s", davRootDir.c_str(), url.c_str());
            return CLIENT_REQUEST_CAN_CONTINUE;
        } else {
            if (dav.parseRequest(method, url, client, contentType)){
                DBG_PRINT ("CLIENT_REQUEST_IS_HANDLED");
                return CLIENT_REQUEST_IS_HANDLED;
            } else {
                DBG_PRINT ("CLIENT_MUST_STOP");
                return CLIENT_MUST_STOP;
            }
        }
    };
}
#endif //ARDUINO_ARCH_ESP32


#endif // WEBSERVER_HAS_HOOK
