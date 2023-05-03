.target "65c02"
.format "bin"

.org $fffc
.word start
.word $0000

FIB_START   .equ $1000   ; Starting address of Fibonacci sequence storage
FIB_MAX     .equ 255     ; Maximum value for an 8-bit unsigned integer
FIB_NUM     .equ 10      ; Number of Fibonacci numbers to generate


.org     $8000   ; Set the start address of the program
start:
    ldx #$ff    ; Initialize the Stack Pointer
    txs

first_fib:
    lda #$00    ; Initialize the first Fibonacci number
    sta FIB_START
    lda #$01   ; Initialize the second Fibonacci number
    sta FIB_START+1

calculate_next_fib:
