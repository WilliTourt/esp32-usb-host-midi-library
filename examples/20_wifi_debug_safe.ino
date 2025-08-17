#include "UsbMidi.h"
#include "wifi_logger.h"

UsbMidi usbMidi;

void onMidiMessage(const uint8_t (&data)[4]) {
    WIFI_LOG("onMidiMessage");
}

void onMidiDeviceConnected() {
    WIFI_LOG("onMidiDeviceConnected");
}

void onMidiDeviceDisconnected() {
    WIFI_LOG("onMidiDeviceDisconnected");
}

void setup() {
    // Please open wifi_logger.h and set DEBUG_WIFI to 1.

    // Specify the IP address of the PC that will receive debug messages.
    WIFI_CONFIG("192.168.0.10", 10000);

    // Here, we choose Option 3 to include credentials.h. For details, see wifi_logger.h.
    WIFI_BEGIN();

    usbMidi.onMidiMessage(onMidiMessage);
    usbMidi.onDeviceConnected(onMidiDeviceConnected);
    usbMidi.onDeviceDisconnected(onMidiDeviceDisconnected);
    usbMidi.begin();
}

void loop() {
    usbMidi.update();
}