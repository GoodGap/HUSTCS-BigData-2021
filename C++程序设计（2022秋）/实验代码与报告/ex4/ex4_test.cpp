#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "ex4_mat_c++.h"
extern const char* TestMAT(int& s);

int main()
{
	int s;  //s���ط���, p����ʾ��Ϣ
	const char* q = TestMAT(s);
	printf("%d, %s \n", s, q);
}

//int main(int argc, char* argv[]) 					//��չmain()����int�����
//{
//	MAT<int>   a(1, 2), b(2, 2), c(1, 2);
//	char t[2048];
//	a[0][0] = 1;			//���Ƶس�ʼ�����������Ԫ��
//	a[0][1] = 2; 			//�ȼ��ڡ�*(a.operator[ ](0)+1)=2;�����ȼ��ڡ�*(a[0]+1)=2;��
//	printf("��ʼ������a\n");
//  a.print(t);			//��ʼ�����������þ���
//	b[0][0] = 3; 	b[0][1] = 4;		//����T* const operator[ ](int r)��ʼ������Ԫ��
//	b[1][0] = 5; 	b[1][1] = 6;
//  printf("��ʼ������b\n");
//	b.print(t);
//	c = a * b;						//���Ծ���˷�����
//  printf("���Ծ���˷�����\n");
//	c.print(t);
//	(a + c).print(t);					//���Ծ���ӷ�����
//	c = c - a;						//���Ծ����������
//  printf("���Ծ����������\n");
//	c.print(t);
//	c += a;							//���Ծ���+=������
//  printf("���Ծ���+=������\n");
//	c.print(t);
//	c = ~a;							//���Ծ���ת������
//  printf("���Ծ���ת������\n");
//	c.print(t);
//	return 0;
//}

//int main(int argc, char* argv[]) 					//��չmain()����long long�����
//{
//	MAT<long long>   a(1, 2), b(2, 2), c(1, 2);
//	char t[2048];
//	a[0][0] = 1;			//���Ƶس�ʼ�����������Ԫ��
//	a[0][1] = 2; 			//�ȼ��ڡ�*(a.operator[ ](0)+1)=2;�����ȼ��ڡ�*(a[0]+1)=2;��
//	printf("��ʼ������a\n");
//  a.print(t);			//��ʼ�����������þ���
//	b[0][0] = 30000000000000000; 	b[0][1] = 40000000000000000;		//����T* const operator[ ](int r)��ʼ������Ԫ��
//	b[1][0] = 50000000000000000; 	b[1][1] = 60000000000000000;
//  printf("��ʼ������b\n");
//	b.print(t);
//	c = a * b;						//���Ծ���˷�����
//  printf("���Ծ���˷�����\n");
//	c.print(t);
//	(a + c).print(t);					//���Ծ���ӷ�����
//	c = c - a;						//���Ծ����������
//  printf("���Ծ����������\n");
//	c.print(t);
//	c += a;							//���Ծ���+=������
//  printf("���Ծ���+=������\n");
//	c.print(t);
//	c = ~a;							//���Ծ���ת������
//  printf("���Ծ���ת������\n");
//	c.print(t);
//	return 0;
//}