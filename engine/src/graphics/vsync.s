.export _frameflag, _nmi_int_set_frameflag, WaitForVSync

.segment "BSS"
_frameflag:
    .res	1,$00

.segment  "CODE"
; ---------------------------------------------------------------------------
; Non-maskable interrupt (NMI) service routine

_nmi_int_set_frameflag:
        PHA
        LDA $1FFF
        BNE nmi_done
        STZ _frameflag
nmi_done:
        PLA
        RTI



.proc WaitForVSync: near
        LDA #1
        STA _frameflag
@Loop:
        LDA _frameflag
        BNE @Loop
        RTS
.endproc