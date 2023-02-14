;PARAMS START
target_mask = 0
invert_mask = 0
any_bit = 0
;PARAMS END

.include "engine.inc"

.export UPDATE_START

UPDATE_START:
    BNE :+
    RTS
    :
    LDA ENTITIES::State, x
.if invert_mask <> 0
    EOR #invert_mask
.endif
    AND #target_mask
.if any_bit <> 0
    BEQ :+
.else
    EOR #target_mask
    BNE :+
.endif
    LDA #1
    RTS
:
    LDA #0
    RTS