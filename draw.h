#include "c_ti83p.h"

void CDrawRectBorderClear(uint8_t r1, uint8_t c1, uint8_t r2, uint8_t c2);
#define CDrawRectBorderClearFull() CDrawRectBorderClear(0, 0, SCREEN_HEIGHT - 1, SCREEN_WIDTH - 1)
