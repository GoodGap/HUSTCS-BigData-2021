.386
STACK 	SEGMENT USE16 STACK
             	DB 200 DUP(0)
STACK 	ENDS

DATA	SEGMENT USE16
	SAMID DB 1,2,3,4,5,6,7,8,9,10   ;ÿ�����ݵ���ˮ�ţ��ɴ�1��ʼ��ţ�
	SDA DD 1,2,3,4,0,-1,-2,-3,-4,0
	SDB DD 1,2,3,4,0,-1,-2,-3,-4,0
	SDC DD 1,2,3,4,0,-1,-2,-3,-4,0  ;״̬��Ϣa,b,c
	SF DD ?                         ;������f�����ڴ������
	LOWF DD 1000 DUP(0)
	MIDF DD 1000 DUP(0)
	HIGHF DD 1000 DUP(0)            ;ÿ���洢��1000����λ
	COUNT DB 10
	RECR DD ?
DATA	ENDS
CODE	SEGMENT USE16
	ASSUME  CS:CODE, SS:STACK, DS:DATA, ES:DATA
START:	MOV AX, DATA
	MOV DS, AX
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
	ADD EAX, EBX                ;һ��ע�����ּĴ�������Ҫ��ͬ�����򱨴�
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
	MOV AH, 4CH
	INT  21H
CODE	ENDS
	END  START