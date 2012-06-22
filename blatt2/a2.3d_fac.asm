main:
    LDN     stk,R15     ; Stack initialisieren
    IN      0,R0        ; Zahl in R0 einlesen
    JSR     fac         ; Fakultaet berechnen
    OUT     R0,0        ; Ausgeben
    HALT                ; beenden
    
fac:
    ADDN R0,0,R0        ; R0 == 0 ?
    JMPZ end            ; Dann -> Return
    PUSH R0             ; Sonst R0 auf Stack
    SUBN R0,1,R0        ; Dekrementieren
    JSR fac             ; Neuer aufruf
    POP R1              ; Danach Alten Wert vom Stack holen
    MUL R0,R1,R0        ; Mit aktuellem R0 multiplizieren
    RET                 ; zur端ck zum Aufrufer

end:
    LDN 1,R0        ; R0 auf 1 setzen
    RET             ; zur端ckspringen zum Aufrufer
    
DS 100                  ; Stack anlegen
stk:                    ; Marker f端r Stack