#include "UsbMidi.h"
#include "wifi_logger.h"

UsbMidi usbMidi;

void onMidiMessage(const uint8_t (&data)[4]) {
    auto cin = static_cast<MidiCin>(data[0] & 0x0F);
    uint8_t channel = data[1] & 0x0F;

    switch (cin) {
        case MidiCin::NOTE_ON: {
            uint8_t pitch = data[2];
            uint8_t velocity = data[3];
            WIFI_LOG("NoteOn: ch:%d / pitch:%d / vel:%d", channel, pitch, velocity);
            break;
        }

        case MidiCin::NOTE_OFF: {
            uint8_t pitch = data[2];
            WIFI_LOG("NoteOff: ch:%d / key:%d", channel, pitch);
            break;
        }

        case MidiCin::CONTROL_CHANGE: {
            uint8_t control = data[2];
            uint8_t value = data[3];
            WIFI_LOG("Control Chnage: ch:%d / cc:%d / v:%d", channel, control, value);
            break;
        }

        case MidiCin::PROGRAM_CHANGE: {
            uint8_t program = data[2];
            WIFI_LOG("Program Change: program:%d", program);
            break;
        }
        default:
            break;
    }
}

void onMidiDeviceConnected() {
    WIFI_LOG("onMidiDeviceConnected");
}

void onMidiDeviceDisconnected() {
    WIFI_LOG("onMidiDeviceDisconnected");
}

void setup() {
    // Please open wifi_logger.h and set DEBUG_WIFI to 1.
    WIFI_CONFIG("192.168.0.255", 10000); // Broadcast
    WIFI_BEGIN("SSID", "PASSWORD");

    usbMidi.onMidiMessage(onMidiMessage);
    usbMidi.onDeviceConnected(onMidiDeviceConnected);
    usbMidi.onDeviceDisconnected(onMidiDeviceDisconnected);
    usbMidi.begin();
}

void loop() {
    usbMidi.update();
}