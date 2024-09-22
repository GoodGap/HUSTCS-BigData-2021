.686P
.model flat,c
externdef SF:dword, SDA:dword, SDB:dword, SDC:dword
public getf

.code
getf proc count: dword             ;获取f的子程序，有入口参数
	MOV EDI, count
	MOV EAX, [4*EDI+SDA]
	MOV EBX, [4*EDI+SDB]
	MOV ECX, [4*EDI+SDC]
	IMUL EAX, 5
	ADD EAX, EBX
	SUB EAX, ECX
	ADD EAX, 100
	SAR EAX, 7
	MOV SF, EAX                     ;将结果保存在SF中
	RET
getf endp

end