#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "c_ti83p.h"
#include "draw.h"

#include "apple.h"

#define DIMX 96
#define DIMY 64

#define SNAKE_BUFFER_SIZE 200
__at 0x9872 uint8_t snake_body[BUFFER_SIZE];
uint16_t snake_size;
uint16_t snake_body_offset; // 0 to (SNAKE_BUFFER_SIZE - 1)
uint8_t snake_head_x;
uint8_t snake_head_y;

// number of ticks until next apple appears, if possible
uint8_t apple_next_delay;
#define apple_next_delay_default (snake_size + 5)

enum Direction { up, down, left, right };
int8_t direction;

#define pixel_on(buf, x, y) buf[(y)*12 + (x)/8] |= (0x80 >> ((x) & 0x07))
#define pixel_off(buf, x, y) buf[(y)*12 + (x)/8] &= ~(0x80 >> ((x) & 0x07))
#define pixel_test(buf, x, y) ((buf[(y)*12 + (x)/8] & (0x80 >> ((x) & 0x07))))

void sleep(int ticks) {
    int i;
    for (i = 0; i < ticks; ++i) {
        __asm
        nop
        __endasm;
    }
}

bool move_snake() {
    // indexes of head and tail coordinates in snake_body
    int16_t volatile head, tail;
    uint8_t tail_x, tail_y;
    head = snake_body_offset + 2;
    if (head >= SNAKE_BUFFER_SIZE)
        head = 0;
    tail = snake_body_offset - (snake_size * 2);
    if (tail < 0)
        tail += SNAKE_BUFFER_SIZE;
    switch (direction) {
    case up:
        snake_head_y--;
        if (snake_head_y == 255)
            snake_head_y = DIMY - 1;
        break;
    case down:
        snake_head_y++;
        if (snake_head_y >= DIMY)
            snake_head_y = 0;
        break;
    case left:
        snake_head_x--;
        if (snake_head_x == 255)
            snake_head_x = DIMX - 1;
        break;
    case right:
        snake_head_x++;
        if (snake_head_x >= DIMX)
            snake_head_x = 0;
        break;
    }
    snake_body[head] = snake_head_x;
    snake_body[head + 1] = snake_head_y;
    snake_body_offset = head;
    if (pixel_test(plotSScreen, snake_head_x, snake_head_y)) {
        if (apple_exists(snake_head_x, snake_head_y)) {
            apple_remove(snake_head_x, snake_head_y);
            snake_size++;
        }
        else
            return false;
    }
    pixel_on(plotSScreen, snake_head_x, snake_head_y);
    --apple_next_delay;
    tail_x = snake_body[tail];
    tail_y = snake_body[tail + 1];
    if (tail_x != 255) {
        // if creating an apple is possible, register one and leave this pixel behind on-screen
        // otherwise, clear this pixel
        if (apple_next_delay == 0 && apple_add(tail_x, tail_y))
            apple_next_delay = apple_next_delay_default;
        else
        {
            // don't erase an apple created here at the last tick
            if (!apple_exists(tail_x, tail_y))
                pixel_off(plotSScreen, tail_x, tail_y);
            // if adding an apple failed, add one at the next possible tick
            if (apple_next_delay == 0)
                apple_next_delay = 1;
        }
    }
    FastCopy();
    return true;
}

int main() {
    uint8_t key;
    memset(snake_body, 255, SNAKE_BUFFER_SIZE);
    snake_size = 5;
    snake_body_offset = snake_size * 2;
    snake_head_x = DIMX / 2;
    snake_head_y = DIMY / 2;
    apple_init();
    apple_next_delay = apple_next_delay_default;
    direction = left;
    CClrLCDFull();
    CDrawRectBorderClearFull();
    CRunIndicatorOff();
    key = 0;
    while (key != skClear)
    {
        if (!move_snake())
            break;
        key = CGetCSC();
        switch (key)
        {
        case skUp:
            if (direction != down) direction = up;
            break;
        case skDown:
            if (direction != up) direction = down;
            break;
        case skLeft:
            if (direction != right) direction = left;
            break;
        case skRight:
            if (direction != left) direction = right;
            break;
        }
        sleep(3000);
    }
    penRow = 28;
    penCol = 25;
    CTextInvertOn();
    CVPutS("Game over!");
    CTextInvertOff();
    CGetKey();
    CClrLCDFull();
    CRunIndicatorOn();
    return 0;
}
