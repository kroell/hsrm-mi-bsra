;; this file tests the
;; jsr/ret instructs.
main:
        LDN  stk, R15
	IN   0, R0
	JSR  proc
	OUT  R0, 0
	HALT

proc:
	LDN  3, R1
	ADD  R1,  R0,R0
	RET

	DS   200
stk: