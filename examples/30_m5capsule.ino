#include "M5UnitSynth.h"
#include "UsbMidi.h"

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
            break;
        }

        case MidiCin::NOTE_OFF: {
            uint8_t pitch = data[2];
            synth.setNoteOff(channel, pitch, 0);
            break;
        }
    }
}

void setup() {
    // For M5 Capsule, set GPIO pin 46 (HOLD) to HIGH during initialization to keep the device powered on.
    // This is necessary when running on the internal battery, as setting the HOLD pin HIGH
    // ensures that the device remains awake and operational after being triggered by the wake-up signal.
    // Without this step, the device will return to sleep mode.
    pinMode(GPIO_NUM_46, OUTPUT);
    digitalWrite(GPIO_NUM_46, HIGH);

    synth.begin(&Serial2, UNIT_SYNTH_BAUD, RX, TX);
    synth.reset();
    synth.setInstrument(0, 0, GrandPiano_1);
    synth.setInstrument(0, 9, SynthDrum);
    synth.setMasterVolume(80);
    synth.setVolume(0, 80);
    synth.setVolume(9, 80);
    program_current = GrandPiano_1;

    usbMidi.onMidiMessage(onMidiMessage);
    usbMidi.begin();
}

void loop() {
    usbMidi.update();
}