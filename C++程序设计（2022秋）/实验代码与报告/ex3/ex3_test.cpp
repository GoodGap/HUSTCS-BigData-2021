#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "ex3_stack_c++.h"
extern const char* TestSTACK(int& s);
int main()
{
	int s;  //s���ط���, p����ʾ��Ϣ
	const char* q = TestSTACK(s);
	printf("%d, %s \n", s, q);
}