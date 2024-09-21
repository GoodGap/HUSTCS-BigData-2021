#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "ex4_mat_c++.h"
extern const char* TestMAT(int& s);

int main()
{
	int s;  //s返回分数, p是提示信息
	const char* q = TestMAT(s);
	printf("%d, %s \n", s, q);
}

//int main(int argc, char* argv[]) 					//扩展main()测试int类矩阵
//{
//	MAT<int>   a(1, 2), b(2, 2), c(1, 2);
//	char t[2048];
//	a[0][0] = 1;			//类似地初始化矩阵的所有元素
//	a[0][1] = 2; 			//等价于“*(a.operator[ ](0)+1)=2;”即等价于“*(a[0]+1)=2;”
//	printf("初始化矩阵a\n");
//  a.print(t);			//初始化矩阵后输出该矩阵
//	b[0][0] = 3; 	b[0][1] = 4;		//调用T* const operator[ ](int r)初始化数组元素
//	b[1][0] = 5; 	b[1][1] = 6;
//  printf("初始化矩阵b\n");
//	b.print(t);
//	c = a * b;						//测试矩阵乘法运算
//  printf("测试矩阵乘法运算\n");
//	c.print(t);
//	(a + c).print(t);					//测试矩阵加法运算
//	c = c - a;						//测试矩阵减法运算
//  printf("测试矩阵减法运算\n");
//	c.print(t);
//	c += a;							//测试矩阵“+=”运算
//  printf("测试矩阵“+=”运算\n");
//	c.print(t);
//	c = ~a;							//测试矩阵转置运算
//  printf("测试矩阵转置运算\n");
//	c.print(t);
//	return 0;
//}

//int main(int argc, char* argv[]) 					//扩展main()测试long long类矩阵
//{
//	MAT<long long>   a(1, 2), b(2, 2), c(1, 2);
//	char t[2048];
//	a[0][0] = 1;			//类似地初始化矩阵的所有元素
//	a[0][1] = 2; 			//等价于“*(a.operator[ ](0)+1)=2;”即等价于“*(a[0]+1)=2;”
//	printf("初始化矩阵a\n");
//  a.print(t);			//初始化矩阵后输出该矩阵
//	b[0][0] = 30000000000000000; 	b[0][1] = 40000000000000000;		//调用T* const operator[ ](int r)初始化数组元素
//	b[1][0] = 50000000000000000; 	b[1][1] = 60000000000000000;
//  printf("初始化矩阵b\n");
//	b.print(t);
//	c = a * b;						//测试矩阵乘法运算
//  printf("测试矩阵乘法运算\n");
//	c.print(t);
//	(a + c).print(t);					//测试矩阵加法运算
//	c = c - a;						//测试矩阵减法运算
//  printf("测试矩阵减法运算\n");
//	c.print(t);
//	c += a;							//测试矩阵“+=”运算
//  printf("测试矩阵“+=”运算\n");
//	c.print(t);
//	c = ~a;							//测试矩阵转置运算
//  printf("测试矩阵转置运算\n");
//	c.print(t);
//	return 0;
//}