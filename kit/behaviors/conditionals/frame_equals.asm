;PARAMS START
target_value = 0
;PARAMS END

.include "engine.inc"

.export UPDATE_START

UPDATE_START:
    BNE :+
    RTS
    :
    LDA ENTITIES::Frame, x
    CMP #target_value
    BNE :+
    LDA #1
    RTS
:
    LDA #0
    RTS