#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "ex1_queue_c.h"
extern const char* TestQueue(int& s);
//TestQueue.lib
int main()
{
    int s;  //s���ط���, p����ʾ��Ϣ
    const char* q = TestQueue(s);
    printf("%d, %s \n", s, q);
}