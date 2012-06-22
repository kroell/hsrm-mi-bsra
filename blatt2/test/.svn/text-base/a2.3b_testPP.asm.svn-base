;; this file tests the
;; push/pop instructs.
main:	
	LDN  stk, R15
	IN   0,  R1
	PUSH R1
	POP  R2
	SUB  R1, R2, R3
	OUT  R2, 0
	HALT
	DS   100
stk:	