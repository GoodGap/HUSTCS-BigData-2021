.686P
.model flat,c
 ExitProcess proto  stdcall :dword
 includelib  kernel32.lib
 printf      proto c :vararg
 scanf       proto c :vararg
 strcmp		 proto c :dword, :dword
 includelib  libcmt.lib
 includelib  legacy_stdio_definitions.lib
 getf		 proto :dword
 externdef SF:dword, SDA:dword, SDB:dword, SDC:dword

.data
SAMID DB 1,2,3,4,5,6,7,8,9,10   ;每组数据的流水号（可从1开始编号）
	SDA DD 2540,2540,2540,4,0,-1,-2,-3,-4,0
	SDB DD 1,2,3,4,0,-1,-2,-3,-4,0
	SDC DD 1,2,3,4,0,-1,-2,-3,-4,0  ;状态信息a,b,c
	SF DD ?                         ;处理结果f，是内存操作数
	LOWF DD 300 DUP(0)
	MIDF DD 300 DUP(0)
	HIGHF DD 300 DUP(0)            ;每个存储区500个空位
	POS DD 0
	POS1 DD 0
	POS2 DD 0						;分别存储三个区域的基址
	HINT DB 'Input username:', 0AH, 0
	HINT1 DB 'Input password:', 0AH, 0
	HINT2 DB 'Login failed!', 0AH, 0
	HINT3 DB 'Login succeeded!', 0AH, 0
	USERNAME DB 'ljp', 0
	PASSWORD DD 123456
	USERNAMEREAD DD ?, 0
	PASSWORDREAD DD ?, 0
	SHOWHINT DB 'MIDF存储区中的各组数据：', 0AH, 0
	SHOWHINT1 DB '--', 0AH, 0
	NEWLINE DB 0AH, 0
	PARTITION DB ', ', 0
	TEMP DB ?
	TEMP1 DD ?
	CHOICEHINT DB '请选择操作（按R键重新执行，按Q键退出）：', 0AH, 0
	CHOICE DB 'R', 0
	CHOICEREAD DD ?, 0
lpfmt db "%d",0ah, 0dh, 0
lpfmt1 db "%s", 0
lpfmt2 db "%d", 0
lpfmt3 db "%d", 0
lpfmt4 db "%d", 0

.stack 200

.code
login proc						;登录子程序，定义并使用局部变量
	local t:dword
	MOV t, 3                    ;最多三次
	MOV ECX, 0                  ;标记是否输入正确
logintry:
	invoke printf, offset HINT
	invoke scanf, offset lpfmt1, offset USERNAMEREAD
	invoke printf, offset HINT1
	invoke scanf, offset lpfmt2, offset PASSWORDREAD
	invoke strcmp, offset USERNAME, offset USERNAMEREAD
	CMP EAX, 0
	JNZ loginfail
	MOV EAX, PASSWORDREAD
	CMP PASSWORD, EAX
	JNZ loginfail
	JMP loginsucceed
loginfail:
	DEC t
	CMP t, 0
	JZ loginout
	JMP logintry
loginsucceed:
	MOV ECX, 1
loginout:
	RET
login endp

copyf proc
	MOV EAX, [4*ESI+SDA]
	MOV EBX, [4*ESI+SDB]
	MOV ECX, [4*ESI+SDC]
	MOV DH, BYTE PTR[ESI+SAMID]     ;重新将该组数据转移到EAX~ECX和DH中
	CMP SF, 100
	JE M                            ;若SF=100，转到M
	JG G                            ;若SF>100，转到G
	MOV ECX, POS                    ;若SF<100，执行以下部分
	MOV BYTE PTR[ECX+LOWF], DH
	MOV [ECX+LOWF+1], EAX
	MOV [ECX+LOWF+5], EBX
	MOV EBX, [4*ESI+SDC]
	MOV [ECX+LOWF+9], EBX
	ADD POS, 13
	JMP copyout
G:	MOV ECX, POS2
	MOV BYTE PTR[ECX+HIGHF], DH
	MOV [ECX+HIGHF+1], EAX
	MOV [ECX+HIGHF+5], EBX
	MOV EBX, [4*ESI+SDC]
	MOV [ECX+HIGHF+9], EBX
	ADD POS2, 13
	JMP copyout
M:	MOV ECX, POS1
	MOV BYTE PTR[ECX+MIDF], DH
	MOV [ECX+MIDF+1], EAX
	MOV [ECX+MIDF+5], EBX
	MOV EBX, [4*ESI+SDC]
	MOV [ECX+MIDF+9], EBX
	ADD POS1, 13
	JMP copyout
copyout:
	RET
copyf endp

showmid proc
	local targetpos:dword
	invoke printf, offset SHOWHINT
	CMP POS1, 0
	JZ showmidout
	MOV targetpos, 0					;存变址
	MOV EDI, POS1						;为定值
	LEA EBX, MIDF						;EBX存偏移地址
showmidloop:
	MOV DH, BYTE PTR[EBX]
	MOV TEMP, DH
	invoke printf, offset lpfmt3, TEMP
	invoke printf, offset PARTITION
	MOV EDX, [EBX+1]
	MOV TEMP1, EDX
	invoke printf, offset lpfmt4, TEMP1
	invoke printf, offset PARTITION
	MOV EDX, [EBX+5]
	MOV TEMP1, EDX
	invoke printf, offset lpfmt4, TEMP1
	invoke printf, offset PARTITION
	MOV EDX, [EBX+9]
	MOV TEMP1, EDX
	invoke printf, offset lpfmt4, TEMP1
	invoke printf, offset NEWLINE
	ADD EBX, 13
	ADD targetpos, 13
	CMP targetpos, EDI
	JNZ showmidloop
showmidout:
	invoke printf, offset SHOWHINT1
	RET
showmid endp

finalchoice proc
	MOV EBX, 0
	invoke printf, offset CHOICEHINT
	invoke scanf, offset lpfmt1, offset CHOICEREAD
	invoke strcmp, offset CHOICE, offset CHOICEREAD
	CMP EAX, 0									;重新计算f
	JZ choiceend
	JMP choiceend1
choiceend:
	MOV EBX, 1
	MOV ESI, 0
	MOV POS, 0
	MOV POS1, 0
	MOV POS2, 0
choiceend1:
	RET
finalchoice endp

main proc
; 程序主体
	MOV POS, 0
	MOV POS1, 0
	MOV POS2, 0
	call login
	CMP ECX, 1
	JNZ mainexit1					;登录失败
	invoke printf, offset HINT3
	MOV ESI, 0                      ;存储变址（循环次数）
mainfunc:
	invoke getf, ESI
	call copyf
	INC ESI
	CMP ESI, 10
	JNZ mainfunc
	call showmid
	call finalchoice
	CMP EBX, 1
	JZ mainfunc
	JMP mainexit
mainexit1:
	invoke printf, offset HINT2
mainexit:
	invoke ExitProcess, 0
main endp
end 