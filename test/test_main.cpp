#include <Arduino.h>
#include <unity.h>

#include "test_render.h"
#include "test_scroller.h"

using namespace tint_matrix;

void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    UNITY_BEGIN();
    
    test_render();
    test_scroller();

    UNITY_END();
}

void loop() {
}