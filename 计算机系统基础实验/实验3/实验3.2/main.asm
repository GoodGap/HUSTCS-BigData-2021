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
SAMID DB 1,2,3,4,5,6,7,8,9,10   ;ÿ�����ݵ���ˮ�ţ��ɴ�1��ʼ��ţ�
	SDA DD 2540,2540,2540,4,0,-1,-2,-3,-4,0
	SDB DD 1,2,3,4,0,-1,-2,-3,-4,0
	SDC DD 1,2,3,4,0,-1,-2,-3,-4,0  ;״̬��Ϣa,b,c
	SF DD ?                         ;������f�����ڴ������
	LOWF DD 300 DUP(0)
	MIDF DD 300 DUP(0)
	HIGHF DD 300 DUP(0)            ;ÿ���洢��500����λ
	POS DD 0
	POS1 DD 0
	POS2 DD 0						;�ֱ�洢��������Ļ�ַ
	SHOWHINT DB 'MIDF�洢���еĸ������ݣ�', 0AH, 0
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
	MOV DH, BYTE PTR[ESI+SAMID]     ;���½���������ת�Ƶ�EAX~ECX��DH��
	CMP SF, 100
	JE M                            ;��SF=100��ת��M
	JG G                            ;��SF>100��ת��G
	MOV ECX, POS                    ;��SF<100��ִ�����²���
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