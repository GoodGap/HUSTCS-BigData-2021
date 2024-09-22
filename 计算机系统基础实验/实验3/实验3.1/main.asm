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
	HINT DB 'Input username:', 0AH, 0
	HINT1 DB 'Input password:', 0AH, 0
	HINT2 DB 'Login failed!', 0AH, 0
	HINT3 DB 'Login succeeded!', 0AH, 0
	USERNAME DB 'ljp', 0
	PASSWORD DD 123456
	USERNAMEREAD DD ?, 0
	PASSWORDREAD DD ?, 0
	SHOWHINT DB 'MIDF�洢���еĸ������ݣ�', 0AH, 0
	SHOWHINT1 DB '--', 0AH, 0
	NEWLINE DB 0AH, 0
	PARTITION DB ', ', 0
	TEMP DB ?
	TEMP1 DD ?
	CHOICEHINT DB '��ѡ���������R������ִ�У���Q���˳�����', 0AH, 0
	CHOICE DB 'R', 0
	CHOICEREAD DD ?, 0
lpfmt db "%d",0ah, 0dh, 0
lpfmt1 db "%s", 0
lpfmt2 db "%d", 0
lpfmt3 db "%d", 0
lpfmt4 db "%d", 0

.stack 200

.code
login proc						;��¼�ӳ��򣬶��岢ʹ�þֲ�����
	local t:dword
	MOV t, 3                    ;�������
	MOV ECX, 0                  ;����Ƿ�������ȷ
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

showmid proc
	local targetpos:dword
	invoke printf, offset SHOWHINT
	CMP POS1, 0
	JZ showmidout
	MOV targetpos, 0					;���ַ
	MOV EDI, POS1						;Ϊ��ֵ
	LEA EBX, MIDF						;EBX��ƫ�Ƶ�ַ
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
	CMP EAX, 0									;���¼���f
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
; ��������
	MOV POS, 0
	MOV POS1, 0
	MOV POS2, 0
	call login
	CMP ECX, 1
	JNZ mainexit1					;��¼ʧ��
	invoke printf, offset HINT3
	MOV ESI, 0                      ;�洢��ַ��ѭ��������
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