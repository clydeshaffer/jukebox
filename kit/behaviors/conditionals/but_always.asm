;PARAMS START
acc_value = 1
;PARAMS END

.include "engine.inc"

.export UPDATE_START

UPDATE_START:
    lda #acc_value
    rts
