#include "M5UnitSynth.h"
#include "UsbMidi.h"
#include "wifi_logger.h"
#include "SimpleTimer.h"

#define RX 15
#define TX 13

UsbMidi usbMidi;
M5UnitSynth synth;
uint8_t program_current = 0;

void onMidiMessage(const uint8_t (&data)[4]) {
    auto cin = static_cast<MidiCin>(data[0] & 0x0F);
    uint8_t channel = data[1] & 0x0F;

    switch (cin) {
        case MidiCin::NOTE_ON: {
            uint8_t pitch = data[2];
            uint8_t velocity = data[3];
            synth.setNoteOn(channel, pitch, velocity);
            WIFI_LOG("NoteOn: ch:%d / pitch:%d / vel:%d", channel, pitch, velocity);
            break;
        }

        case MidiCin::NOTE_OFF: {
            uint8_t pitch = data[2];
            synth.setNoteOff(channel, pitch, 0);
            WIFI_LOG("NoteOff: ch:%d / key:%d", channel, pitch);
            break;
        }

        case MidiCin::CONTROL_CHANGE: {
            uint8_t control = data[2];
            uint8_t value = data[3];
            switch (control) {
                case 73: synth.setInstrument(0, 0, value); program_current = value; break;
                case 7:  synth.setVolume(0, value); synth.setVolume(9, value); break;
                case 91: synth.setReverb(0, program_current, value, 0x22); break;
                case 93: synth.setChorus(0, program_current, value, 0, 0x0C); break;
            }
            WIFI_LOG("Control Chnage: ch:%d / cc:%d / v:%d", channel, control, value);
            break;
        }

        case MidiCin::PROGRAM_CHANGE: {
            uint8_t program = data[2];
            bool isChange = false;
            // This is a very unique, author-specific implementation.
            if (program == 0 && program_current > 0) {
                program_current--;
                isChange = true;
            } else if (program == 1 && program_current < 127) {
                program_current++;
                isChange = true;
            }

            if (isChange) {
                synth.setInstrument(0, 0, program_current);
            }
            WIFI_LOG("Program Change: pr:%d -> new:%d", program, program_current);
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
    WIFI_BEGIN();

    synth.begin(&Serial2, UNIT_SYNTH_BAUD, RX, TX);
    synth.reset();
    synth.setInstrument(0, 0, GrandPiano_1);
    synth.setInstrument(0, 9, SynthDrum);
    synth.setMasterVolume(127);
    synth.setVolume(0, 127);
    synth.setVolume(9, 127);
    program_current = GrandPiano_1;

    usbMidi.onMidiMessage(onMidiMessage);
    usbMidi.onDeviceConnected(onMidiDeviceConnected);
    usbMidi.onDeviceDisconnected(onMidiDeviceDisconnected);
    usbMidi.begin();
}

void loop() {
    usbMidi.update();
}