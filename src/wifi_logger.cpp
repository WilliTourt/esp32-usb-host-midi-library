/*
Easy WiFi Logger - Simple Wireless Debugging Without Serial Ports or Messy Wiring

MIT License

Copyright (c) 2025 ndenki

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/


#include "wifi_logger.h"

#if DEBUG_WIFI

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>

static WiFiUDP s_wifi_udp;
static char s_remote_addr[16] = "192.168.0.255";
static int s_remote_port = 10000;
static const int s_local_port = 40000;
char g_wifi_log_buffer[WIFI_LOG_BUFFER_SIZE];

void WIFI_CONFIG(const char* addr, int port) {
    if (addr != nullptr) {
        snprintf(s_remote_addr, sizeof(s_remote_addr), "%s", addr);
    }
    if (port > 0 && port < 65536) {
        s_remote_port = port;
    }
}

void send_udp_log(const char* s) {
    if (WiFi.status() == WL_CONNECTED) {
        s_wifi_udp.beginPacket(s_remote_addr, s_remote_port);
        s_wifi_udp.print(s);
        s_wifi_udp.endPacket();
    }
}

void WIFI_BEGIN(const char* ssid, const char* password) {
    #ifdef WIFI_SSID
    ssid = (ssid != nullptr) ? ssid : WIFI_SSID;
    #endif
    #ifdef WIFI_PASSWORD
    password = (password != nullptr) ? password : WIFI_PASSWORD;
    #endif

    WiFi.begin(ssid, password);
    int retries = 0;
    while (WiFi.status() != WL_CONNECTED && retries < 30) {
        delay(100);
        retries++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        s_wifi_udp.begin(s_local_port);
        WIFI_LOG("Logger initialized. IP: %s", WiFi.localIP().toString().c_str());
    }
}

#endif // DEBUG_WIFI