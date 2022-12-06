.PC02

.import _pushRect, _render_queue_input, _queue_pending, _NextQueue, _queue_flags_param
.import ShiftROMBank
.export EntityCount, Entities, EntSlots
.export Draw_All_Entities, FrameDataPtr, Run_Update_Funcs

.include "../graphics/rect.inc"
.include "../system/regs.inc"

.include "entity.inc"

.segment "ZEROPAGE"
FrameDataPtr:
    .res 2
.segment "BSS"
Entities:
    .TAG Entity
EntSlots:
    .TAG EntSlot
EntityCount:
    .res 1
EntSlotsUsed:
    .res 1
EntGRAMBank:
    .res 1
EntYOffset:
    .res 1

.segment "CODE"

.proc Draw_All_Entities: near
    LDX #0
@DrawEntsLoop:
    CPX EntityCount
    BEQ @Exit
    PHX
    JSR Draw_An_Entity
    PLX
    INX
    JMP @DrawEntsLoop
@Exit:
    RTS
.endproc

.proc Draw_An_Entity: near
    LDA Entities+Entity::Slot, x
    TAY
    LDA EntSlots+EntSlot::GRAM_Bank, y
    STA EntGRAMBank
    LDA EntSlots+EntSlot::Y_Offset, y
    STA EntYOffset
    LDA EntSlots+EntSlot::Frame_Tables_LByte, y
    STA FrameDataPtr
    LDA EntSlots+EntSlot::Frame_Tables_HByte, y
    STA FrameDataPtr+1
    LDA EntSlots+EntSlot::Frame_Tables_Bank, y
    JSR ShiftROMBank
    LDY Entities+Entity::Frame, x

    LDA (FrameDataPtr), y
    CLC
    ADC Entities+Entity::vX, x
    STA _render_queue_input+Rect::vx
    INC FrameDataPtr+1

    LDA (FrameDataPtr), y
    CLC
    ADC Entities+Entity::vY, x
    CLC
    ADC EntYOffset 
    STA _render_queue_input+Rect::vy
    INC FrameDataPtr+1

    LDA (FrameDataPtr), y
    STA _render_queue_input+Rect::width
    INC FrameDataPtr+1

    LDA (FrameDataPtr), y
    STA _render_queue_input+Rect::height
    INC FrameDataPtr+1

    LDA (FrameDataPtr), y
    STA _render_queue_input+Rect::gx
    INC FrameDataPtr+1

    LDA (FrameDataPtr), y
    STA _render_queue_input+Rect::gy
    INC FrameDataPtr+1

    LDA (FrameDataPtr), y
    STA _render_queue_input+Rect::color

    LDA EntGRAMBank
    STA _render_queue_input+Rect::bank

    LDA #0
    STA _queue_flags_param
    
    JSR _pushRect
    LDA _queue_pending
    BNE @Else
    JSR _NextQueue
@Else:
    RTS
.endproc

.proc Run_Update_Funcs: near
    RTS
.endproc