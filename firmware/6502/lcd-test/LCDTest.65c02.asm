.target "65c02"
.format "bin"
.setting "Debug", true 

; 65C22 VIA Addresses
PORTB       .equ    $6000
PORTA       .equ    $6001
DDRB        .equ    $6002
DDRA        .equ    $6003

; HD44780 LCD Masks
LCD_E       .equ    %10000000
LCD_RW      .equ    %01000000
LCD_RS      .equ    %00100000
LCD_COMM_PORT   .equ    PORTA
LCD_DATA_PORT   .equ    PORTB

; LCD Commands

; LCD Function Set - RS = 0, RW = 0
LCD_COMM_F_SET              .equ    %00100000
LCD_COMM_F_SET_DATA_LEN_8   .equ    %00010000
LCD_COMM_F_SET_DATA_LEN_4   .equ    %00000000
LCD_COMM_F_SET_2_LINES      .equ    %00001000
LCD_COMM_F_SET_1_LINE       .equ    %00000000
LCD_COMM_F_SET_5x10_DOTS    .equ    %00000100
LCD_COMM_F_SET_5x8_DOTS     .equ    %00000000

; LCD Display Control
LCD_COMM_D_CTRL             .equ    %00001000
LCD_COMM_D_CTRL_DISPLAY_ON  .equ    %00000100
LCD_COMM_D_CTRL_DISPLAY_OFF .equ    %00000000
LCD_COMM_D_CTRL_CURSOR_ON   .equ    %00000010
LCD_COMM_D_CTRL_CURSOR_OFF  .equ    %00000000
LCD_COMM_D_CTRL_BLINK_ON    .equ    %00000001
LCD_COMM_D_CTRL_BLINK_OFF   .equ    %00000000

; LCD Entry Mode Set
LCD_COMM_E_MODE             .equ    %00000100
LCD_COMM_E_MODE_INC         .equ    %00000010
LCD_COMM_E_MODE_DEC         .equ    %00000000
LCD_COMM_E_MODE_SHIFT_ON    .equ    %00000001
LCD_COMM_E_MODE_SHIFT_OFF   .equ    %00000000

; LCD Return Home
LCD_COMM_RETURN_HOME        .equ    %00000010

; LCD Clear Display
LCD_COMM_CLEAR_DISPLAY      .equ    %00000001

; LCD Set DDRAM Address
LCD_COMM_SET_DDRAM_ADDR     .equ    %10000000

; LCD Set CGRAM Address
LCD_COMM_SET_CGRAM_ADDR     .equ    %01000000

; LCD Busy Flag - RW = 1
LCD_BUSY_FLAG               .equ    %10000000

; Program Start
.org    $8000       ; Start of ROM
reset:
    ldx #$ff        ; Set X to $ff
    txs             ; Transfer X to Stack Pointer

disable_interrupts:
    sei             ; Disable Interrupts

via_init:
    lda #$ff        ; Configure VIA Ports as Outputs
    sta DDRB        ; Set DDRB to output on all pins
    sta DDRA        ; Set DDRA to output on all pins
    lda #$00        ; Set VIA Ports to $00 (off)
    sta PORTB       ; Set PORTB to $00
    sta PORTA       ; Set PORTA to $00

    ; Initialize LCD
lcd_init:
    jsr lcd_init_by_internal_reset


main_loop:
    ; Print "Hello World!" to LCD
    jsr lcd_print_string
    jsr delay
    lda #(LCD_COMM_CLEAR_DISPLAY)
    jsr lcd_commit_instruction
    jsr delay
    jmp main_loop

; Interrupt Service Routine - NMI
isr_nmi:
    rti

; Interrupt Service Routine - IRQ
isr_irq:
    rti

; LCD Functions
lcd_init_by_internal_reset:
    lda #(LCD_COMM_F_SET | LCD_COMM_F_SET_DATA_LEN_8 | LCD_COMM_F_SET_2_LINES | LCD_COMM_F_SET_5x8_DOTS)
    jsr lcd_commit_instruction
    lda #(LCD_COMM_D_CTRL | LCD_COMM_D_CTRL_DISPLAY_ON | LCD_COMM_D_CTRL_CURSOR_ON | LCD_COMM_D_CTRL_BLINK_ON)
    jsr lcd_commit_instruction
    lda #(LCD_COMM_E_MODE | LCD_COMM_E_MODE_INC | LCD_COMM_E_MODE_SHIFT_OFF)
    jsr lcd_commit_instruction
    lda #(LCD_COMM_CLEAR_DISPLAY)
    jsr lcd_commit_instruction
    rts

lcd_wait_for_bf:
    pha
    lda #$00        ; Set DDRB to input on all pins
    sta DDRB
@lcd_busy:
    lda #LCD_RW
    sta LCD_COMM_PORT
    lda #(LCD_RW | LCD_E)
    sta LCD_COMM_PORT
    lda LCD_DATA_PORT
    and #%10000000
    bne @lcd_busy

    lda #LCD_RW
    sta LCD_COMM_PORT
    lda #$ff        ; Set DDRB to output on all pins
    sta DDRB
    pla
    rts

lcd_commit_instruction:
    jsr lcd_wait_for_bf
    sta LCD_DATA_PORT
    lda #$00
    sta LCD_COMM_PORT
    lda #(LCD_E)
    sta LCD_COMM_PORT
    lda #$00
    sta LCD_COMM_PORT
    rts

lcd_print_char:
    jsr lcd_wait_for_bf
    sta LCD_DATA_PORT
    lda #(LCD_RS)
    sta LCD_COMM_PORT
    lda #(LCD_RS | LCD_E)
    sta LCD_COMM_PORT
    lda #(LCD_RS)
    sta LCD_COMM_PORT
    rts

lcd_print_string:
    pha
    phx
    ldx #0
@lcd_print_string_loop:
    lda _HelloWorld, x
    beq @lcd_print_string_done
    jsr lcd_print_char
    inx
    jmp @lcd_print_string_loop
@lcd_print_string_done:
    plx
    pla
    rts

; Utility Functions
; Delay
delay:
    phy
    ldy #$02
@delay_loop:
    dey
    bne @delay_loop
    ply
    rts

  


; Pad with NOPs
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop

; Data
_HelloWorld .asciiz "Hello, World!"

; 65C02 Reset Vector
.org    $fffa
.word   isr_nmi
.word   reset
.word   isr_irq