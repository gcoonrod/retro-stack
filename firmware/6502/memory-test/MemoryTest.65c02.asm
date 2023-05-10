.target "65c02"
.format "bin"

PORTB   .equ $6000
PORTA   .equ $6001
DDRB    .equ $6002
DDRA    .equ $6003

; Reset vector
.org $fffc
.word START
.word $0000

; Assembly program for W65C02S microprocessor
; Testing SRAM in the range 0x0000 to 0x3FFF
; No stack initialization or usage

.ORG $8000

via_init:
        LDA #$ff        ; Set all pins to output
        STA DDRA        ; Set DDRA to output
        STA DDRB        ; Set DDRB to output
        LDA #$00        ; Set all pins to low
        STA PORTA
        STA PORTB

START:
        SEI               ; Disable interrupts
        LDX #$00          ; Initialize X register (high byte of address)

ADDR_HI_LOOP:
        LDY #$00          ; Initialize Y register (low byte of address)
ADDR_LO_LOOP:
        STY $00           ; Store Y in temporary location
        LDA #$AA          ; Load test value (0xAA) to A register
        STA ($00),Y       ; Store test value at (X, Y) address
        LDA ($00),Y       ; Load value from (X, Y) address to A register
        CMP #$AA          ; Compare A with the test value (0xAA)
        BNE ERROR         ; If not equal, jump to error handler
        LDA #$55          ; Load alternate test value (0x55) to A register
        STA ($00),Y       ; Store test value at (X, Y) address
        LDA ($00),Y       ; Load value from (X, Y) address to A register
        CMP #$55          ; Compare A with the alternate test value (0x55)
        BNE ERROR         ; If not equal, jump to error handler
        LDY $00           ; Load Y from temporary location
        INY               ; Increment Y (low byte of address)
        BNE ADDR_LO_LOOP  ; If Y not zero, continue low byte loop
        INX               ; Increment X (high byte of address)
        CPX #$40          ; Compare X with 0x40 (0x3FFF high byte + 1)
        BNE ADDR_HI_LOOP  ; If X not equal to 0x40, continue high byte loop
        JMP PRG_DONE      ; Jump to done state

ERROR:
        ; Handle error case here (e.g., toggle a LED or output to a port)
        ; In this example, we just halt the CPU
        LDA #$00          ; Load 0xFF to A register
        STA PORTA
        NOP
        NOP
        NOP
        NOP
        NOP
        JMP ERROR

PRG_DONE:
        ; Handle success case here (e.g., toggle a LED or output to a port)
        ; In this example, we just halt the CPU
        LDA #$FF          ; Load 0x00 to A register
        STA PORTA
        NOP
        NOP
        NOP
        NOP
        NOP
        JMP PRG_DONE

.END

