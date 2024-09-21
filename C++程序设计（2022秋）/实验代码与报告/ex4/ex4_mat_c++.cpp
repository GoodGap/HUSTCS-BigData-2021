#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <stdio.h>
#include "ex4_mat_c++.h"
#include <iomanip>
#include <exception>
#include <typeinfo>
using namespace std;

template <typename T>
MAT<T>::MAT(int r, int c) :r(r), c(c), e(new T[r * c]) {}//������

template <typename T>
MAT<T>::MAT(const MAT& a) : e(new T[a.r * a.c]), r(a.r), c(a.c) {
	//�������
	for (int i = 0; i < a.r * a.c; i++)
		e[i] = a.e[i];
}

template <typename T>
MAT<T>::MAT(MAT&& a)noexcept :e(a.e), r(a.r), c(a.c) {
	//�ƶ�����
	*(T**)&a.e = 0;
	*(int*)&a.r = 0;
	*(int*)&a.c = 0;
}

template <typename T>
MAT<T>:: ~MAT()noexcept {
	//��������
	if (e) {
		delete[]e;
		*(T**)&e = 0;
		*(int*)&r = 0;
		*(int*)&c = 0;
	}
}

template <typename T>
T* const MAT<T>:: operator[ ](int r) {
	//ȡ����r�еĵ�һ��Ԫ�ص�ַ��rԽ�����쳣
	if (r >= this->r || r < 0)
		throw "r����Խ�磡";
	return &e[r * c];
}

template <typename T>
MAT<T> MAT<T>:: operator+(const MAT& a)const {
	//����ӷ������ܼ����쳣
	if (r != a.r || c != a.c || !e || !a.e)
		throw "���������ܼӣ�";
	MAT M(r, c);//�������
	for (int i = 0; i < r * c; i++)
		M.e[i] = e[i] + a.e[i];
	return M;
}

template <typename T>
MAT<T> MAT<T>:: operator-(const MAT& a)const {
	//������������ܼ����쳣
	if (r != a.r || c != a.c || !e || !a.e)
		throw "���������ܼ���";
	MAT M(r, c);
	for (int i = 0; i < r * c; i++)
		M.e[i] = e[i] - a.e[i];
	return M;
}

template <typename T>
MAT<T> MAT<T>:: operator*(const MAT& a)const {
	//����˷������ܳ����쳣
	if (c != a.r || !e || !a.e)
		throw "���������ܳˣ�";
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
	//����ת��
	MAT M(c, r);
	for (int i = 0; i < r; i++)
		for (int j = 0; j < c; j++)
			M.e[j * r + i] = e[i * c + j];
	return M;
}

template <typename T>
MAT<T>& MAT<T>::operator=(const MAT& a) {
	//�����ֵ����
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
	//�ƶ���ֵ����
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
	//��+=������
	if (r != a.r || c != a.c || !e || !a.e)
		throw "���������ܼӣ�";
	for (int i = 0; i < r * c; i++)
		e[i] = e[i] + a.e[i];
	return *this;
}

template <typename T>
MAT<T>& MAT<T>:: operator-=(const MAT& a) {
	//��-=������
	if (r != a.r || c != a.c || !e || !a.e)
		throw "���������ܼ���";
	for (int i = 0; i < r * c; i++)
		e[i] = e[i] - a.e[i];
	return *this;
}

template <typename T>
MAT<T>& MAT<T>:: operator*=(const MAT& a) {
	//��*=������
	if (c != a.r || !e || !a.e)
		throw "���������ܳˣ�";
	MAT<T> mat(r, a.c);
	*this = *this * a;
	return *this;
}

template<>
char* MAT<int>::print(char* s)const noexcept {
	//print�����s������s�����ÿո���������ûس�����
	//�˴�Ϊint��Ӧ�ĺ���������long long��float��Ӧ�ĺ�������
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