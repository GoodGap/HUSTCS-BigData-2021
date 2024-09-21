#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "ex3_stack_c++.h"

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
	return *this;
}

char* QUEUE::print(char* s) const noexcept {//打印队列至s并返回s
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

QUEUE::~QUEUE() {//销毁当前队列
	if (elems) {
		delete[]elems;
		head = 0;
		tail = 0;
		*(int**)&elems = 0;
		*(int*)&max = 0;
	}
}

//以下是STACK类成员函数体定义

STACK::STACK(int m) : QUEUE(m), q(m) {//初始化栈：最多存放2m-2个元素
}

STACK::STACK(const STACK& s) :QUEUE(s), q(s.q) {//用栈s深拷贝初始化栈

}//此处QUEUE(s)中s代表STACK继承的基类，s.q代表s的成员对象

STACK::STACK(STACK&& s)noexcept :QUEUE((QUEUE&&)s), q((QUEUE&&)s.q) {//用栈s移动拷贝初始化栈
}

int STACK::size()const noexcept {//返回栈的容量即2m
	return 2 * QUEUE::size();
}

STACK::operator int() const noexcept {//返回栈的实际元素个数
	return QUEUE::operator int() + (int)q;
	//return (int)*this（错，递归） + (int)q（对）;
	//return this->operator int()（错，递归） + q.operator int()（对）;
}

STACK& STACK::operator<<(int e) {//将e入栈，并返回当前栈
	if (operator int() >= size() - 2)
		throw "STACK is full!";
	if (QUEUE::operator int() >= QUEUE::size() - 1)//第一个队列已满
		q.operator<<(e);
	else
		QUEUE::operator<<(e);
	return *this;
}

STACK& STACK::operator>>(int& e) {//出栈到e，并返回当前栈
	if (!operator int())
		throw "STACK is empty!";
	int temp[5000] = { 0 };
	if (q.operator int())        //第二个队列非空
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

STACK& STACK::operator=(const STACK& s) {//深拷贝赋值并返回被赋值栈
	QUEUE::operator=(s);
	q.operator=(s.q);
	return *this;
}

STACK& STACK::operator=(STACK&& s)noexcept {//移动赋值并返回被赋值栈
	if (s == *this)
		return *this;
	QUEUE::operator=((QUEUE&&)s);
	q.operator=((QUEUE&&)s.q);
	return *this;
}

char* STACK::print(char* b)const noexcept {//从栈底到栈顶打印栈元素 
	if (!QUEUE::operator int())
		return b;
	else {
		QUEUE::print(b);
		if (!q.operator int())
			return b;
		else
		{
			sprintf(b + strlen(b), ",");//两队列之间加入逗号
			q.print(b);
			return b;
		}
	}
}

STACK::~STACK()noexcept {//销毁栈
}