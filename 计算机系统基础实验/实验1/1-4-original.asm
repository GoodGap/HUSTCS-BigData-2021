.386
STACK 	SEGMENT USE16 STACK
             	DB 200 DUP(0)
STACK 	ENDS

DATA	SEGMENT USE16
	SAMID DB 1,2,3,4,5,6,7,8,9,10   ;每组数据的流水号（可从1开始编号）
	SDA DD 1,2,3,4,0,-1,-2,-3,-4,0
	SDB DD 1,2,3,4,0,-1,-2,-3,-4,0
	SDC DD 1,2,3,4,0,-1,-2,-3,-4,0  ;状态信息a,b,c
	SF DD ?                         ;处理结果f，是内存操作数
	LOWF DD 1000 DUP(0)
	MIDF DD 1000 DUP(0)
	HIGHF DD 1000 DUP(0)            ;每个存储区1000个空位
	COUNT DB 10
	RECR DD ?
DATA	ENDS
CODE	SEGMENT USE16
	ASSUME  CS:CODE, SS:STACK, DS:DATA, ES:DATA
START:	MOV AX, DATA
	MOV DS, AX
LOOOP:
	MOV ESI, 0                      ;存储变址（循环次数）
	MOV RECR, 0                      ;注意不能用ESP
	MOV EBP, 0
	MOV EDI, 0                      ;分别存储三个数据段的变址
BEGINNING:
	MOV EAX, [4*ESI+SDA]
	MOV EBX, [4*ESI+SDB]
	MOV ECX, [4*ESI+SDC]
	IMUL EAX, 5
	ADD EAX, EBX                ;一定注意两种寄存器种类要相同，否则报错
	SUB EAX, ECX
	ADD EAX, 100
	CDQ
	MOV ECX, 128
	IDIV ECX                        ;结果的商在EAX，余数在EDX
	MOV SF, EAX                     ;将结果EAX保存在SF中
	MOV EAX, [4*ESI+SDA]
	MOV EBX, [4*ESI+SDB]
	MOV ECX, [4*ESI+SDC]
	MOV DH, BYTE PTR[ESI+SAMID]     ;重新将该组数据转移到EAX~ECX和DH中
	CMP SF, 100
	JL L                            ;若SF<100，转到L
	JG G                            ;若SF>100，转到G
	JE M                            ;若SF=100，转到M
L:	MOV ECX, RECR
	MOV BYTE PTR[ECX+LOWF], DH
	MOV [ECX+LOWF+1], EAX
	MOV [ECX+LOWF+5], EBX
	MOV EBX, [4*ESI+SDC]
	MOV [ECX+LOWF+9], EBX
	ADD RECR, 13
	JMP OUTT
G:	MOV BYTE PTR DS:[EBP+HIGHF], DH
	MOV DS:[EBP+HIGHF+1], EAX
	MOV DS:[EBP+HIGHF+5], EBX
	MOV DS:[EBP+HIGHF+9], ECX
	ADD EBP, 13
	JMP OUTT
M:	MOV BYTE PTR[EDI+MIDF], DH
	MOV [EDI+MIDF+1], EAX
	MOV [EDI+MIDF+5], EBX
	MOV [EDI+MIDF+9], ECX
	ADD EDI, 13
	JMP OUTT
OUTT:	INC ESI
	DEC COUNT
	JNZ BEGINNING
	DEC LOOPCOUNT
	JNZ LOOOP
	MOV AH, 4CH
	INT  21H
CODE	ENDS
	END  START