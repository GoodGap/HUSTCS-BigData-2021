.686P
.model flat,c
externdef SF:dword, SDA:dword, SDB:dword, SDC:dword
public getf

.code
getf proc count: dword             ;��ȡf���ӳ�������ڲ���
	MOV EDI, count
	MOV EAX, [4*EDI+SDA]
	MOV EBX, [4*EDI+SDB]
	MOV ECX, [4*EDI+SDC]
	IMUL EAX, 5
	ADD EAX, EBX
	SUB EAX, ECX
	ADD EAX, 100
	SAR EAX, 7
	MOV SF, EAX                     ;�����������SF��
	RET
getf endp

end