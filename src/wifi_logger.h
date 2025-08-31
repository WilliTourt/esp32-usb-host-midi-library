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

#ifndef WIFI_LOGGER_H
#define WIFI_LOGGER_H

// To enable Wi-Fi debugging, you need to set the SSID and password
// For details, refer to the comment a few lines below.
#define DEBUG_WIFI 0

#if DEBUG_WIFI

// You can choose one of the following three options to provide your Wi-Fi credentials:
//
// [Option 1]: Pass the SSID and password directly as arguments from your main .ino file.
// WIFI_BEGIN("SSID", "PASSWORD");
//
// [Option 2]: Define the SSID and password using macros below these lines.
// Uncomment and set the following two lines:
// #define WIFI_SSID "SSID"
// #define WIFI_PASSWORD "PASSWORD"
//
// [Option 3]: Include the credentials file (recommended for safety and clarity).
// Make sure to run set_wifi.py to generate this file first.
// Uncomment the following line:
// #include "credentials.h"

#define WIFI_LOG_BUFFER_SIZE 256
extern char g_wifi_log_buffer[WIFI_LOG_BUFFER_SIZE];

void WIFI_BEGIN(const char* ssid = nullptr, const char* password = nullptr);
void WIFI_CONFIG(const char* addr, int port);
void send_udp_log(const char* s);

#define WIFI_LOG(fmt, ...) \
    do { \
        snprintf(g_wifi_log_buffer, WIFI_LOG_BUFFER_SIZE, fmt, ##__VA_ARGS__); \
        send_udp_log(g_wifi_log_buffer); \
    } while(0)

#else

#define WIFI_BEGIN(...)           do {} while(0)
#define WIFI_CONFIG(addr, port)   do {} while(0) 
#define WIFI_LOG(fmt, ...)        do {} while(0)

#endif // DEBUG_WIFI

#endif // WIFI_LOGGER_H