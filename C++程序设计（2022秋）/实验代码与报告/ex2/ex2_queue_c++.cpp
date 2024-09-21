#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "ex2_queue_c++.h"

QUEUE::QUEUE(int m) :elems(new int[m]), max(m) {//初始化队列：最多申请m个元素
	head = 0;
	tail = 0;
}

QUEUE::QUEUE(const QUEUE& q) :elems(new int[q.max]), max(q.max) {//用q深拷贝初始化队列
	head = q.head;
	tail = q.tail;
	for (int i = 0; i < q.max; i++)
		elems[i] = q.elems[i];
}

QUEUE::QUEUE(QUEUE&& q)noexcept :elems(q.elems), max(q.max) {//用q移动初始化队列
	head = q.head;
	tail = q.tail;
	*(int**)&q.elems = 0;
	*(int*)&q.max = 0;
	q.head = 0;
	q.tail = 0;
}

QUEUE::operator int() const noexcept {//返回队列的实际元素个数
	if (!max)
		return 0;
	return (tail - head + max) % max;
}

int QUEUE::size() const noexcept {//返回队列申请的最大元素个数max
	return max;
}

QUEUE& QUEUE::operator<<(int e) {//将e入队列尾部，并返回当前队列
	if (!max || (tail + 1) % max == head)
		throw "QUEUE is full!";
	elems[tail] = e;
	tail = (tail + 1) % max;
	return *this;
}

QUEUE& QUEUE::operator>>(int& e) {//从队首出元素到e，并返回当前队列
	if (head == tail)
		throw "QUEUE is empty!";
	e = elems[head];
	head = (head + 1) % max;
	return *this;
}

QUEUE& QUEUE::operator=(const QUEUE& q) {//深拷贝赋值并返回被赋值队列
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

QUEUE& QUEUE::operator=(QUEUE&& q)noexcept {//移动赋值并返回被赋值队列
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

char* QUEUE::print(char* s) const noexcept {//打印队列至s并返回s
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

QUEUE::~QUEUE() {//销毁当前队列
	if (elems) {
		delete[]elems;
		head = 0;
		tail = 0;
		*(int**)&elems = 0;
		*(int*)&max = 0;
	}
}