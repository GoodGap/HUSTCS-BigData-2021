.386
STACK SEGMENT USE16 STACK   ;主程序的堆栈段
            DB 200 DUP(0)
STACK ENDS
;
CODE SEGMENT USE16
           ASSUME CS:CODE, DS:CODE, SS:STACK
;新的INT 08H使用的变量
           COUNT DB 18                    ;“滴答”计数
           HOUR DB ?,?,':'                  ;小时的ASCII码
           MIN DB ?,?,':'                      ;分钟的ASCII码
           SEC DB ?,?                          ;秒的ASCII码
           BUF_LEN = $ - HOUR        ;计算显示信息长度
           CURSOR DW ?                   ;原光标位置
           OLD_INT DW ?,?                ;原INT 08H的中断矢量
           HINT DB "Already installed", 0dh, 0ah, '$'
;新的INT 08H的代码
NEW08H PROC FAR
	PUSHF
	CALL DWORD PTR CS:OLD_INT
			      ;完成原功能
	DEC CS:COUNT	      ;倒计数
	JZ DISP		      ;计满18次，转时钟显示
	IRET		      ;未计满，中断返回
DISP:	MOV CS:COUNT,18
	STI
	PUSHA
	PUSH DS
	PUSH ES
	MOV AX,CS
	MOV DS,AX
	MOV ES,AX
	CALL GET_TIME
	MOV BH,0
	MOV AH,3
	INT 10H
	MOV CURSOR,DX
	MOV BP,OFFSET HOUR
	MOV BH,0
	MOV DH,0
	MOV DL,79-BUF_LEN
	MOV BL,07H
	MOV CX,BUF_LEN
	MOV AL,0
	MOV AH,13H
	INT 10H
	MOV BH,0
	MOV DX,CURSOR
	MOV AH,2
	INT 10H
	POP ES
	POP DS
	POPA
	IRET
NEW08H ENDP

GET_TIME PROC
    MOV  AL,4
    OUT 70H,AL
    JMP $+2
    IN AL,71H
    MOV AH,AL
    AND AL,0FH
    SHR AH,4
    ADD AX,3030H
    XCHG AH,AL

    MOV WORD PTR HOUR,AX
    MOV AL,2
    OUT 70H,AL
    JMP $+2
    IN AL,71H
    MOV AH,AL
    AND AL,0FH
    SHR AH,4
    ADD AX,3030H
    XCHG AH,AL

    MOV WORD PTR MIN,AX
    MOV AL,0
    OUT 70H,AL
    JMP $+2
    IN AL,71H
    MOV AH,AL
    AND AL,0FH
    SHR AH,4
    ADD AX,3030H
    XCHG AH,AL
    MOV WORD PTR SEC,AX
    RET
GET_TIME ENDP

;主程序+中断处理程序的安装
BEGIN:
   PUSH CS
   POP DS
   MOV AX,3508H
   INT 21H
   CMP BX,OFFSET NEW08H         ;判断是否已安装新中断处理程序
   JZ INSTALLED
   MOV OLD_INT,BX
   MOV OLD_INT+2,ES
   MOV DX,OFFSET NEW08H
   MOV AX,2508H                          ;设置新的08H中断矢量
   INT 21H
   JMP NEXT
;主程序的其它工作

INSTALLED:
   LEA DX,HINT
   MOV AH,9
   INT 21H
   JMP FINISH

NEXT:MOV AH,0
     INT 16H
     CMP AL,'q'
     JNE NEXT
     ;LDS DX,DWORD PTR OLD_INT
     ;MOV AX,2508H
     ;INT 21H
     MOV AH,4CH
     INT 21H

FINISH: MOV    DX,OFFSET BEGIN+15
          MOV    CL,4
          SHR    DX,CL
          ADD    DX,10H
          MOV    AH,31H
          INT    21H

CODE ENDS
  END BEGIN