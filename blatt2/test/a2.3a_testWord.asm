;; this file tests the store/load instructs.
main:
	LDN   stk, R15		; initialize stack pointer
	LDN   200, R1
	IN    0, R2
	STOR  R2, R1
	LOAD  R1, R3
	OUT   R3, 0
	DS    200
stk:				; mark stack