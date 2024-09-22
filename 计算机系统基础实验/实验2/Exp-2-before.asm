.686P 
.model flat,c
 ExitProcess proto  stdcall :dword
 includelib  kernel32.lib
 printf      proto c :vararg
 includelib  libcmt.lib   
 includelib  legacy_stdio_definitions.lib
 clock		 proto c

.data

SAMID DB 1,2,3,4,5,6,7,8,9,10   ;每组数据的流水号（可从1开始编号）
	SDA DD 1374,248364,3840,3764,0,-1,-2,-3,-4,0
	SDB DD 3144,2342,29743,4382,0,-1,-2,-3,-4,0
	SDC DD 13155,27573,3474,45083,0,-1,-2,-3,-4,0  ;状态信息a,b,c
	SF DD ?                         ;处理结果f，是内存操作数
	LOWF DD 500 DUP(0)
	MIDF DD 500 DUP(0)
	HIGHF DD 500 DUP(0)            ;每个存储区500个空位
	COUNT DB 10
	RECR DD ?
	LOOPCOUNT DD 5000000
lpfmt db "%d",0ah, 0dh, 0
start_t dd 0
end_t dd 0
.stack 200

.code
main proc 
	invoke clock
	mov start_t, eax

; 程序主体
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
	ADD EAX, EBX
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
;
	invoke clock
	mov end_t, eax
	sub eax, start_t
	invoke printf, offset lpFmt, eax
	invoke ExitProcess, 0
main endp
end 