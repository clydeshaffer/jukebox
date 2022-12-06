.export _main
.import _render_queue_input, _queue_flags_param, _NextQueue, _pushRect, Init_Draw_Queue, _frameflip, _flagsMirror
.import WaitForVSync, Draw_All_Entities, EntSlots, Entities, EntityCount, _queue_pending, Run_Update_Funcs

.PC02

.include "../graphics/rect.inc"
.include "regs.inc"
.include "../graphics/colors.inc"
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

.segment "CODE"

_main:
    CLI
    JSR Init_Draw_Queue
    STZ _frameflip
    LDA #(VFLAG_VNMI | VFLAG_BLIT_IRQ | VFLAG_COLORFILL | VFLAG_OPAQUE);
    STA _flagsMirror
	STA DMA_Flags

    ;hardcoded test Entity
    LDA #0
    STA Entities+Entity::Frame
    STA Entities+Entity::Slot
    STA Entities+Entity::State
    LDA #10
    STA Entities+Entity::vX
    STA Entities+Entity::vY
    LDA #100
    STA Entities+Entity::HP

    ;hardcoded test Entity
    LDA #0
    STA Entities+(Entity::Frame)+1
    LDA #1
    STA Entities+(Entity::Slot)+1
    STA Entities+(Entity::State)+1
    LDA #30
    STA Entities+(Entity::vX)+1
    STA Entities+(Entity::vY)+1
    LDA #100
    STA Entities+(Entity::HP)+1

    LDA #2
    STA EntityCount

    ;hardcoded test Entity Slot
    LDA #<TestFrameData
    STA EntSlots+EntSlot::Frame_Tables_LByte
    LDA #>TestFrameData
    STA EntSlots+EntSlot::Frame_Tables_HByte
    LDA #127
    STA EntSlots+EntSlot::Frame_Tables_Bank
    LDA #<TestUpdateFunc_MoveRight
    STA EntSlots+EntSlot::Updater_LByte
    LDA #>TestUpdateFunc_MoveRight
    STA EntSlots+EntSlot::Updater_HByte
    LDA #127
    STA EntSlots+EntSlot::Updater_Bank
    LDA #0
    STA EntSlots+EntSlot::GRAM_Bank
    STA EntSlots+EntSlot::Y_Offset
    STA EntSlots+EntSlot::Type

    ;hardcoded test Entity Slot 2
    LDA #<TestFrameData
    STA EntSlots+EntSlot::Frame_Tables_LByte+1
    LDA #>TestFrameData
    STA EntSlots+EntSlot::Frame_Tables_HByte+1
    LDA #127
    STA EntSlots+EntSlot::Frame_Tables_Bank+1
    LDA #<TestUpdateFunc_MoveDown
    STA EntSlots+EntSlot::Updater_LByte+1
    LDA #>TestUpdateFunc_MoveDown
    STA EntSlots+EntSlot::Updater_HByte+1
    LDA #127
    STA EntSlots+EntSlot::Updater_Bank+1
    LDA #0
    STA EntSlots+EntSlot::GRAM_Bank+1
    STA EntSlots+EntSlot::Y_Offset+1
    STA EntSlots+EntSlot::Type+1

    Forever:
    JSR ClearScreen
    JSR Draw_All_Entities
    JSR WaitForVSync

    JSR Run_Update_Funcs

    JMP Forever

ClearScreen:
    ;Draw background (to be replaced by tilemap process)
    LDA #0
    STA _render_queue_input+Rect::vx
    STA _render_queue_input+Rect::vy
    LDA #127
    STA _render_queue_input+Rect::width
    STA _render_queue_input+Rect::height
    LDA #0
    STA _render_queue_input+Rect::gx
    STA _render_queue_input+Rect::gy
    LDA #(SAT_NONE | 2)
	EOR #$FF
    STA _render_queue_input+Rect::color
    LDA #0
    STA _render_queue_input+Rect::bank
    LDA #(VFLAG_COLORFILL | VFLAG_OPAQUE)
    STA _queue_flags_param
    JSR _pushRect

    LDA _queue_pending
    BNE @Else
    JSR _NextQueue
@Else:
    RTS