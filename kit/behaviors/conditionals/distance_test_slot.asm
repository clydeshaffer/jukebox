;PARAMS START
test_x = 1
test_y = 1

x_offset = 0
y_offset = 0

distance = 8
slot_number = 0

;PARAMS END

.include "engine.inc"
.PC02

.export UPDATE_START

UPDATE_START:
    bne :+
    rts
:
    ldy #0
TestLoop:
    lda ENTITIES::Slot, y
    cmp #slot_number
    bne NextTest
    jsr TestEntityY
    beq NextTest
    lda #1
    rts
NextTest:
    iny
    cpy EntityCount
    bne TestLoop
    lda #0
    rts

TestEntityY:
.if test_x <> 0
    lda ENTITIES::vX, x
.if x_offset <> 0
    clc
    adc #x_offset
.endif
    sec
    sbc ENTITIES::vX,y
    jsr AbsVal
    sta scratch
.else
    stz scratch
.endif

.if test_y <> 0
    lda ENTITIES::vY, x
.if y_offset <> 0
    clc
    adc #y_offset
.endif
    sec
    sbc ENTITIES::vY,y
    jsr AbsVal
    clc
    adc scratch
.endif

    cmp #distance

    bcs outside_radius
    lda #1
    rts

outside_radius:
    lda #0
    rts