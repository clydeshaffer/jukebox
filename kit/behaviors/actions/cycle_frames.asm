;PARAMS START
LOWER = 0
UPPER = 8
;PARAMS END

.include "engine.inc"

.export UPDATE_START

UPDATE_START:
    BNE :+
    RTS
    :

    LDA ENTITIES::Frame, x       ; Load the current value at ENTITIES::Frame, x into the accumulator
    CLC             ; Clear the carry flag
    ADC #1          ; Add 1 to the accumulator
    CMP #UPPER       ; Compare the accumulator to the constant value UPPER
    BCC CHECK_LOWER ; If the accumulator is less than or equal to UPPER, check if it is less than LOWER
    LDA #LOWER       ; If the accumulator is greater than UPPER, load the constant value LOWER into the accumulator
    JMP END_ROUTINE ; Store the updated value (LOWER or the original value) back into ENTITIES::Frame, x and return from the subroutine
CHECK_LOWER:
    CMP #LOWER       ; Compare the accumulator to the constant value LOWER
    BCS END_ROUTINE ; If the accumulator is greater than or equal to LOWER, store the updated value back into ENTITIES::Frame, x and return from the subroutine
    LDA #LOWER       ; If the accumulator is less than LOWER, load the constant value LOWER into the accumulator
END_ROUTINE:
    STA ENTITIES::Frame, x       ; Store the updated value (LOWER or the original value) back into ENTITIES::Frame, x
    RTS             ; Return from the subroutine