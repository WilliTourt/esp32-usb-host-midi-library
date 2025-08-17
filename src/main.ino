#include "M5UnitSynth.h"
#include "UsbMidi.h"

UsbMidi usbMidi;

void onMidiMessage(const uint8_t (&data)[4]) {
    auto cin = static_cast<MidiCin>(data[0] & 0x0F);
    uint8_t channel = data[1] & 0x0F;

    switch (cin) {
        case MidiCin::NOTE_ON: {
            uint8_t pitch = data[2];
            uint8_t velocity = data[3];
            Serial.printf("NoteOn: ch:%d / pitch:%d / vel:%d\n", channel, pitch, velocity);
            break;
        }

        case MidiCin::NOTE_OFF: {
            uint8_t pitch = data[2];
            Serial.printf("NoteOff: ch:%d / key:%d\n", channel, pitch);
            break;
        }

        case MidiCin::CONTROL_CHANGE: {
            uint8_t control = data[2];
            uint8_t value = data[3];
            Serial.printf("Control Chnage: ch:%d / cc:%d / v:%d\n", channel, control, value);
            break;
        }

        case MidiCin::PROGRAM_CHANGE: {
            uint8_t program = data[2];
            Serial.printf("Program Change: program:%d\n", program);
            break;
        }
        default:
            break;
    }
}

void onMidiDeviceConnected() {
    Serial.printf("onMidiDeviceConnected\n");
}

void onMidiDeviceDisconnected() {
    Serial.printf("onMidiDeviceDisconnected\n");
}

void setup() {
    Serial.begin(115200);
    Serial.printf("setup()\n");

    usbMidi.onMidiMessage(onMidiMessage);
    usbMidi.onDeviceConnected(onMidiDeviceConnected);
    usbMidi.onDeviceDisconnected(onMidiDeviceDisconnected);
    usbMidi.begin();
}

void loop() {
    usbMidi.update();
}