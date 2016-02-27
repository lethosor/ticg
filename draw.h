#include <stdint.h>
#include "c_ti83p.h"

void CDrawRectBorderClear(uint8_t r1, uint8_t c1, uint8_t r2, uint8_t c2);
#define CDrawRectBorderClearFull() CDrawRectBorderClear(0, 0, SCREEN_HEIGHT - 1, SCREEN_WIDTH - 1)

#define pixel_on(buf, x, y) (buf)[(y)*12 + (x)/8] |= (0x80 >> ((x) & 0x07))
#define pixel_off(buf, x, y) (buf)[(y)*12 + (x)/8] &= ~(0x80 >> ((x) & 0x07))
#define pixel_test(buf, x, y) (((buf)[(y)*12 + (x)/8] & (0x80 >> ((x) & 0x07))))
