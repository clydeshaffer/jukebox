.define MAX_ENTITIES 32

.struct Entity
vX .res MAX_ENTITIES
vY .res MAX_ENTITIES
Frame .res MAX_ENTITIES
Slot .res MAX_ENTITIES
HP .res MAX_ENTITIES
State .res MAX_ENTITIES
.endstruct



.define ENTITY_SLOTS 32

.struct EntSlot
Type .res ENTITY_SLOTS
GRAM_Bank .res ENTITY_SLOTS
Y_Offset .res ENTITY_SLOTS
Updater_HByte .res ENTITY_SLOTS
Updater_LByte .res ENTITY_SLOTS
Updater_Bank .res ENTITY_SLOTS
Frame_Tables_HByte .res ENTITY_SLOTS
Frame_Tables_LByte .res ENTITY_SLOTS
Frame_Tables_Bank .res ENTITY_SLOTS
.endstruct