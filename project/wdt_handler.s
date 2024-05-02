.text
    .balign 2

    .global WDT
    ;; WDT uses interrupt #11
    ;; flags: "a"=allocatable, "x"=executable
    .section __interrupt_vector_11,"ax"
    .word WDT ; entry for vector table

    .text

    .extern redrawScreen
    .extern wdt_c_handler

WDT:
    ; start of prologue
    PUSH    R15
    PUSH    R14
    PUSH    R13
    PUSH    R12
    PUSH    R11
    PUSH    R10
    PUSH    R9
    PUSH    R8
    PUSH    R7
    PUSH    R6
    PUSH    R5
    PUSH    R4
    ; end of prologue
    
    CALL    #wdt_c_handler

    ; start of epilogue
    POP     R4
    POP     R5
    POP     R6
    POP     R7
    POP     R8
    POP     R9
    POP     R10
    POP     R11
    POP     R12
    POP     R13
    POP     R14
    POP     R15

    MOV   #redrawScreen, R12 ; Load address of redrawScreen into R12
    MOV   @R12, R13           ; Load value of redrawScreen into R13
    CMP   #0, R13             ; Compare value of redrawScreen with 0
    JZ      dont_wake           ; Jump to dont_wake if redrawScreen is 0

    AND     #0xFFEF, R1         ; Clear CPU off in saved SR

dont_wake:
    RETI
