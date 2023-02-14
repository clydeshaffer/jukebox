;PARAMS START
mask_value = 1
inv_mask = 0
;PARAMS END

.include "engine.inc"

.export UPDATE_START

UPDATE_START:
    bne :+
    rts
:
    lda globalTick
.if inv_mask <> 0
    eor #inv_mask
.endif
    and #mask_value
    eor #mask_value
    bne :+
    lda #1
    rts
:
    lda #0
    rts
