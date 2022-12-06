.PC02

.export ShiftROMBank, Setup_VIA_Port_A, _SwitchRomBank, _banksMirror, Init_RAM_Page_Numbers

.include "regs.inc"
.include "via.inc"

.segment "RAMPAGEMARK"
_currentRamBank:
    .res    1,$00

.segment    "BSS"
_romBankMirror:
	.res	1,$00
_banksMirror:
    .res    1,$00
_bankFlip:
	.res	1,$00

.segment    "CODE"

Init_RAM_Page_Numbers:
    CLC
    LDA #0
RAM_PAGE_LOOP:
    STA Bank_Flags
    STA _currentRamBank
    ADC #%01000000
    BNE RAM_PAGE_LOOP
    STZ Bank_Flags
    STZ _banksMirror
    RTS

Setup_VIA_Port_A:
    LDA #%00000111
	STA VIA::DDRA
    LDA #$FF
    STA VIA::ORA
    RTS

ShiftROMBank:
	CMP _romBankMirror
	BNE @continue
	RTS
@continue:
    STA _romBankMirror
	STA $0
	LDA #$FF
	STA VIA::ORA

	LDA $0
	ROR
	ROR
	ROR
	ROR
	ROR
	AND #2
	ORA #%00000100
	STA VIA::ORA
	ORA #1
	STA VIA::ORA

	LDA $0
	ROR
	ROR
	ROR
	ROR
	AND #2
	ORA #%00000100
	STA VIA::ORA
	ORA #1
	STA VIA::ORA

	LDA $0
	ROR
	ROR
	ROR
	AND #2
	ORA #%00000100
	STA VIA::ORA
	ORA #1
	STA VIA::ORA

	LDA $0
	ROR
	ROR
	AND #2
	ORA #%00000100
	STA VIA::ORA
	ORA #1
	STA VIA::ORA
	
	LDA $0
	ROR
	AND #2
	ORA #%00000100
	STA VIA::ORA
	ORA #1
	STA VIA::ORA

	LDA $0
	AND #2
	ORA #%00000100
	STA VIA::ORA
	ORA #1
	STA VIA::ORA

	LDA $0
	ROL
	AND #2
	STA VIA::ORA
	ORA #1
	STA VIA::ORA

	ORA #4
	STA VIA::ORA

	RTS

.proc _SwitchRomBank: near
	PHP
	SEI
	JSR ShiftROMBank
	PLP
	RTS
.endproc