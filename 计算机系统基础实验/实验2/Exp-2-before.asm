.686P 
.model flat,c
 ExitProcess proto  stdcall :dword
 includelib  kernel32.lib
 printf      proto c :vararg
 includelib  libcmt.lib   
 includelib  legacy_stdio_definitions.lib
 clock		 proto c

.data

SAMID DB 1,2,3,4,5,6,7,8,9,10   ;ÿ�����ݵ���ˮ�ţ��ɴ�1��ʼ��ţ�
	SDA DD 1374,248364,3840,3764,0,-1,-2,-3,-4,0
	SDB DD 3144,2342,29743,4382,0,-1,-2,-3,-4,0
	SDC DD 13155,27573,3474,45083,0,-1,-2,-3,-4,0  ;״̬��Ϣa,b,c
	SF DD ?                         ;������f�����ڴ������
	LOWF DD 500 DUP(0)
	MIDF DD 500 DUP(0)
	HIGHF DD 500 DUP(0)            ;ÿ���洢��500����λ
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

; ��������
LOOOP:
	MOV ESI, 0                      ;�洢��ַ��ѭ��������
	MOV RECR, 0                      ;ע�ⲻ����ESP
	MOV EBP, 0
	MOV EDI, 0                      ;�ֱ�洢�������ݶεı�ַ
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
	IDIV ECX                        ;���������EAX��������EDX
	MOV SF, EAX                     ;�����EAX������SF��
	MOV EAX, [4*ESI+SDA]
	MOV EBX, [4*ESI+SDB]
	MOV ECX, [4*ESI+SDC]
	MOV DH, BYTE PTR[ESI+SAMID]     ;���½���������ת�Ƶ�EAX~ECX��DH��
	CMP SF, 100
	JL L                            ;��SF<100��ת��L
	JG G                            ;��SF>100��ת��G
	JE M                            ;��SF=100��ת��M
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