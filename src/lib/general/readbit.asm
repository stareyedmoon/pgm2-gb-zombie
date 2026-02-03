    .globl _read_bit
	.globl _read_bits
    


    .area _CODE

; -----------------
; Function read_bit
; -----------------
; uint8_t* src		- de
; uint16_t* offset	- bc
_read_bit:

	; hl = &offset
	ld h, b
	ld l, c

	; bc = offset
	; (hl) += 1
	ld c, (hl)
	inc (hl)
	inc hl
	ld b, (hl)
	jr nz, _read_bit_no_inc_hl_high
	inc (hl)
_read_bit_no_inc_hl_high:


	; a = offset % 8
	ld a, #7
	and a, c

	; bc = offset / 8
	srl b
	rr  c

	srl b
	rr  c
	
	srl b
	rr  c



	; b = (src+bc)
	ld h, b
	ld l, c
	add hl, de
	ld b, (hl)

	; a = 1 << (7 - offset % 8)
	ld hl, #_read_bit_lut
	add a, l
	ld l, a
	ld a, (hl)

	; a = b & a
	and a, b

	jr z, .unset
	ld a, #1
.unset:
	ret

; ------------------
; Function read_bits
; ------------------
; uint8_t* src		- de
; uint16_t* offset	- bc
; uint8_t count		- sp+2
_read_bits:
	; a = count
	ldhl sp, #2
	ld a, (hl)

	; hl = &offset
	ld h, b
	ld l, c

	; bc = offset
	; (hl) += count
	ld c, (hl)
	add (hl)
	ld (hl), a
	inc hl
	ld b, (hl)
	jr nc, _read_bits_no_inc_hl_high
	inc (hl)
_read_bits_no_inc_hl_high:

	; a = offset % 8 + count
	ld a, #7
	and a, c
	ldhl sp, #2
	add (hl)

	; bc = offset / 8
	srl b
	rr  c

	srl b
	rr  c
	
	srl b
	rr  c

	; bc = be(src[bc])
	ld h, b
	ld l, c
	add hl, de
	ld b, (hl)
	inc hl
	ld c, (hl)

	; bc >>= 16 - a
	cpl
	add #17
_read_bits_shift_loop:
	srl b
	rr  c
	dec a
	jr nz, _read_bits_shift_loop

	; a = bc & lut[count-1]
	ldhl sp, #2
	ld a, (hl)
	dec a
	ld hl, #_read_bits_lut
	add l
	ld l, a
	ld a, (hl)
	and c

	;ret

	pop hl
	inc sp
	jp (hl)




_read_bit_lut:
	.db #0x80
	.db #0x40
	.db #0x20
	.db #0x10
	.db #0x08
	.db #0x04
	.db #0x02
	.db #0x01

_tmp_padding:
	.db #0
	.db #0
	.db #0
	.db #0
	.db #0
	.db #0
	.db #0

_read_bits_lut:
	.db #0x01
	.db #0x03
	.db #0x07
	.db #0x0f
	.db #0x1f
	.db #0x3f
	.db #0x7f
	.db #0xff
