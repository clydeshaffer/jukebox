.PC02

.export ReadInputs
.export inputsP1A, inputsP1B, inputsP2A, inputsP2B
.export inputsP1A_old, inputsP1B_old, inputsP2A_old, inputsP2B_old

.include "regs.inc"
.include "via.inc"

.segment    "BSS"
inputsP1A:
	.res	1
inputsP1B:
	.res	1
inputsP2A:
    .res    1
inputsP2B:
	.res	1
inputsP1A_old:
	.res	1
inputsP1B_old:
	.res	1
inputsP2A_old:
    .res    1
inputsP2B_old:
	.res	1

.segment    "CODE"

ReadInputs:
    LDA inputsP1A
    STA inputsP1A_old
    LDA inputsP1B
    STA inputsP1B_old
    LDA inputsP2A
    STA inputsP2B_old
    LDA inputsP2A
    STA inputsP2B_old
    LDA InputPort2
    LDA InputPort1
    EOR #$FF
    STA inputsP1A
    LDA InputPort1
    EOR #$FF
    STA inputsP1B
    LDA InputPort2
    EOR #$FF
    STA inputsP2A
    LDA InputPort2
    EOR #$FF
    STA inputsP2B
    RTS