
.struct SpriteLoadCommand
SheetPtr .word
SheetBank .byte
GRAMBank .byte ;;TODO encode bank and Xoffset
Y_Offset .byte
.endstruct

.struct SceneHeader
EntSlotCount .byte
EntSlots .word
EntSlotsBank .byte
EntityCount .byte
EntityList .word
EntityListBank .byte
SpriteLoadCount .byte
SpriteLoadList .word
SpriteLoadListBank .byte
.endstruct