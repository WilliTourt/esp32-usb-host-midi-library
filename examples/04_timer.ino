#include "SimpleTimer.h"

SimpleTimer timer(500);

void setup() {
    Serial.begin(115200);
    Serial.printf("setup()\n");
}

void loop() {
    // Since MIDI IN is handled on the main thread in this library,
    // blocking delays like delay() would interrupt message processing.
    // This code uses a non-blocking timer to perform periodic actions instead.
    if (timer.shouldRun()) {
        Serial.printf("loop()\n");
    }
}