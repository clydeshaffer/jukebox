;PARAMS START
spawn_slot = 0
spawn_state = 0
spawn_hp = 0
spawn_frame = 0
x_offset = 0
y_offset = 0
;PARAMS END

.include "engine.inc"

.export UPDATE_START

UPDATE_START:
    bne :+
    rts
:
    lda ENTITIES::vX, x
.if x_offset <> 0
    clc
    adc #x_offset
.endif
    sta ENTITIES::vX+1, x
    
    lda ENTITIES::vY, x
.if y_offset <> 0
    clc
    adc #y_offset
.endif
    sta ENTITIES::vY+1, x

    lda #spawn_slot
    sta ENTITIES::Slot+1, x

    lda #spawn_state
    sta ENTITIES::State+1, x

    lda #spawn_hp
    sta ENTITIES::HP+1, x

    lda #spawn_frame
    sta ENTITIES::Frame+1, x
    rts
