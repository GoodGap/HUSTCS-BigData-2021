#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>

extern int SAMID, SDA, SDB, SDC, SF, LOWF, MIDF, HIGHF, POS, POS1, POS2;
void getf(int count);
void copyf();

int login() {
	int PASSWORD;
	char USERNAME[50];
	for (int t = 3; t > 0; t--)
	{
		printf("Input username:\n");
		scanf("%s", USERNAME);
		printf("Input password:\n");
		scanf("%d", &PASSWORD);
		if (strcmp(USERNAME, "ljp") || PASSWORD != 123456)
			continue;
		else
		{
			printf("Login succeeded!\n");
			return 1;
		}
	}
	printf("Login failed!\n");
	return 0;
}

void showmid() {
	int targetpos = 0;
	printf("MIDF�洢���еĸ������ݣ�\n");
	if (POS1)
		while (targetpos != POS1) {
			int chartemp = &MIDF;
			chartemp += targetpos;
			char* temp = chartemp;
			int* temp1;
			printf("%d, ", (int)*temp);
			temp++;
			temp1 = temp;
			printf("%d, ", *temp1);
			temp1++;
			printf("%d, ", *temp1);
			temp1++;
			printf("%d\n", *temp1);
			targetpos += 13;
		}
	printf("--\n");
	return;
}

int finalchoice() {
	printf("��ѡ���������R������ִ�У���Q���˳�����M������һ��ɼ����ݣ�������f�ֶΣ�������״̬��Ϣ�еĵ�һ�����ݣ���\n");
	getchar();
	char c = getchar();
	if (c == 'M') {
		printf("���������кš�״̬��Ϣa��b��c���ÿո������\n");
		int id, a, b, c;
		scanf("%d %d %d %d", &id, &a, &b, &c);
		/*char* temp = &MIDF;
		int* temp1;
		*temp = id;
		temp++;
		temp1 = temp;
		*temp1 = a;
		temp1++;
		*temp1 = b;
		temp1++;
		*temp1 = c;*/
		char* samid = &SAMID;
		*samid = id;
		int* sd;
		sd = &SDA;
		*sd = a;
		sd = &SDB;
		*sd = b;
		sd = &SDC;
		*sd = c;
		return 2;
	}
	if (c == 'R')
		return 1;
	if (c != 'M' && c != 'R')
		return 0;
}

int main() {
	int log = login(), choice = 1;
	if (!log)
		return 0;
	while (choice == 1)
	{
		POS = 0;
		POS1 = 0;
		POS2 = 0;
		__asm MOV ESI, 0
		for (int i = 0; i < 10; i++) {
			getf(i);
			copyf();
			__asm INC ESI
		}
		showmid();
		choice = finalchoice();
		while (choice == 2)
			choice = finalchoice();
	}
	return 0;
}