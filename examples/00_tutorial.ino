#include "UsbMidi.h"

UsbMidi usbMidi;

void onMidiMessage(const uint8_t (&data)[4]) {
    // USB-MIDI message received. See examples for more info.
}

void setup() {
    usbMidi.onMidiMessage(onMidiMessage);
    usbMidi.begin();
}

void loop() {
    usbMidi.update();
}