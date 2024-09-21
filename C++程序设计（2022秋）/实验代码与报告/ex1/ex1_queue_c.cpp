#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "ex1_queue_c.h"

void initQueue(Queue* const p, int m) {//��ʼ��pָ���У��������m��Ԫ��
	*(int**)&p->elems = (int*)malloc(m * sizeof(int));
	*(int*)&p->max = m;
	p->head = 0;
	p->tail = 0;
}

void initQueue(Queue* const p, const Queue& s) {//��s�����ʼ��pָ����
	*(int**)&p->elems = new int[s.max];
	for (int i = 0; i < s.max; i++)
		p->elems[i] = s.elems[i];
	p->head = s.head;
	p->tail = s.tail;
	*(int*)&p->max = s.max;
}

void initQueue(Queue* const p, Queue&& s) { //��s�ƶ���ʼ��pָ����
	*(int**)&p->elems = s.elems;
	p->head = s.head;
	p->tail = s.tail;
	*(int*)&p->max = s.max;
	*(int**)&s.elems = 0;
	*(int**)&s.head = 0;
	*(int**)&s.tail = 0;
	*(int*)&s.max = 0;
}

int number(const Queue* const p) {//����pָ���е�ʵ��Ԫ�ظ���
	if (!p->max)
		return 0;
	return (p->tail - p->head + p->max) % p->max;
}

int size(const Queue* const p) {//����pָ������������Ԫ�ظ���max
	return p->max;
}

Queue* const enter(Queue* const p, int e) {//��e�����β����������p
	if ((p->tail + 1) % p->max == p->head)
		throw "Queue is full!";
	p->elems[p->tail] = e;
	p->tail = (p->tail + 1) % p->max;
	return p;
}

Queue* const leave(Queue* const p, int& e) {//�Ӷ��׳�Ԫ�ص�e��������p
	if (p->head == p->tail)
		throw "Queue is empty!";
	e = p->elems[p->head];
	p->head = (p->head + 1) % p->max;
	return p;
}

Queue* const assign(Queue* const p, const Queue& s) {//�����s�����в�����p
	if (p == &s)
		return p;
	if (p->elems)
	{
		free(p->elems);
		*(int**)&p->elems = 0;
	}
	*(int**)&p->elems = new int[s.max];
	for (int i = 0; i < s.max; i++)
		p->elems[i] = s.elems[i];
	p->head = s.head;
	p->tail = s.tail;
	*(int*)&p->max = s.max;
	return p;
}

Queue* const assign(Queue* const p, Queue&& s) {//�ƶ���s�����в�����p
	if (p == &s)
		return p;
	if (p->elems && p != &s)
		free(p->elems);
	*(int**)&p->elems = s.elems;
	p->head = s.head;
	p->tail = s.tail;
	*(int*)&p->max = s.max;
	*(int**)&s.head = 0;
	*(int**)&s.tail = 0;
	*(int*)&s.max = 0;
	*(int**)&s.elems = 0;
	return p;
}

char* print(const Queue* const p, char* s) {//��ӡpָ������s������s
	if (p->head == p->tail)
		throw "Queue is empty!";
	char temp[100];
	int target = p->head;
	for (int i = 1; i <= number(p); i++)
	{
		_itoa_s(p->elems[target], temp, 100, 10);
		strcat_s(temp, 100, ",");
		strcat_s(s, 100, temp);
		target = (target + 1) % p->max;
	}
	return s;
}

void destroyQueue(Queue* const p) {//����pָ��Ķ���
	if (p->elems) {
		free(p->elems);
		p->head = 0;
		p->tail = 0;
		*(int**)&p->elems = 0;
		*(int*)&p->max = 0;
	}
}