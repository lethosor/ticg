#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "c_ti83p.h"
#include "draw.h"

#include "apple.h"

#define DIMX 96
#define DIMY 64

#define SNAKE_BUFFER_SIZE BUFFER_SIZE
#define MAX_SNAKE_SIZE ((SNAKE_BUFFER_SIZE - 1) / 2)
__at 0x9872 uint8_t snake_body[SNAKE_BUFFER_SIZE];
uint16_t snake_size;
uint16_t snake_body_offset; // 0 to (SNAKE_BUFFER_SIZE - 1)
uint8_t snake_head_x;
uint8_t snake_head_y;

// number of ticks until next apple appears, if possible
uint8_t apple_next_delay;
#define apple_next_delay_default (10)

uint16_t score;

enum Direction { UP, DOWN, LEFT, RIGHT };
uint8_t direction;
enum State { PLAYING, WON, LOST };
uint8_t state;

void sleep(unsigned ticks) {
    while (--ticks) {
        __asm
        nop
        __endasm;
    }
}

bool move_snake() {
    // indexes of head and tail coordinates in snake_body
    int16_t volatile head, tail;
    uint8_t tail_x, tail_y;
    if (snake_size == MAX_SNAKE_SIZE) {
        state = WON;
        return false;
    }
    head = snake_body_offset + 2;
    if (head >= SNAKE_BUFFER_SIZE)
        head = 0;
    tail = snake_body_offset - (snake_size * 2);
    if (tail < 0)
        tail += SNAKE_BUFFER_SIZE;
    switch (direction) {
    case UP:
        snake_head_y--;
        break;
    case DOWN:
        snake_head_y++;
        break;
    case LEFT:
        snake_head_x--;
        break;
    case RIGHT:
        snake_head_x++;
        break;
    }
    snake_body[head] = snake_head_x;
    snake_body[head + 1] = snake_head_y;
    snake_body_offset = head;
    if (pixel_test(plotSScreen, snake_head_x, snake_head_y)) {
        if (apple_remove(snake_head_x, snake_head_y)) {
            ++snake_size;
            ++score;
        }
        else {
            state = LOST;
            return false;
        }
    }
    pixel_on(plotSScreen, snake_head_x, snake_head_y);
    --apple_next_delay;
    tail_x = snake_body[tail];
    tail_y = snake_body[tail + 1];
    if (tail_x != 255) {
        if (apple_next_delay == 0)
            apple_next_delay = apple_add(tail_x, tail_y) ? apple_next_delay_default : 1;
        pixel_off(plotSScreen, tail_x, tail_y);
    }
    apple_draw_all(plotSScreen);
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
    score = 0;
    direction = LEFT;
    state = PLAYING;
    CRunIndicatorOff();
    CClrLCDFull();
    CDrawRectBorderClearFull();
    // Horizontal (DIMY - 8)
    memset(plotSScreen + (12 * (DIMY - 8)), 255, 12);
    key = 0;
    while (key != skClear)
    {
        if (!move_snake())
            break;
        key = CGetCSC();
        switch (key)
        {
        case skUp:
            if (direction != DOWN) direction = UP;
            break;
        case skDown:
            if (direction != UP) direction = DOWN;
            break;
        case skLeft:
            if (direction != RIGHT) direction = LEFT;
            break;
        case skRight:
            if (direction != LEFT) direction = RIGHT;
            break;
        }
        sleep(5000);
    }
    penRow = DIMY - 7;
    penCol = 1;
    CTextInvertOn();
    CVPutS((state == WON) ? "You WIN!" : "Game over!");
    CTextInvertOff();
    CGetKey();
    CClrLCDFull();
    memset(plotSScreen, 0, BUFFER_SIZE);
    CRunIndicatorOn();
    return 0;
}
