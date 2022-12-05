.export _main
.import _render_queue_input, _queue_flags_param, _NextQueue, _pushRect, Init_Draw_Queue, _frameflip, _flagsMirror

.PC02

.include "../include/rect.inc"
.include "../include/regs.inc"
.include "../include/colors.inc"

.segment "CODE"
_main:
    CLI
    JSR Init_Draw_Queue
    STZ _frameflip
    LDA #(VFLAG_VNMI | VFLAG_BLIT_IRQ);
    STA _flagsMirror
	STA DMA_Flags
    WAI
    
    LDA #16
    STA _render_queue_input+Rect::vx
    STA _render_queue_input+Rect::vy
    STA _render_queue_input+Rect::width
    STA _render_queue_input+Rect::height
    STA _render_queue_input+Rect::gx
    STA _render_queue_input+Rect::gy
    LDA #(HUE_BLUE | SAT_FULL | 4)
	EOR #$FF
    STA _render_queue_input+Rect::color
    LDA #0
    STA _render_queue_input+Rect::bank
    LDA #(VFLAG_COLORFILL | VFLAG_OPAQUE)
    STA _queue_flags_param
    JSR _pushRect

    LDA #32
    STA _render_queue_input+Rect::vx
    STA _render_queue_input+Rect::vy
    STA _render_queue_input+Rect::width
    STA _render_queue_input+Rect::height
    STA _render_queue_input+Rect::gx
    STA _render_queue_input+Rect::gy
    LDA #(HUE_GREEN | SAT_FULL | 4)
	EOR #$FF
    STA _render_queue_input+Rect::color
    LDA #0
    STA _render_queue_input+Rect::bank
    LDA #(VFLAG_COLORFILL | VFLAG_OPAQUE)
    STA _queue_flags_param
    JSR _pushRect

    LDA #64
    STA _render_queue_input+Rect::vx
    STA _render_queue_input+Rect::vy
    STA _render_queue_input+Rect::width
    STA _render_queue_input+Rect::height
    STA _render_queue_input+Rect::gx
    STA _render_queue_input+Rect::gy
    LDA #(HUE_RED | SAT_FULL | 4)
	EOR #$FF
    STA _render_queue_input+Rect::color
    LDA #0
    STA _render_queue_input+Rect::bank
    LDA #(VFLAG_COLORFILL | VFLAG_OPAQUE)
    STA _queue_flags_param
    JSR _pushRect
    
    JSR _NextQueue
Forever:
    JMP Forever