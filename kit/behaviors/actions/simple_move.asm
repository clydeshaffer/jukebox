;PARAMS START
x_increment = 1
y_increment = 0
;PARAMS END

.include "engine.inc"

.export UPDATE_START

UPDATE_START:
    bne :+
    rts
:
    lda ENTITIES::vX, x
    clc
    adc #x_increment
    sta ENTITIES::vX, x

    lda ENTITIES::vY, x
    clc
    adc #y_increment
    sta ENTITIES::vY, x

    lda #1
    rts
