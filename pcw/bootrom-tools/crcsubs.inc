

CRC_Init:
;
; Builds the 512-byte look-up table, enter with HL containing
; buffer starting address.
;
	LD	(CRCTBL),HL	; ADDRESS OF THE 512-BYTE LOOK-UP TABLE
	LD	C,0		; TABLE INDEX
;
GLOOP:	EX	DE,HL
	LD	HL,0		; INITIALIZE 'CRC' REGISTER PAIR
	LD	A,C
	PUSH	BC		; SAVE THE INDEX IN 'C' REG.
	LD	B,8
	XOR	H
	LD	H,A
;
LLOOP:	ADD	HL,HL
	JR	NC,LSKIP
	LD	A,10H		; GENERATOR IS x^16 + x^12 + x^5 + x^0 AS
	XOR	H		; RECOMMENDED BY 'CCIT' FOR ASYNCHRONOUS
	LD	H,A		; COMMUNICATIONS
	LD	A,21H
	XOR	L
	LD	L,A
;
LSKIP:	DJNZ	LLOOP
	POP	BC
	EX	DE,HL		; 'DE' PAIR NOW HAS 'CRC', 'HL' POINTING
	LD	(HL),D		; INTO TABLE.  STORE HIGH BYTE OF 'CRC'
	INC	H
	LD	(HL),E		; STORE LOW BYTE
	DEC	H
	INC	HL		; MOVE TO NEXT TABLE ENTRY
	INC	C		; NEXT INDEX
	JR	NZ,GLOOP
	RET

CRCVAL:	defw	2		; Running total
CRCTBL:	defw	2		; Table address
;
;Clear the stored CRC
;
CRC_Clear:
	push	hl
	LD	HL,0
	LD	(CRCVAL),HL
	pop	hl
	RET


; Updates the CRC value.  Finds its value by indexing into the look-up
; table.  Character arrives in the 'A' register.
;
CRC_Update:
	PUSH	hl
	PUSH	de
	push	bc
	LD	DE,(CRCVAL)	; PICK UP PARTIAL REMAINDER
	LD	B,0
	XOR	D
	LD	C,A
	LD	HL,(CRCTBL)	; INDEX INTO THE 512-BYTE 'CRC' TABLE
	ADD	HL,BC		; [JE] See? 16-bit pointer arithmetic
	LD	A,(HL)
	XOR	E
	LD	D,A
	INC	H
	LD	E,(HL)
	LD	(CRCVAL),DE	; STORE THE CURRENT 'CRC' VALUE
	pop	bc
	pop	de
	pop	hl
	RET
;
;Returns the calculated value.
;
CRC_Done:
	LD	HL,(CRCVAL)
	RET
;
;Returns the table address
;
CRC_Table:
	LD	HL,(CRCTBL)
	RET

