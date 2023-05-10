.target "65c02"
.format "bin"

; 65C22 VIA Addresses
PORTB   .equ   $6000
PORTA   .equ   $6001
DDRB    .equ   $6002
DDRA    .equ   $6003

; 65C02 Reset Vector
.org    $fffc
.word   start

; 65C02 NMI Vector
;.org    $fffa
;.word   nmi

; 65C02 IRQ Vector
;.org    $fff8
;.word   irq

; Program Start
.org    $8000       ; Start of ROM
start:
    sei             ; Disable Interrupts
    cld             ; Clear Decimal Mode
    ldx #$ff        ; Set X to $ff
    txs             ; Transfer X to Stack Pointer

via_init:
    lda #$ff        ; Configure VIA Ports as Outputs
    sta DDRB        ; Set DDRB to $ff
    sta DDRA        ; Set DDRA to $ff
    lda #$00        ; Set VIA Ports to $00 (off)
    sta PORTB       ; Set PORTB to $00
    sta PORTA       ; Set PORTA to $00
    cli             ; Enable Interrupts

; Main Loop
; 8-bit Counter in RAM and outputing to port A
counter:
    ldx #$00        ; Set X to $00
counter_loop:
    stx PORTA       ; Output A to PORTA
    lda #$05        ; Set A to $05
    jsr wait_for_delay  ; Wait for 10 cycles
    inx             ; Increment value at $00
    cpx #$ff        ; Compare X to $ff
    bne counter_loop    ; If not equal, loop
    jmp counter     ; Jump to counter

; Wait For Subroutine - variable delay using input in A
wait_for_delay:
    sei             ; Disable Interrupts
    phx             ; Push X to Stack    
wait_loop:
    nop
    nop
    dec             ; Decrement A
    bne wait_loop   ; If not zero, loop
    plx             ; Pull X from Stack
    cli             ; Enable Interrupts
    rts             ; Return from Subroutine

.end