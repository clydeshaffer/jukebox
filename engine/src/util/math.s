.export AbsVal

.PC02

.code

AbsVal:
    bmi :+
    rts
:
    clc
    eor #$ff
    adc #1
    rts