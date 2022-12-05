.export _frameflag, _nmi_int_set_frameflag

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