#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "ex2_queue_c++.h"
extern const char* TestQUEUE(int& s);
int main()
{
    int s;  //s���ط���, p����ʾ��Ϣ
    const char* q = TestQUEUE(s);
    printf("%d, %s \n", s, q);
}