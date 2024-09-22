.686P
.model flat,c

 public copyf
 public POS
 public POS1
 public POS2
 public SAMID
 public SDA
 public SDB
 public SDC
 public LOWF
 public MIDF
 public HIGHF

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
	SHOWHINT DB 'MIDF存储区中的各组数据：', 0AH, 0
	SHOWHINT1 DB '--', 0AH, 0
	NEWLINE DB 0AH, 0
	PARTITION DB ', ', 0
	TEMP DB ?
	TEMP1 DD ?
lpfmt db "%d",0ah, 0dh, 0
lpfmt1 db "%s", 0
lpfmt2 db "%d", 0
lpfmt3 db "%d", 0
lpfmt4 db "%d", 0
.stack 200

.code
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
end