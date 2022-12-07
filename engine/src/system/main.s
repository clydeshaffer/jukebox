.export _main
.import _render_queue_input, _queue_flags_param, _NextQueue, _pushRect, Init_Draw_Queue, _frameflip, _flagsMirror
.import WaitForVSync, Draw_All_Entities, EntSlots, Entities, EntityCount, _queue_pending, Run_Update_Funcs
.import LoadScene
.importzp args
.import InitialSceneHeader

.PC02

.include "../graphics/rect.inc"
.include "regs.inc"
.include "../graphics/colors.inc"
.include "../game/entity.inc"

.segment "CODE"

_main:
    CLI
    JSR Init_Draw_Queue
    STZ _frameflip
    LDA #(VFLAG_VNMI | VFLAG_BLIT_IRQ | VFLAG_COLORFILL | VFLAG_OPAQUE);
    STA _flagsMirror
	STA DMA_Flags

    LDA #<InitialSceneHeader
    STA args
    LDA #>InitialSceneHeader
    STA args+1
    LDA #127
    STA args+2
    JSR LoadScene

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