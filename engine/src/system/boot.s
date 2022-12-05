; ---------------------------------------------------------------------------
; boot.s
; ---------------------------------------------------------------------------
;
; Startup code for cc65 (GameTank version)

.export   _init
.import   _main

.export   __STARTUP__ : absolute = 1        ; Mark as startup
.import   __RAM0_START__, __RAM0_SIZE__       ; Linker generated

.import    copydata, zerobss, initlib, donelib

.import ShiftROMBank, Setup_VIA_Port_A, Init_RAM_Page_Numbers, _banksMirror

.include "../include/regs.inc"

.PC02

.include  "zeropage.inc"

; ---------------------------------------------------------------------------
; Place the startup code in a special segment

.segment  "STARTUP"

; ---------------------------------------------------------------------------
; A little light 6502 housekeeping

_init:    LDX     #$FF                 ; Initialize stack pointer to $01FF
          TXS
          CLD                          ; Clear decimal mode

    ldx #0
viaWakeup:
	inx
	bne viaWakeup
    STZ Bank_Flags
    STZ _banksMirror
	
	JSR Init_RAM_Page_Numbers

	JSR Setup_VIA_Port_A
    
    STZ Audio_Rate
    STZ Audio_Reset

	lda #$00
	jsr ShiftROMBank

; ---------------------------------------------------------------------------
; Call main()

          JSR     _main

          BRK