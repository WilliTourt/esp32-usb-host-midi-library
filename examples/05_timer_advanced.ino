#include "SimpleTimer.h"

// Two timers: fast and slow
SimpleTimer fastTimer(500);   // Timer object with 500ms interval
SimpleTimer slowTimer(2000);  // Timer object with 2000ms (2 seconds) interval

void setup() {
    Serial.begin(115200);
    Serial.println("setup() started");
}

void loop() {
    // Check if fastTimer has elapsed
    if (fastTimer.shouldRun()) {
        // Get current toggle state and count from timer
        bool toggle = fastTimer.getToggle();
        int count = fastTimer.getCount();

        Serial.printf("[Fast] Toggle: %s, Count: %d\n", toggle ? "ON" : "OFF", count);

        // Every 3rd call (except the first), perform a special action
        if (count % 3 == 0 && count != 0) {
            Serial.println("[Fast] Special action triggered!");
        }
    }

    // Check if slowTimer has elapsed (called every 2 seconds)
    if (slowTimer.shouldRun()) {
        bool toggle = slowTimer.getToggle();
        int count = slowTimer.getCount();

        Serial.printf("[Slow] Toggle: %s, Count: %d\n", toggle ? "ON" : "OFF", count);
    }
}
