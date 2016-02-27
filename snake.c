#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "c_ti83p.h"
#include "draw.h"

#define DIMX 96
#define DIMY 64

#define SNAKE_BUFFER_SIZE 200
__at 0x9872 uint8_t snake_body[BUFFER_SIZE];
uint16_t snake_size;
uint16_t snake_body_offset; // 0 to (SNAKE_BUFFER_SIZE - 1)
uint8_t snake_head_x;
uint8_t snake_head_y;

#define MAX_OBJECTS 10
int8_t objects[MAX_OBJECTS * 2];

enum Direction { up, down, left, right };
int8_t direction;

#define pixel_on(buf, x, y) buf[(y)*12 + (x)/8] |= (0x80 >> ((x) & 0x07))
#define pixel_off(buf, x, y) buf[(y)*12 + (x)/8] &= ~(0x80 >> ((x) & 0x07))
#define pixel_test(buf, x, y) ((buf[(y)*12 + (x)/8] & (0x80 >> ((x) & 0x07))) != 0)

bool move_snake() {
    int16_t volatile head, tail;
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
    if (pixel_test(plotSScreen, snake_head_x, snake_head_y))
        return false;
    pixel_on(plotSScreen, snake_head_x, snake_head_y);
    if (snake_body[tail] != 255)
        pixel_off(plotSScreen, snake_body[tail], snake_body[tail + 1]);
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
    }
    CClrLCDFull();
    CRunIndicatorOn();
    return 0;
}
