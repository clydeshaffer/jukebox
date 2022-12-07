.export LoadScene
.import EntityCount, Entities, EntSlots
.import ShiftROMBank
.importzp args, tmp_ptr

.include "scene.inc"
.include "entity.inc"
.include "../system/regs.inc"

.PC02

.code

.macro MOVE_PTR p, amt
.if amt = 256
    INC p+1
.else
    CLC
    LDA p
    ADC #amt
    STA p
    LDA p+1
    ADC #0
    STA p+1
.endif
.endmacro

.macro Y_COPY_PTR2ABS src, dest
.scope
@Loop:
    DEY
    LDA (src), y
    STA dest, y
    TYA
    BNE @Loop
.endscope
.endmacro

.macro LOAD_SLOT member
    PHY
    Y_COPY_PTR2ABS tmp_ptr, EntSlots+EntSlot::member
    PLY
    MOVE_PTR tmp_ptr, ENTITY_SLOTS
.endmacro

.macro LOAD_ENT member
    LDY EntityCount
    Y_COPY_PTR2ABS tmp_ptr, Entities+Entity::member
    MOVE_PTR tmp_ptr, MAX_ENTITIES
.endmacro

;args+0 and args+1 are pointer to SceneHeader
;args+2 is bank number
.proc LoadScene: near
    LDA args+2
    JSR ShiftROMBank
    JSR LoadEntSlots
    LDA args+2
    JSR ShiftROMBank
    JSR LoadEntities
    RTS
.endproc

.proc LoadEntSlots: near
    ;copy ptr to EntSlots
    LDY #SceneHeader::EntSlots
    LDA (args), y
    STA tmp_ptr
    INY
    LDA (args), y
    STA tmp_ptr+1
    LDY #SceneHeader::EntSlotCount
    LDA (args), y
    PHA
    LDY #SceneHeader::EntSlotsBank
    LDA (args), y
    JSR ShiftROMBank
    PLY
    LOAD_SLOT Type
    LOAD_SLOT GRAM_Bank
    LOAD_SLOT Y_Offset
    LOAD_SLOT Updater_HByte
    LOAD_SLOT Updater_LByte
    LOAD_SLOT Updater_Bank
    LOAD_SLOT Frame_Tables_HByte
    LOAD_SLOT Frame_Tables_LByte
    LOAD_SLOT Frame_Tables_Bank

    RTS
.endproc

.proc LoadEntities: near
    ;copy ptr to EntSlots
    LDY #SceneHeader::EntityList
    LDA (args), y
    STA tmp_ptr
    INY
    LDA (args), y
    STA tmp_ptr+1
    LDY #SceneHeader::EntityCount
    LDA (args), y
    STA EntityCount
    LDY #SceneHeader::EntityListBank
    LDA (args), y
    JSR ShiftROMBank

    LOAD_ENT vX
    LOAD_ENT vY
    LOAD_ENT Frame
    LOAD_ENT Slot
    LOAD_ENT HP
    LOAD_ENT State

    RTS
.endproc