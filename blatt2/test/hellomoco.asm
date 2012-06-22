; Kommentar
    LDN    stk,R15		; Stackpointer initialisieren
    LDN    hello,R0
    JSR    output_string
    LDN    quest,R0
    JSR    output_string
    LDN    buffer,R0
    JSR    input_string
    LDN    hello2,R0
    JSR    output_string
    LDN    buffer,R0
    JSR    output_string
    LDN    hello3,R0
    JSR    output_string
    HALT


input_string:
    PUSH  R1
    PUSH  R14
    LDN   0,R1
input_loop:
    IN    1,R14     ; Ein Zeichen einlesen
    STORB R14,R0,R1 ; Zeichen abspeichern
    ADDN  R1,1,R1   ; Zähler inkrementieren
    SUBN  R14,0,R14 ; Null eingelesen?
    JMPNZ input_loop
    POP   R14
    POP   R1
    RET

output_string:
    PUSH  R1
    PUSH  R14
    LDN  0,R1
output_loop:
    LOADB R0,R1,R14 ; Zeichen holen
    SUBN  R14,0,R14 ; Null eingelesen?
    JMPZ  output_end
    OUT   R14,1     ; Ein Zeichen ausgeben
    ADDN  R1,1,R1   ; Zähler inkrementieren
    JMP   output_loop ; weiter
output_end:
    POP   R14
    POP   R1
    RET

hello:    DB "Hello World! This is MoCo!\n"
quest:    DB "Please enter your Name:"
buffer:   DS 100
hello2:   DB "Hello "
hello3:   DB "! How are you today?\n"
	  DS 100
stk:
