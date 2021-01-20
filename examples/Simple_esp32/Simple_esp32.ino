/*
    Copyright (c) 2018 Gurpreet Bal https://github.com/ardyesp/ESPWebDAV
    Copyright (c) 2020 David Gauchard https://github.com/d-a-v/ESPWebDAV
    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification,
    are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    3. The name of the author may not be used to endorse or promote products
      derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
    WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
    SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
    OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
    IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
    OF SUCH DAMAGE.

*/

/*  Using the WebDAV server

    From windows file explorer,
        \\ESPWebDAV.local\
        or http://ESPWebDAV.local/
        or Map Network Drive -> Connect to http://ESPWebDAV.local/
           subst w: \\ESPWebDAV.local/DavWWWRoot
    From macOS Finder > command-K > http://ESPWebDAV.local/
        (do not select anonymous to have write access)
    From macOS cmdline:
        mkdir -p /tmp/esp; mount_webdav -S -i -v esp32 http://ESPWebDAV.local/ /tmp/esp && echo OK
    From linux:
        use mount -t davs2 http://ESPWebDAV.local/ /mnt/
        use gio/gvfs/nautilus/YourFileExplorer http://ESPWebDAV.local/

    When running emulation on host (script ./run), use one of these URLs instead:
        http://ESPWebDAV.local:9080/
        http://127.0.0.1:9080/
        http://local-ip-address:9080/
        subst w: \\ESPWebDAV.local@9080/DavWWWRoot
*/

#include <WiFi.h>
#include <ESPmDNS.h>
//#include <SPIFFS.h>
#include <LITTLEFS.h>
#include <ESPWebDAV.h>

#define HOSTNAME    "ESPWebDAV"

#ifndef STASSID
#define STASSID "ssid"
#define STAPSK "psk"
#endif

FS& gfs = LITTLEFS;
#define FILESYSTEM LITTLEFS
//#define FILESYSTEM SPIFFS

//WiFiServerSecure tcp(443);
WiFiServer tcp(80);

ESPWebDAV dav;

// ------------------------
void setup()
{
    // ------------------------
    WiFi.persistent(false);
    WiFi.setHostname(HOSTNAME);
    WiFi.mode(WIFI_STA);
    Serial.begin(115200);
    WiFi.begin(STASSID, STAPSK);
    Serial.println("Connecting to " STASSID " ...");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.print("Connected to "); Serial.println(STASSID);
    Serial.print("IP address: "); Serial.println(WiFi.localIP());
    Serial.print("RSSI: "); Serial.println(WiFi.RSSI());

    MDNS.begin(HOSTNAME);

    FILESYSTEM.begin();
    tcp.begin();
    dav.begin(&tcp, &gfs);
    dav.setTransferStatusCallback([](const char* name, int percent, bool receive)
    {
        Serial.printf("%s: '%s': %d%%\n", receive? "recv": "send", name, percent);
    });

    Serial.println("WebDAV server started");
}

void help()
{
    Serial.printf("interactive: F/ormat D/ir C/reateFile\n");

    Serial.printf("Heap stats: free heap: %u\n",
                  ESP.getHeapSize());
}

// ------------------------
void loop()
{
    dav.handleClient();

    int c = Serial.read();
    if (c > 0)
    {
        if (c == 'F')
        {
            Serial.println("formatting...");
            if (FILESYSTEM.format())
                Serial.println("Success");
            else
                Serial.println("Failure");
        }
        else if (c == 'D')
        {
            Serial.printf(">>>>>>>> dir /\n");
            dav.dir("/", &Serial);
            Serial.printf("<<<<<<<< dir\n");
        }
        else if (c == 'C')
        {
            auto f = gfs.open("/readme.md", "w");
            f.printf("hello\n");
            f.close();
        }
        else
            help();
    }

    // ------------------------
}
