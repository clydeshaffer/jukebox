;PARAMS START
new_slot = 0
;PARAMS END

.include "engine.inc"

.export UPDATE_START

UPDATE_START:
    bne :+
    rts
:
    lda #new_slot
    sta ENTITIES::Slot, x

    lda #1
    rts
