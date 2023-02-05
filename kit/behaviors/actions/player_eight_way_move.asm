;PARAMS START
port_number = 0
;PARAMS END

.if port_number = 0
inputsrc = inputsP1B
.else
inputsrc = inputsP2B
.endif

.include "engine.inc"

INPUT_MASK_UP		= %00001000
INPUT_MASK_DOWN		= %00000100
INPUT_MASK_LEFT		= %00000010
INPUT_MASK_RIGHT	= %00000001

.export UPDATE_START

UPDATE_START:
    bne :+
    rts
:
    lda inputsrc
    and #INPUT_MASK_RIGHT
    beq :+
    inc ENTITIES::vX, x
:
    lda inputsrc
    and #INPUT_MASK_LEFT
    beq :+
    dec ENTITIES::vX, x
:
    lda inputsrc
    and #INPUT_MASK_DOWN
    beq :+
    inc ENTITIES::vY, x
:
    lda inputsrc
    and #INPUT_MASK_UP
    beq :+
    dec ENTITIES::vY, x
:
    lda #1
    rts
