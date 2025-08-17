#include "SimpleTimer.h"
#include "UsbMidi.h"

UsbMidi usbMidi;

// Timer with 500ms interval, starting with count = 1 
// so toggle begins as true (e.g., first run acts "ON"
SimpleTimer timer(500, 1);

// MIDI message callback: prints received MIDI data bytes in hex
void onMidiMessage(const uint8_t (&d)[4]) {
    Serial.printf("USB MIDI: %02X %02X %02X %02X\n", d[0], d[1], d[2], d[3]);
}

// Callback when a MIDI device is connected
void onMidiDeviceConnected() {
    Serial.println("onMidiDeviceConnected");
}

// Callback when a MIDI device is disconnected
void onMidiDeviceDisconnected() {
    Serial.println("onMidiDeviceDisconnected");
}

void setup() {
    Serial.begin(115200);
    Serial.println("setup()");

    usbMidi.onMidiMessage(onMidiMessage);
    usbMidi.onDeviceConnected(onMidiDeviceConnected);
    usbMidi.onDeviceDisconnected(onMidiDeviceDisconnected);
    usbMidi.begin();
}

void loop() {
    usbMidi.update();

    if (timer.shouldRun()) {
        // Get toggle state and count from timer
        bool toggle = timer.getToggle();
        int count = timer.getCount();

        // Use C4 (60) for first 20 steps, then D4 (62)
        int note = (count < 20) ? 60 : 62;

        // On even toggles, send Note On; on odd toggles, send Note Off
        if (toggle) {
            usbMidi.noteOn(0, note, 100);
        } else {
            usbMidi.noteOff(0, note, 0);
        }

        // Optional: Print debug info
        Serial.printf("Note: %d, Toggle: %s, Count: %d\n", note, toggle ? "ON" : "OFF", count);
    }
}
