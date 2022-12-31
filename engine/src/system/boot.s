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
.import _romBankMirror

.include "regs.inc"

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

    STZ _romBankMirror
	lda #$FF
	jsr ShiftROMBank

; ---------------------------------------------------------------------------
; Set cc65 argument stack pointer

          LDA     #<(__RAM0_START__ + __RAM0_SIZE__)
          STA     sp
          LDA     #>(__RAM0_START__ + __RAM0_SIZE__)
          STA     sp+1

; ---------------------------------------------------------------------------
; Initialize memory storage

          JSR     zerobss              ; Clear BSS segment
          JSR     copydata             ; Initialize DATA segment
          JSR     initlib              ; Run constructors

; ---------------------------------------------------------------------------
; Call main()

          JSR     _main

          BRK