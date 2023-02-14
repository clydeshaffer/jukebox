;PARAMS START
;PARAMS END

.include "engine.inc"

.export UPDATE_START

UPDATE_START:
    bne :+
    lda #1
    rts
:
    lda #0
    rts
