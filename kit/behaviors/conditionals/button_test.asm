;PARAMS START
port_number = 0
mask_A = 16
mask_B = 0
on_press_only = 1
;PARAMS END

.if port_number = 0
inputsrcA = inputsP1A
inputsrcB = inputsP1B
inputsrcA_old = inputsP1A_old
inputsrcB_old = inputsP1B_old
.else
inputsrcA = inputsP2A
inputsrcB = inputsP2B
inputsrcA_old = inputsP2A_old
inputsrcB_old = inputsP2B_old
.endif

.include "engine.inc"

.export UPDATE_START

UPDATE_START:
    bne :+
    rts
:

.if mask_A <> 0
.if on_press_only <> 0
    lda inputsrcA_old
    eor #$ff
    and inputsrcA
.else 
    lda inputsrcA
.endif
    and #mask_A
    cmp #mask_A
    beq :+
    lda #0
    rts
:
.endif

.if mask_B <> 0
.if on_press_only <> 0
    lda inputsrcB_old
    eor #$ff
    and inputsrcB
.else 
    lda inputsrcB
.endif
    and #mask_B
    cmp #mask_B
    beq :+
    lda #0
    rts
:
.endif
    lda #1
    rts
