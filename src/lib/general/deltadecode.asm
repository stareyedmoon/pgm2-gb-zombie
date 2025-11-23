    .globl _decompression_deltadecode
    


    .area _CODE

; Registers:
;  8-bit -  A,  B,  C, D, E, H, L
; 16-bit - BC, DE, HL

; ----------------------------------
; Function decompression_deltadecode
; ----------------------------------
; uint8_t* dest     - de
; uint8_t width     - a
; uint8_t height    - sp+2
_decompression_deltadecode:
    ldhl sp, #2
    ld  c, (hl)
    ld  (hl), a
    
    ; dest   - de
    ; width  - sp+2
    ; height - c
    
    swap a
    and a, #0xF0
    ld  b, a

    ; width and height are in tiles

    ; line length is `width` bytes
    .y_loop:
        push bc
        push de

        ld  c, #8
        .tile_height_loop:
            ldhl sp, #6
            ld  b, (hl)
            ld  l, #0x00

            push de
            .x_loop:
                ; VRAM access
                push hl
                ld  hl, #0xFF41 ; STAT
                .wait_stat_1:
                    bit 1, (hl)
                    jr  nz, .wait_stat_1
                pop hl
                ld  a, (de)

                push bc
                push de

                ld  b, #0

                ld  e, a
                swap a
                and a, #0x0F
                or  a, l
                
                ld  hl, #_decompression_deltadecode_lut
                ld  c, a
                add hl, bc
                ld  l, (hl)

                ld  a, l
                and a, #0x0F
                swap a
                ld  d, a

                ld  a, l
                and a, #0x10
                ld  l, a

                ld  a, e
                and a, #0x0F
                or  a, l

                ld  hl, #_decompression_deltadecode_lut
                ld  c, a
                add hl, bc
                ld  l, (hl)

                ld  a, l
                and a, #0x0F
                or  a, d
                ld  d, a

                ld  a, l
                and a, #0x10
                ld  l, a

                ld  a, d

                ld  b, h
                ld  c, l
                pop hl

                ; VRAM access
                push hl
                ld  hl, #0xFF41 ; STAT
                .wait_stat_2:
                    bit 1, (hl)
                    jr  nz, .wait_stat_2
                pop hl
                
                ld  (hl), a

                ld  de, #16
                add hl, de


                ld  d, h ; ld de, hl
                ld  e, l
                ld  h, b ; ld hl, bc
                ld  l, c

                pop bc

                dec b
                jr  NZ, #.x_loop
                
            pop hl
            inc hl
            inc hl
                
            ld  d, h ; ld  de, hl
            ld  e, l

            dec c
            jr  NZ, #.tile_height_loop
        
        pop hl
        pop bc

        push bc
        ld  c, b
        ld  b, #0
        add hl, bc
        pop bc

        ld  d, h ; ld  de, hl
        ld  e, l

        dec c
        jr  NZ, #.y_loop

    pop hl
    inc sp
    jp (hl)

_decompression_deltadecode_lut:
    .db #0x00   ; 0 0000
    .db #0x11   ; 0 0001
    .db #0x13   ; 0 0010
    .db #0x02   ; 0 0011
    .db #0x17   ; 0 0100
    .db #0x06   ; 0 0101
    .db #0x04   ; 0 0110
    .db #0x15   ; 0 0111
    .db #0x1F   ; 0 1000
    .db #0x0E   ; 0 1001
    .db #0x0C   ; 0 1010
    .db #0x1D   ; 0 1011
    .db #0x08   ; 0 1100
    .db #0x19   ; 0 1101
    .db #0x1B   ; 0 1110
    .db #0x0A   ; 0 1111
    .db #0x1F   ; 1 0000
    .db #0x0E   ; 1 0001
    .db #0x0C   ; 1 0010
    .db #0x1D   ; 1 0011
    .db #0x08   ; 1 0100
    .db #0x19   ; 1 0101
    .db #0x1B   ; 1 0110
    .db #0x0A   ; 1 0111
    .db #0x00   ; 1 1000
    .db #0x11   ; 1 1001
    .db #0x13   ; 1 1010
    .db #0x02   ; 1 1011
    .db #0x17   ; 1 1100
    .db #0x06   ; 1 1101
    .db #0x04   ; 1 1110
    .db #0x15   ; 1 1111
