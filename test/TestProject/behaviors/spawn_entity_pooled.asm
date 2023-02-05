;PARAMS START
spawn_slot = 2
spawn_state = 0
spawn_hp = 0
spawn_frame = 0
x_offset = 0
y_offset = 0

pool_size = 3
open_slot = 0
;PARAMS END

.include "engine.inc"

.export UPDATE_START

UPDATE_START:
    bne :+
    rts
:

    txa
    tay
.repeat pool_size, pool_index
    iny
    lda ENTITIES::Slot, y
    cmp #open_slot
    beq found_ent
.endrep
    lda #0
    rts

found_ent:
    lda ENTITIES::vX, x
.if x_offset <> 0
    clc
    adc #x_offset
.endif
    sta ENTITIES::vX, y
    
    lda ENTITIES::vY, x
.if y_offset <> 0
    clc
    adc #y_offset
.endif
    sta ENTITIES::vY, y

    lda #spawn_slot
    sta ENTITIES::Slot, y

    lda #spawn_state
    sta ENTITIES::State, y

    lda #spawn_hp
    sta ENTITIES::HP, y

    lda #spawn_frame
    sta ENTITIES::Frame, y
    rts
