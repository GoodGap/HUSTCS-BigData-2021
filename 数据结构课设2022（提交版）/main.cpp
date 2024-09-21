#include "definition.h"
#include "SATsolver.cpp"
#include "twosudoku.cpp"

int SATsolver() {
	int op = 1, strategy; //��¼�û�����
	char input[50];
	char output[50];//��������ļ���
	formula original;
	original.clausenum = 0;
	original.literalnum = 0;
	original.singleclausenum = 0;
	original.currentclausenum = 0;
	original.head = NULL;//��ʼ��CNF��ʽ
	while (op) {
		system("cls");
		printf("\n                       SAT�������                       \n");
		printf("-----------------------------------------------------------\n");
		printf("     1.��ȡcnf�����ļ�                  2.����������Ӿ�   \n");
		printf("     3.DPLL��⣨ͬʱ�����.res�ļ���   4.ȥ����ǰ�Ӿ伯   \n");
		printf("     0.�˳�\n");
		printf("-----------------------------------------------------------\n");
		printf("��ѡ�������0-4��:");
		scanf("%d", &op);
		switch (op) {
			case 1:
				if (original.head != NULL) {
					printf("-----------------------------------------------------------\n");
					printf("�Ѵ���cnf�����ļ���\n");
					system("pause");
					break;
				} else {
					printf("-----------------------------------------------------------\n");
					printf("������cnf�����ļ����ƣ�");
					scanf("%s", input);
					strcpy(output, input);
					output[strlen(output) - 3] = 'r';
					output[strlen(output) - 2] = 'e';
					output[strlen(output) - 1] = 's';
					cnfopen(original, input);
					system("pause");
					break;
				}
			case 2:
				if (original.head == NULL) {
					printf("-----------------------------------------------------------\n");
					printf("������cnf�����ļ���\n");
					system("pause");
					break;
				} else {
					printformula(original);
					system("pause");
					break;
				}
			case 3:
				if (original.head == NULL) {
					printf("-----------------------------------------------------------\n");
					printf("������cnf�����ļ���\n");
					system("pause");
					break;
				} else {
					printf("-----------------------------------------------------------\n");
					printf("��ѡ���㷨��1Ϊԭʼ�㷨��2Ϊ�Ż��㷨����");
					scanf("%d", &strategy);
					clock_t ta = clock();//��¼�㷨��ʼʱ��
					if (DPLL(original, strategy) == OK) {
						clock_t tb = clock();//��¼�㷨����ʱ��
						printf("-----------------------------------------------------------\n");
						printf("���������ɣ�һ�ֿ��еĽ�����£�\n");
						for (int i = 1; i <= original.literalnum; i++) {
							if (result[i])
								printf("%d ", i);
							else
								printf("-%d ", i);
						}
						saveres(output, original, (tb - ta) * 1000 / CLOCKS_PER_SEC, 1);
						printf("\n-----------------------------------------------------------\n");
						printf("\n�����ʱ%d ms�����ѱ��棡\n", (tb - ta) * 1000 / CLOCKS_PER_SEC);
					} else {
						clock_t tc = clock();
						saveres(output, original, (tc - ta) * 1000 / CLOCKS_PER_SEC, 0);
						printf("�����޽⣬�����ʱ%d ms�����ѱ��棡\n", (tc - ta) * 1000 / CLOCKS_PER_SEC);
					}
					system("pause");
					break;
				}
			case 4:
				if (original.head == NULL) {
					printf("-----------------------------------------------------------\n");
					printf("������cnf�����ļ���\n");
					system("pause");
					break;
				} else {
					deleteformula(original);
					system("pause");
					break;
				}
			case 0:
				return 0;
		}
	}
	return 0;
}

int twosudoku() {
	int op = 1, ans = 0, holecount = 0; //��¼�û����롢DPLL��������ڶ�����
	int a, x, y, value; //��¼��a������x��y�������ֵvalue
	formula newdouble;
	newdouble.clausenum = 0;
	newdouble.literalnum = 0;
	newdouble.singleclausenum = 0;
	newdouble.currentclausenum = 0;
	newdouble.head = NULL;//��ʼ��˫����CNF
	while (op) {
		system("cls");
		printf("\n                        ˫������Ϸ                       \n");
		printf("-----------------------------------------------------------\n");
		printf("          1.���˫������Ϸ              2.����˫����\n");
		printf("          0.�˳�\n");
		printf("-----------------------------------------------------------\n");
		printf("��ѡ�������0-2����");
		scanf("%d", &op);
		switch (op) {
			case 1:
				generatecnf(newdouble);
				printf("����չʾ��ʼ˫�������̸�֣�\n");
				printtwosudoku1(newdouble);
				system("pause");
				printf("\n\nDPLL��������£�\n");
				ans = solvesudoku(newdouble);
				printf("%d��1�����н⣬0�����޽⣩\n\n", ans);
				if (ans)
					for (int i = 1; i <= newdouble.literalnum; i++) {
						if (result[i])
							printf("%d ", i);
						else
							printf("-%d ", i);
					}
				system("pause");
				printf("\n\nԭʼ˫������������£�\n");
				printtwosudoku2(newdouble);
				system("pause");
				break;
			case 2:
				if (!fullboard[111]) {
					printf("�����ڳ�ʼ���̣�������⣡\n");
					system("pause");
					break;
				}
				printf("�������ڶ�������\n");
				scanf("%d", &holecount);
				digholes(newdouble, fullboard, holecount);//�ڶ�
				printtwosudoku2(newdouble);
				while (holecount > 0) {//�������
					printf("�������������ֵ�����λ�ã����Ͻ�Ϊ1�����½�Ϊ2�����С��к�ֵ���ÿո������\n");
					scanf("%d %d %d %d", &a, &x, &y, &value);
					if (!board[a * 100 + x * 10 + y] && fullboard[a * 100 + x * 10 + y] == value) {
						board[a * 100 + x * 10 + y] = value;
						holecount--;
						printf("�˷���������ȷ��\n");
					} else
						printf("�����������������룡\n");
					system("pause");
					system("cls");
					printf("\n\n\n\n\n\n");
					printtwosudoku2(newdouble);
				}
				printf("���������ȷ��\n");
				system("pause");
				break;
			case 0:
				return 0;
		}
	}
	return 0;
}

int main() {
	int op = 1;
	while (op) {
		system("cls");
		printf("\n              ����SAT��˫������Ϸ������                \n");
		printf("\n           ������2101��  �����  U202115652              \n");
		printf("-----------------------------------------------------------\n");
		printf("          1.SAT�������           2.����˫����\n");
		printf("          0.�˳�\n");
		printf("-----------------------------------------------------------\n");
		printf("��ѡ�������0-2����");
		scanf("%d", &op);
		switch (op) {
			case 1:
				SATsolver();
				system("pause");
				break;
			case 2:
				twosudoku();
				system("pause");
				break;
			case 0:
				return 0;
		}
	}
	return 0;
}