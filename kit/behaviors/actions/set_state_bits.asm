;PARAMS START
state_bits = 0
actually_clear_the_bits_instead = 0
;PARAMS END

.include "engine.inc"

.export UPDATE_START

UPDATE_START:
    bne :+
    rts
:

.if actually_clear_the_bits_instead = 0
    lda #state_bits
    ora ENTITIES::State, x
.else
    lda #(~state_bits)
    and ENTITIES::State, x
.endif
    sta ENTITIES::State, x

    lda #1
    rts
