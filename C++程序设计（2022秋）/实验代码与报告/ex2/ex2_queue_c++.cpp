#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "ex2_queue_c++.h"

QUEUE::QUEUE(int m) :elems(new int[m]), max(m) {//��ʼ�����У��������m��Ԫ��
	head = 0;
	tail = 0;
}

QUEUE::QUEUE(const QUEUE& q) :elems(new int[q.max]), max(q.max) {//��q�����ʼ������
	head = q.head;
	tail = q.tail;
	for (int i = 0; i < q.max; i++)
		elems[i] = q.elems[i];
}

QUEUE::QUEUE(QUEUE&& q)noexcept :elems(q.elems), max(q.max) {//��q�ƶ���ʼ������
	head = q.head;
	tail = q.tail;
	*(int**)&q.elems = 0;
	*(int*)&q.max = 0;
	q.head = 0;
	q.tail = 0;
}

QUEUE::operator int() const noexcept {//���ض��е�ʵ��Ԫ�ظ���
	if (!max)
		return 0;
	return (tail - head + max) % max;
}

int QUEUE::size() const noexcept {//���ض�����������Ԫ�ظ���max
	return max;
}

QUEUE& QUEUE::operator<<(int e) {//��e�����β���������ص�ǰ����
	if (!max || (tail + 1) % max == head)
		throw "QUEUE is full!";
	elems[tail] = e;
	tail = (tail + 1) % max;
	return *this;
}

QUEUE& QUEUE::operator>>(int& e) {//�Ӷ��׳�Ԫ�ص�e�������ص�ǰ����
	if (head == tail)
		throw "QUEUE is empty!";
	e = elems[head];
	head = (head + 1) % max;
	return *this;
}

QUEUE& QUEUE::operator=(const QUEUE& q) {//�����ֵ�����ر���ֵ����
	if (&q == this)
		return *this;
	if (elems)
	{
		free(elems);
		*(int**)&elems = 0;
	}
	*(int**)&elems = new int[q.max];
	for (int i = 0; i < q.max; i++)
		elems[i] = q.elems[i];
	head = q.head;
	tail = q.tail;
	*(int*)&max = q.max;
	return *this;
}

QUEUE& QUEUE::operator=(QUEUE&& q)noexcept {//�ƶ���ֵ�����ر���ֵ����
	if (&q == this)
		return *this;
	*(int**)&elems = q.elems;
	head = q.head;
	tail = q.tail;
	*(int**)&q.elems = 0;
	*(int*)&q.max = 0;
	q.head = 0;
	q.tail = 0;
}

char* QUEUE::print(char* s) const noexcept {//��ӡ������s������s
	int temp;
	s[0] = 0;
	for (int i = head; i != tail; i = (i + 1) % max)
	{
		char* t = s + strlen(s);
		if ((i + 1) % max == tail)
		{
			temp = i;
			break;
		}
		sprintf(t, "%d,", elems[i]);
	}
	char* t = s + strlen(s);
	sprintf(t, "%d", elems[temp]);
	return s;
}

QUEUE::~QUEUE() {//���ٵ�ǰ����
	if (elems) {
		delete[]elems;
		head = 0;
		tail = 0;
		*(int**)&elems = 0;
		*(int*)&max = 0;
	}
}