; ---------------------------------------------------------------------------
; renderqueue.s
; ---------------------------------------------------------------------------

.import   _frameflag, _flagsMirror, _frameflip, _banksMirror

.export  _NextQueue, _irq_int_check_render_queue, _pushRect, _render_queue_input, _queue_flags_param, Init_Draw_Queue
.export _queue_pending
.pc02

.include "../system/regs.inc"
.include "rect.inc"

.segment "BSS"

_queue_pending:
    .res 1
_queue_start:
    .res 1
_queue_end:
    .res 1
_queue_count:
    .res 1

.segment "DRAWQ"
.align 256
Q_DMA_Flags:
    .res 256
Q_VX:
    .res 256
Q_VY:
    .res 256
Q_GX:
    .res 256
Q_GY:
    .res 256
Q_WIDTH:
    .res 256
Q_HEIGHT:
    .res 256
Q_Color:
    .res 256
Q_Bank_Flags:
    .res 256


.segment "ACP_MISC"
_queue_flags_param:
    .res 1
_render_queue_input:
    .tag Rect

.segment  "CODE"

.proc   Init_Draw_Queue: near
    STZ _queue_start
    STZ _queue_end
    STZ _queue_pending
    STZ _queue_count
    RTS
.endproc

.proc	_pushRect: near
    ldy     _queue_end
    inc     _queue_end
    inc     _queue_count

    lda     _banksMirror ;get current banks reg
    tax
    ora     #$40 ;make BANK1 version of same
    sei
    sta     Bank_Flags ;switch to BANK1


    lda     _render_queue_input+Rect::bank
    sta     Q_Bank_Flags, y

    lda     _render_queue_input+Rect::vx
    sta     Q_VX, y

    lda     _render_queue_input+Rect::vy
    sta     Q_VY, y

    lda     _render_queue_input+Rect::gx
    sta     Q_GX, y

    lda     _render_queue_input+Rect::gy
    sta     Q_GY, y

    lda     _render_queue_input+Rect::width
    sta     Q_WIDTH, y

    lda     _render_queue_input+Rect::height
    sta     Q_HEIGHT, y

    lda     _render_queue_input+Rect::color
    sta     Q_Color, y

    lda     _queue_flags_param ;flags
    sta     Q_DMA_Flags, y

    ;now finally switch back to BANK0
    stx     Bank_Flags
    cli
    rts

.endproc

.proc _NextQueue: near
        ;determined that there is more to process
        ;so load next set of parameters
        STZ Bank_Flags
        LDA #1
        STA _queue_pending

        ;make sure DMA mode is set to input these params
        LDA _flagsMirror
        ORA #$01
        STA DMA_Flags

        LDY _queue_start

        LDA #$40
        STA Bank_Flags

        LDA Q_VX, y
        STA DMA_VX

        LDA Q_VY, y
        STA DMA_VY

        LDA Q_GX, y
        STA DMA_GX

        LDA Q_GY, y
        STA DMA_GY

        LDA Q_WIDTH, y
        STA DMA_WIDTH

        LDA Q_HEIGHT, y
        STA DMA_HEIGHT

        LDA Q_Color, y
        STA DMA_Color

        LDX Q_DMA_Flags, y
        LDA Q_Bank_Flags, y

        AND #$3F
        STA Bank_Flags
        STA _banksMirror

        TXA ;retrieve Q_DMA_Flags value
        ORA _frameflip
        ORA #$45 ; DMA_ENABLE | DMA_NMI | DMA_IRQ
        STA _flagsMirror
        STA DMA_Flags

        INC _queue_start

        LDA #1
        STA DMA_Start ;START

        DEC _queue_count
        RTS
.endproc

_irq_int_check_render_queue:
        PHX                    ; Save X register contents to stack
        PHA
        PHY

        ;make sure DMA isn't running then compare head and tail of queue
        ;to determine whether there is more to process
        STZ DMA_Start
        STZ _queue_pending
        LDA _queue_start
        CMP _queue_end
        BEQ finish_irq
        JSR _NextQueue
finish_irq:
        PLY
        PLA                    ; Restore accumulator contents
        PLX                    ; Restore X register contents
        RTI                    ; Return from all IRQ interrupts
