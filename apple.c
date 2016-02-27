#include <string.h>
#include "c_ti83p.h"
#include "draw.h"
#include "apple.h"

#define MAX_APPLES 20
#define APPLE_BUFFER_SIZE (MAX_APPLES * 2)

// on-screen apple positions (0 = invalid)
uint8_t apples[APPLE_BUFFER_SIZE];
// number of on-screen apples
uint8_t apple_count;

void apple_init() {
    apple_count = 0;
    memset(apples, 0, APPLE_BUFFER_SIZE);
}

bool apple_add (uint8_t x, uint8_t y) {
    int i;
    if (apple_exists(x, y) || apple_count == MAX_APPLES)
        return false;
    for (i = 0; i < APPLE_BUFFER_SIZE; i += 2) {
        if (apples[i] == 0) {
            apples[i] = x;
            apples[i + 1] = y;
            ++apple_count;
            return true;
        }
    }
    return false;
}

bool apple_exists (uint8_t x, uint8_t y) {
    int i;
    for (i = 0; i < APPLE_BUFFER_SIZE; i += 2) {
        if (apples[i] == x && apples[i + 1] == y)
            return true;
    }
    return false;
}

bool apple_remove (uint8_t x, uint8_t y) {
    int i;
    for (i = 0; i < APPLE_BUFFER_SIZE; i += 2) {
        if (apples[i] == x && apples[i + 1] == y) {
            apples[i] = 0;
            apples[i + 1] = 0;
            --apple_count;
            return true;
        }
    }
    return false;
}

void apple_draw_all (uint8_t *buffer) {
    int i;
    for (i = 0; i < APPLE_BUFFER_SIZE; i += 2)
        pixel_on(buffer, apples[i], apples[i + 1]);
}
