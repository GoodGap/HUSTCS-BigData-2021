#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "ex3_stack_c++.h"

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
	return *this;
}

char* QUEUE::print(char* s) const noexcept {//��ӡ������s������s
	int temp = 0;
	//s[0] = 0;
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
	sprintf(t, "%d\0", elems[temp]);
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

//������STACK���Ա�����嶨��

STACK::STACK(int m) : QUEUE(m), q(m) {//��ʼ��ջ�������2m-2��Ԫ��
}

STACK::STACK(const STACK& s) :QUEUE(s), q(s.q) {//��ջs�����ʼ��ջ

}//�˴�QUEUE(s)��s����STACK�̳еĻ��࣬s.q����s�ĳ�Ա����

STACK::STACK(STACK&& s)noexcept :QUEUE((QUEUE&&)s), q((QUEUE&&)s.q) {//��ջs�ƶ�������ʼ��ջ
}

int STACK::size()const noexcept {//����ջ��������2m
	return 2 * QUEUE::size();
}

STACK::operator int() const noexcept {//����ջ��ʵ��Ԫ�ظ���
	return QUEUE::operator int() + (int)q;
	//return (int)*this�����ݹ飩 + (int)q���ԣ�;
	//return this->operator int()�����ݹ飩 + q.operator int()���ԣ�;
}

STACK& STACK::operator<<(int e) {//��e��ջ�������ص�ǰջ
	if (operator int() >= size() - 2)
		throw "STACK is full!";
	if (QUEUE::operator int() >= QUEUE::size() - 1)//��һ����������
		q.operator<<(e);
	else
		QUEUE::operator<<(e);
	return *this;
}

STACK& STACK::operator>>(int& e) {//��ջ��e�������ص�ǰջ
	if (!operator int())
		throw "STACK is empty!";
	int temp[5000] = { 0 };
	if (q.operator int())        //�ڶ������зǿ�
	{
		int a = q.operator int();
		for (int i = 0; i < a - 1; i++)
			q.operator>>(temp[i]);
		q.operator>>(e);
		for (int i = 0; i < a - 1; i++)
			q.operator<<(temp[i]);
	}
	else
	{
		int b = QUEUE::operator int();
		for (int i = 0; i < b - 1; i++)
			QUEUE::operator>>(temp[i]);
		QUEUE::operator>>(e);
		for (int i = 0; i < b - 1; i++)
			QUEUE::operator<<(temp[i]);
	}
	return *this;
}

STACK& STACK::operator=(const STACK& s) {//�����ֵ�����ر���ֵջ
	QUEUE::operator=(s);
	q.operator=(s.q);
	return *this;
}

STACK& STACK::operator=(STACK&& s)noexcept {//�ƶ���ֵ�����ر���ֵջ
	if (s == *this)
		return *this;
	QUEUE::operator=((QUEUE&&)s);
	q.operator=((QUEUE&&)s.q);
	return *this;
}

char* STACK::print(char* b)const noexcept {//��ջ�׵�ջ����ӡջԪ�� 
	if (!QUEUE::operator int())
		return b;
	else {
		QUEUE::print(b);
		if (!q.operator int())
			return b;
		else
		{
			sprintf(b + strlen(b), ",");//������֮����붺��
			q.print(b);
			return b;
		}
	}
}

STACK::~STACK()noexcept {//����ջ
}