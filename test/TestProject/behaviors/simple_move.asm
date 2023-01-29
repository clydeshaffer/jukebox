;PARAMS START
x_increment = 1
y_increment = 0
;PARAMS END

simple_move:
    lda ENTITIES::X, x
    clc
    adc #x_increment
    sta ENTITIES::X, x

    lda ENTITIES::Y, x
    clc
    adc #y_increment
    sta ENTITIES::Y, x
    rts
