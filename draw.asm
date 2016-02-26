
	.module draw
	.optsdcc -mz80

	.globl _CDrawRectBorderClear

	.area _DATA

.nlist
.include "c_ti83p/ti83plus.inc"
.list

	.area _CODE

;; void CDrawRectBorderClear(uint8_t r1, uint8_t c1, uint8_t r2, uint8_t c2);
_CDrawRectBorderClear::
	push	ix
	ld	ix,#0
	add	ix,sp

	ld	h,4(ix)
	ld	l,5(ix)
	ld	d,6(ix)
	ld	e,7(ix)
	bcall _DrawRectBorderClear

	pop ix
	ret
