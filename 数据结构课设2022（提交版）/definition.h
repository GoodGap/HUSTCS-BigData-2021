#ifndef data_structure_project_2022
#define data_structure_project_2022
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0





int result[3500] = {0}; //������ȡֵ�Ľ�����飬1�����棬0�����
int found[3500] = {0}; //���Ӿ�Ĳ��Ҽ�¼����
int temp[50] = {0}; //����˫����cnfʱ�ݴ�������
int board[300] = {0}; //�洢���������ֵĽ⣬board[abc]��ʾ��a����������b��c�е�ֵ
int fullboard[300] = {0}; //�洢��ʼʱ�ĺϷ����̽⣨�����ڶ�����ȫ���̱��ֲ���
FILE *fp;

typedef struct literal { //��������ʽ�洢����
	int word;//��ֵ���������֣���֮Ϊ������
	struct literal *next;//��һ������
} literal, *L;

typedef struct clause { //��������ʽ�洢�Ӿ�
	int num; //ĳ�Ӿ��ڵ���������
	struct literal *head; //��һ������
	struct clause *next; //��һ���Ӿ�
} clause, *C;

typedef struct formula { //������ʽ
	int clausenum;//��������
	int literalnum;//�Ӿ�����
	int emptyclausenum;//���Ӿ�����
	int singleclausenum;//���Ӿ���������DPLL���ж����٣�
	int currentclausenum;//��ǰ�Ӿ���������DPLL���ж����٣�
	struct clause *head;//��һ���Ӿ�
} formula, *F;
#endif