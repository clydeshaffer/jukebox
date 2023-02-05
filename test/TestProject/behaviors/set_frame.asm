;PARAMS START
new_frame = 0
;PARAMS END

.include "engine.inc"

.export UPDATE_START

UPDATE_START:
    bne :+
    rts
:
    lda #new_frame
    sta ENTITIES::Frame, x

    lda #1
    rts
