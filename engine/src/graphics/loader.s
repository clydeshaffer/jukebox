
.PC02

.import _inflatemem, ShiftROMBank, _banksMirror
.importzp sp, args, tmp_ptr
.export LoadSpriteSheet

.include "../game/scene.inc"
.include "../system/regs.inc"

;(args) points to a SpriteLoadCommand
.proc LoadSpriteSheet: near

    LDY #SpriteLoadCommand::Y_Offset
    LDA (args),y
    TAX ; put Y_Offset in X register

    PHP
    SEI
    LDA #1
    STA DMA_Flags
    STA DMA_WIDTH
    STA DMA_HEIGHT
    STZ DMA_VX
    STZ DMA_VY
    STZ DMA_GX
    TXA
    STA DMA_GY
    LDA #1
    STA DMA_Start
    PLP
    STZ DMA_Flags

    
    LDY #0
    LDA #0
    STA (sp),y ;outputPointer
    INY
    TXA ;get Y_Offset back in accumulator
    AND #$7F
    CLC
    LSR ;divide by 2
    CLC
    ADC #$40
    STA (sp),y ;outputPointer+1

    LDY #SpriteLoadCommand::SheetPtr
    LDA (args), y
    STA tmp_ptr
    INY
    LDA (args), y
    STA tmp_ptr+1

    LDA _banksMirror
    AND #$F8
    STA _banksMirror
    LDY #SpriteLoadCommand::GRAMBank
    LDA (args), y
    AND #$07
    ORA _banksMirror
    STA _banksMirror
    STA Bank_Flags

    LDY #SpriteLoadCommand::SheetBank
    LDA (args), y
    JSR ShiftROMBank
    LDA tmp_ptr
    LDX tmp_ptr+1

    JSR _inflatemem
    RTS
.endproc