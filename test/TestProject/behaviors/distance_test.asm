;PARAMS START
test_x = 1
test_y = 1

x_offset = 0
y_offset = 0

distance = 8
entity_number = 0

;PARAMS END

.include "engine.inc"
.PC02

.export UPDATE_START

UPDATE_START:
    bne :+
    rts
:
.if test_x <> 0
    lda ENTITIES::vX, x
.if x_offset <> 0
    clc
    adc #x_offset
.endif
    sec
    sbc ENTITIES::vX+entity_number
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
    sbc ENTITIES::vY+entity_number
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