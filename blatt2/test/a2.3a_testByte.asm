;; this program tests the storb/loadb instructions
main:
	LDN   stk, R15		; initialize stack pointer
	LDN   0, R0
	LDN   203, R1
	IN    0, R2
	STORB R2, R1, R0
	LOADB R1, R0, R3
	OUT   R3, 0
	DS    200
stk:				; mark stack