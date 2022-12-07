.export InitialSceneHeader

.PC02

.import Entities
.include "../game/entity.inc"

.segment "RODATA"
TestFrameData:
    .res 256, $0 ;x
    .res 256, $0 ;y
    .res 256, $10 ;w
    .res 256, $10 ;h
    .res 256, $0  ;gx
    .res 256, $0  ;gy
    .res 256, $86  ;c
    .res 256, $0  ;b

TestUpdateFunc_MoveRight:
    INC Entities+Entity::vX, x
    RTS

TestUpdateFunc_MoveDown:
    INC Entities+Entity::vY, x
    RTS

InitialSlots:
    .res ENTITY_SLOTS, 0 ;type
    .res ENTITY_SLOTS, 0 ;bank
    .res ENTITY_SLOTS, 0 ;y offset

    .byte >TestUpdateFunc_MoveDown ;Updater H
    .byte >TestUpdateFunc_MoveRight ;Updater H
    .res ENTITY_SLOTS-2, 0

    .byte <TestUpdateFunc_MoveDown ;Updater L
    .byte <TestUpdateFunc_MoveRight ;Updater L
    .res ENTITY_SLOTS-2, 0
    
    .res ENTITY_SLOTS, 127 ; Updater bank

    .byte >TestFrameData; FrameTables H
    .byte >TestFrameData; FrameTables H
    .res ENTITY_SLOTS-2, 0

    .byte <TestFrameData; FrameTables L
    .byte <TestFrameData; FrameTables L
    .res ENTITY_SLOTS-2, 0

    .res ENTITY_SLOTS, 127 ; FrameTables bank
InitialEnts:

    .byte 24
    .res MAX_ENTITIES-1, 10; x

    .byte 55
    .res MAX_ENTITIES-1, 10; y
    .res MAX_ENTITIES, 0 ; frame

    .byte 0
    .res MAX_ENTITIES-1, 1 ; slot

    .res MAX_ENTITIES, 100 ; HP
    .res MAX_ENTITIES, 0 ; state

InitialSceneHeader:
    .byte $2 ;EntSlotCount
    .word InitialSlots ;EntSlots
    .byte $FE ;EntSlotsBank
    .byte $2 ;EntityCount
    .word InitialEnts
    .byte $FE ;EntityListBank


