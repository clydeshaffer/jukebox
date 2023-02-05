;PARAMS START
test_x = 1
test_y = 1

x_offset = 0
y_offset = 0

;PARAMS END

.include "engine.inc"

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
    bpl :+
    lda #1
    rts
:
.endif

.if test_y <> 0
    lda ENTITIES::vY, x
.if y_offset <> 0
    clc
    adc #y_offset
.endif
    bpl :+
    lda #1
    rts
:
.endif

    lda #0
    rts
