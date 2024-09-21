#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <stdio.h>
#include "ex4_mat_c++.h"
#include <iomanip>
#include <exception>
#include <typeinfo>
using namespace std;

template <typename T>
MAT<T>::MAT(int r, int c) :r(r), c(c), e(new T[r * c]) {}//矩阵定义

template <typename T>
MAT<T>::MAT(const MAT& a) : e(new T[a.r * a.c]), r(a.r), c(a.c) {
	//深拷贝构造
	for (int i = 0; i < a.r * a.c; i++)
		e[i] = a.e[i];
}

template <typename T>
MAT<T>::MAT(MAT&& a)noexcept :e(a.e), r(a.r), c(a.c) {
	//移动构造
	*(T**)&a.e = 0;
	*(int*)&a.r = 0;
	*(int*)&a.c = 0;
}

template <typename T>
MAT<T>:: ~MAT()noexcept {
	//析构函数
	if (e) {
		delete[]e;
		*(T**)&e = 0;
		*(int*)&r = 0;
		*(int*)&c = 0;
	}
}

template <typename T>
T* const MAT<T>:: operator[ ](int r) {
	//取矩阵r行的第一个元素地址，r越界抛异常
	if (r >= this->r || r < 0)
		throw "r输入越界！";
	return &e[r * c];
}

template <typename T>
MAT<T> MAT<T>:: operator+(const MAT& a)const {
	//矩阵加法，不能加抛异常
	if (r != a.r || c != a.c || !e || !a.e)
		throw "两个矩阵不能加！";
	MAT M(r, c);//结果矩阵
	for (int i = 0; i < r * c; i++)
		M.e[i] = e[i] + a.e[i];
	return M;
}

template <typename T>
MAT<T> MAT<T>:: operator-(const MAT& a)const {
	//矩阵减法，不能减抛异常
	if (r != a.r || c != a.c || !e || !a.e)
		throw "两个矩阵不能减！";
	MAT M(r, c);
	for (int i = 0; i < r * c; i++)
		M.e[i] = e[i] - a.e[i];
	return M;
}

template <typename T>
MAT<T> MAT<T>:: operator*(const MAT& a)const {
	//矩阵乘法，不能乘抛异常
	if (c != a.r || !e || !a.e)
		throw "两个矩阵不能乘！";
	MAT<T> mat(r, a.c);
	int temp;
	for (int i = 0; i < r; i++)
		for (int j = 0; j < a.c; j++)
		{
			temp = 0;
			for (int k = 0; k < c; k++)
				temp += e[i * c + k] * a.e[k * a.c + j];
			mat.e[i * a.c + j] = temp;
		}
	return mat;
}

template <typename T>
MAT<T> MAT<T>::operator~()const {
	//矩阵转置
	MAT M(c, r);
	for (int i = 0; i < r; i++)
		for (int j = 0; j < c; j++)
			M.e[j * r + i] = e[i * c + j];
	return M;
}

template <typename T>
MAT<T>& MAT<T>::operator=(const MAT& a) {
	//深拷贝赋值运算
	if (&a == this)
		return *this;
	*(T**)&e = new T[a.r * a.c];
	*(int*)&r = a.r;
	*(int*)&c = a.c;
	for (int i = 0; i < r * c; i++)
		e[i] = a.e[i];
	return *this;
}

template <typename T>
MAT<T>& MAT<T>:: operator=(MAT&& a)noexcept {
	//移动赋值运算
	if (&a == this)
		return *this;
	*(T**)&e = a.e;
	*(int*)&r = a.r;
	*(int*)&c = a.c;
	*(T**)&a.e = 0;
	*(int*)&a.r = 0;
	*(int*)&a.c = 0;
	return *this;
}

template <typename T>
MAT<T>& MAT<T>:: operator+=(const MAT& a) {
	//“+=”运算
	if (r != a.r || c != a.c || !e || !a.e)
		throw "两个矩阵不能加！";
	for (int i = 0; i < r * c; i++)
		e[i] = e[i] + a.e[i];
	return *this;
}

template <typename T>
MAT<T>& MAT<T>:: operator-=(const MAT& a) {
	//“-=”运算
	if (r != a.r || c != a.c || !e || !a.e)
		throw "两个矩阵不能减！";
	for (int i = 0; i < r * c; i++)
		e[i] = e[i] - a.e[i];
	return *this;
}

template <typename T>
MAT<T>& MAT<T>:: operator*=(const MAT& a) {
	//“*=”运算
	if (c != a.r || !e || !a.e)
		throw "两个矩阵不能乘！";
	MAT<T> mat(r, a.c);
	*this = *this * a;
	return *this;
}

template<>
char* MAT<int>::print(char* s)const noexcept {
	//print输出至s并返回s：列用空格隔开，行用回车结束
	//此处为int对应的函数，还有long long和float对应的函数见下
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++)
			sprintf(s + strlen(s), "%6ld ", (int)e[i * c + j]);
		sprintf(s + strlen(s), "\n");
	}
	return s;
}

template<>
char* MAT<long long>::print(char* s)const noexcept {
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++)
			sprintf(s + strlen(s), "%6lld ", (long long)e[i * c + j]);
		sprintf(s + strlen(s), "\n");
	}
	return s;
}

template<>
char* MAT<float>::print(char* s)const noexcept {
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++)
			sprintf(s + strlen(s), "%8f ", (float)e[i * c + j]);
		sprintf(s + strlen(s), "\n");
	}
	return s;
}