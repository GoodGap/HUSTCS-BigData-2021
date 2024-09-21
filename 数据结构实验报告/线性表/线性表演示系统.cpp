#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

//���ೣ����������
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
#define LIST_INIT_SIZE 100
#define LISTINCREMENT  10
typedef int status;//����Ԫ�����ඨ��
typedef int ElemType;

typedef struct {
	ElemType *elem;
	int length;
	int listsize;
} SqList;//���Ա�ṹ����

typedef struct {
	struct {
		char name[30];
		SqList L;
	} elem[10];
	int length;
} LISTS;//���Ա��Ͻṹ����

LISTS Lists;

status InitList(SqList &L) {
	// ���Ա�L�����ڣ�����һ���յ����Ա�����OK�����򷵻�INFEASIBLE��
	if (!L.elem) {
		L.elem = (ElemType *)malloc(LIST_INIT_SIZE * sizeof(ElemType));
		L.length = 0;
		L.listsize = LIST_INIT_SIZE;
		return OK;
	} else
		return INFEASIBLE;
}

status DestroyList(SqList &L) {
	// ������Ա�L���ڣ��������Ա�L���ͷ�����Ԫ�صĿռ䣬����OK�����򷵻�INFEASIBLE��
	if (!L.elem)
		return INFEASIBLE;
	else {
		free(L.elem);
		L.elem = NULL;
		return OK;
	}
}

status ClearList(SqList &L) {
	// ������Ա�L���ڣ�ɾ�����Ա�L�е�����Ԫ�أ�����OK�����򷵻�INFEASIBLE��
	if (!L.elem)
		return INFEASIBLE;
	else {
		L.length = 0;
		return OK;
	}
}

status ListEmpty(SqList L) {
	// ������Ա�L���ڣ��ж����Ա�L�Ƿ�Ϊ�գ��վͷ���TRUE�����򷵻�FALSE��
	// ������Ա�L�����ڣ�����INFEASIBLE��
	if (!L.elem)
		return INFEASIBLE;
	else {
		if (L.length == 0)
			return TRUE;
		else
			return FALSE;
	}
}

status ListLength(SqList L) {
	// ������Ա�L���ڣ��������Ա�L�ĳ��ȣ����򷵻�INFEASIBLE��
	if (!L.elem)
		return INFEASIBLE;
	else
		return L.length;
}

status GetElem(SqList L, int i, ElemType &e) {
	// ������Ա�L���ڣ���ȡ���Ա�L�ĵ�i��Ԫ�أ�������e�У�����OK��
	// ���i���Ϸ�������ERROR��������Ա�L�����ڣ�����INFEASIBLE��
	if (!L.elem)
		return INFEASIBLE;
	else {
		if (i < 1 || i > L.length)
			return ERROR;
		else {
			e = L.elem[i - 1];
			return OK;
		}
	}
}

int LocateElem(SqList L, ElemType e) {
	// ������Ա�L���ڣ�����Ԫ��e�����Ա�L�е�λ����Ų����ظ���ţ�
	// ���e�����ڣ�����0�������Ա�L������ʱ������INFEASIBLE����-1����
	if (!L.elem)
		return INFEASIBLE;
	else {
		int flag = -1;
		for (int i = 0; i < L.length; i++)
			if (L.elem[i] == e) {
				// �������Ա���Ԫ�أ�������ͬ������
				flag = i + 1;
				break;
			}
		if (flag != -1)
			return flag;
		else
			return ERROR;
	}
}

status PriorElem(SqList L, ElemType e, ElemType &pre) {
	// ������Ա�L���ڣ���ȡ���Ա�L��Ԫ��e��ǰ����������pre�У�����OK��
	// ���û��ǰ��������ERROR��������Ա�L�����ڣ�����INFEASIBLE��
	if (!L.elem)
		return INFEASIBLE;
	else {
		int loc;
		for (loc = 0; loc < L.length; loc++)
			if (L.elem[loc] == e)
				break;// ����Ԫ��λ��
		if (loc == 0 || loc == L.length)
			return ERROR;// Ԫ��λ��Ϊ��λ
		else {
			pre = L.elem[loc - 1];
			return OK;
		}
	}
}

status NextElem(SqList L, ElemType e, ElemType &next) {
	// ������Ա�L���ڣ���ȡ���Ա�LԪ��e�ĺ�̣�������next�У�����OK��
	// ���û�к�̣�����ERROR��������Ա�L�����ڣ�����INFEASIBLE��
	if (!L.elem)
		return INFEASIBLE;
	else {
		int loc;
		for (loc = 0; loc < L.length; loc++)
			if (L.elem[loc] == e)
				break;// ����Ԫ��λ��
		if (loc == L.length - 1 || loc == L.length)
			return ERROR;// Ԫ��λ��Ϊĩλ
		else {
			next = L.elem[loc + 1];
			return OK;
		}
	}
}

status ListInsert(SqList &L, int i, ElemType e) {
	// ������Ա�L���ڣ���Ԫ��e���뵽���Ա�L�ĵ�i��Ԫ��֮ǰ������OK��
	// ������λ�ò���ȷʱ������ERROR��������Ա�L�����ڣ�����INFEASIBLE��
	if (!L.elem)
		return INFEASIBLE;
	else {
		if (i < 1 || i > L.length + 1)
			return ERROR;
		else {
			if (L.length == 0) {
				L.elem[0] = e;
				L.length++;
			} else {
				L.length++;
				if (L.length > L.listsize) {
					ElemType *newbase = (ElemType *)realloc(L.elem, (L.listsize + LISTINCREMENT) * sizeof(ElemType));
					L.elem = newbase;
					L.listsize += LISTINCREMENT;
					// ���L.length�Ѿ��ﵽ�������Ҫ����
				}
				for (int j = L.length - 2; j >= i - 1; j--)
					L.elem[j + 1] = L.elem[j];
				// �������Ԫ��ȫ������һλ
				L.elem[i - 1] = e;
			}
			return OK;
		}
	}
}

status ListDelete(SqList &L, int i, ElemType &e) {
	// ������Ա�L���ڣ�ɾ�����Ա�L�ĵ�i��Ԫ�أ���������e�У�����OK��
	// ��ɾ��λ�ò���ȷʱ������ERROR��������Ա�L�����ڣ�����INFEASIBLE��
	if (!L.elem)
		return INFEASIBLE;
	else {
		if (i < 1 || i > L.length)
			return ERROR;
		else {
			e = L.elem[i - 1];
			for (int j = i - 1; j < L.length - 1; j++)
				L.elem[j] = L.elem[j + 1];
			// �������Ԫ��ȫ��ǰ��һλ
			L.length--;
			return OK;
		}
	}
}

status ListTraverse(SqList L) {
	// ������Ա�L���ڣ�������ʾ���Ա��е�Ԫ�أ�ÿ��Ԫ�ؼ��һ�񣬷���OK��
	// ������Ա�L�����ڣ�����INFEASIBLE��
	if (!L.elem)
		return INFEASIBLE;
	else {
		if (L.length > 0) {
			printf("\n-----------all elements -----------------------\n");
			for (int i = 0; i < L.length - 1; i++)
				printf("%d ", L.elem[i]);
			printf("%d", L.elem[L.length - 1]);
			printf("\n------------------ end ------------------------\n");
		} else
			printf("���Ա�Ϊ�գ�\n");
		return OK;
	}
}

status  SaveList(SqList L, char FileName[]) {
	// ������Ա�L���ڣ���L��Ԫ��д��FileName�ļ��У�����OK�����򷵻�INFEASIBLE��
	if (!L.elem)
		return INFEASIBLE;
	else {
		FILE *fp;
		fp = fopen(FileName, "w");
		for (int i = 0; i < L.length; i++)
			fprintf(fp, "%d ", L.elem[i]);
		// ���϶������֣�ֱ��������L.length��
		fclose(fp);
		return OK;
	}
}

status  LoadList(SqList &L, char FileName[]) {
	// ������Ա�L�����ڣ���FileName�ļ��е�Ԫ��д��L������OK�����򷵻�INFEASIBLE��
	if (L.elem)
		return INFEASIBLE;
	else {
		FILE *fp;
		fp = fopen(FileName, "r");
		L.elem = (ElemType *)malloc(LIST_INIT_SIZE * sizeof(ElemType));
		// �����µ����ݿռ�
		int len = 0;
		while (!feof(fp))
			fscanf(fp, "%d", &L.elem[len++]);
		// ���ļ���д�����ݵ����Ա��У�ֱ���ļ�����
		L.length = len - 1;
		fclose(fp);
		return OK;
	}
}

status AddList(LISTS &Lists, char ListName[]) {
	// ��Lists������һ������ΪListName�Ŀ����Ա�
	Lists.elem[Lists.length].L.elem = NULL;
	Lists.elem[Lists.length].L.length = 0;
	InitList(Lists.elem[Lists.length++].L);
	// ���Ա��ϳ���+1
	int t = 0;
	while (ListName[t] != 0) {
		Lists.elem[Lists.length - 1].name[t] = ListName[t];
		t++;
	}
	return OK;
}

status RemoveList(LISTS &Lists, char ListName[]) {
	// ��Lists��ɾ��һ������ΪListName�����Ա��ɹ�����OK�����򷵻�ERROR
	if (Lists.length < 1)
		return ERROR;
	else {
		int i, j, k, flag = 0;
		for (i = 1; i <= Lists.length; i++) {
			// �������Ա�����
			for (j = 0, k = 0; Lists.elem[i - 1].name[j] != 0; j++) {
				if (Lists.elem[i - 1].name[j] != ListName[j])
					break;
				else
					k++;
			}
			if (j == k && j != 0) {
				flag = 1;
				break;
			}
		}
		// ��������ɹ���flag��Ϊ1�����к���ɾ������
		if (flag == 1) {
			DestroyList(Lists.elem[i - 1].L);
			for (j = i; j < Lists.length; j++) {
				Lists.elem[j - 1].L = Lists.elem[j].L;
				k = 0;
				while (Lists.elem[j].name[k] != 0) {
					Lists.elem[j - 1].name[k] = Lists.elem[j].name[k];
					k++;
				}
			}
			// �������Ա�λ��ȫ��ǰ��һλ
			Lists.length--;
			return OK;
		} else
			return ERROR;
	}
}

int LocateList(LISTS Lists, char ListName[]) {
	// ��Lists�в���һ����ΪListName�����Ա��ɹ���������ţ����򷵻�0
	if (Lists.length < 1)
		return 0;
	else {
		int i, j, k, flag = 0;
		for (i = 1; i <= Lists.length; i++) {
			for (j = 0, k = 0; Lists.elem[i - 1].name[j] != 0; j++) {
				if (Lists.elem[i - 1].name[j] != ListName[j])
					break;
				else
					k++;
			}
			if (j == k && j != 0) {
				flag = 1;
				break;
			}
		}
		// ����ԭ��ͬ��
		if (flag == 1) {
			return i;
		} else
			return 0;
	}
}

status MaxSubArray(SqList L) {
	// �����Ա�L�������������������ͣ����������ֵ
	if (!L.elem)
		return INFEASIBLE;
	else {
		if (!L.length)
			return ERROR;
		else {
			int max = -9999999, tmax; //tmax�����ݴ�
			for (int i = 1; i <= L.length; i++)
				// ��������������Ԫ�ظ������б���
				for (int j = 1; j <= L.length - i + 1; j++) {
					tmax = 0;
					for (int k = j; k <= j + i - 1; k++)
						tmax += L.elem[k - 1];
					if (tmax > max)
						max = tmax;// ���ϸ������ֵ
				}
			return max;
		}
	}
}

status SubArrayNum(SqList L, int k) {
	// �����Ա�L��������Ϊһ��ֵ�����������飬�ɹ���������������򷵻�ERROR
	if (!L.elem)
		return INFEASIBLE;
	else {
		if (!L.length)
			return ERROR;
		else {
			int tmax, count = 0;// count��������
			for (int i = 1; i <= L.length; i++)
				// ��������������Ԫ�ظ������б���
				for (int j = 1; j <= L.length - i + 1; j++) {
					tmax = 0;
					for (int k = j; k <= j + i - 1; k++)
						tmax += L.elem[k - 1];
					if (tmax == k)
						count++;
				}
			return count;
		}
	}
}

status SortList(SqList L) {
	// �����Ա�L�����������򣬳ɹ�����OK�����򷵻�ERROR
	if (!L.elem)
		return INFEASIBLE;
	else {
		if (!L.length)
			return ERROR;
		else {
			int t;
			for (int i = 0; i < L.length - 1; i++)
				for (int j = 0; j < L.length - i - 1; j++)
					if (L.elem[j] > L.elem[j + 1]) {
						t = L.elem[j];
						L.elem[j] = L.elem[j + 1];
						L.elem[j + 1] = t;
					}
			// ʹ��ð������
			return OK;
		}
	}
}

status  AdvancedSave(SqList L, char FileName[]) {
	// �����Ա�L�����ļ��Ĵ�ȡ����
	if (!L.elem)
		return INFEASIBLE;
	else {
		FILE *fp;
		fp = fopen(FileName, "w");
		fprintf(fp, "���Ա��Ϊ��%d\n", L.length);
		fprintf(fp, "���Ա�洢��ʽΪ��˳��洢�����飩��\n��Ԫ��Ϊ��\n");
		fprintf(fp, "(��Ԫ��) ");
		for (int i = 0; i < L.length; i++)
			fprintf(fp, "%d ", L.elem[i]);
		fprintf(fp, " (βԪ��)");
		fclose(fp);
		return OK;
	}
}

int main(void) {
	SqList L;
	L.elem = NULL;
	int op = 1;
	int i, e, pre, next, cur, testans, knum, choice;
	char FileName[30], ListName[50];
//	LA.length = 0;// �����Ա��Ͻ��г�ʼ��
	while (op) {
		system("cls");
		printf("\n\n");
		printf("-------------------------------------------------\n");
		printf("          ����˳��洢�ṹ�����Ա�ʵ�� \n      ʵ����ʾϵͳ    �������2110��  �������\n");
		printf("-------------------------------------------------\n");
		printf("    	  1. InitList       7. LocateElem\n");
		printf("    	  2. DestroyList    8. PriorElem\n");
		printf("    	  3. ClearList      9. NextElem \n");
		printf("    	  4. ListEmpty      10. ListInsert\n");
		printf("    	  5. ListLength     11. ListDelete\n");
		printf("    	  6. GetElem        12. ListTraverse\n");
		printf("    	  13. Save/Load     14. MaxSubArray\n");
		printf("    	  15. SubArrayNum   16. SortList\n");
		printf("    	  17. AdvancedFile  18. MultiList\n");
		printf("    	  0. Exit\n");
		printf("-------------------------------------------------\n");
		printf("    ��ѡ����Ĳ���[0~18]:\n");
		scanf("%d", &op);
		switch (op) {
			// ���·ֱ��Ӧ18�����ܣ������������ִ�ж�Ӧ�Ĳ���
			case 1:
				if (InitList(L) == OK)
					printf("���Ա����ɹ���\n");
				else
					printf("���Ա���ʧ�ܣ�\n");
				getchar();
				getchar();
				break;
			case 2:
				if (DestroyList(L) == OK)
					printf("����Ԫ�ؿռ��ͷ���ɣ����Ա����ٳɹ���\n");
				else
					printf("���Ա�����ʧ�ܣ�\n");
				getchar();
				getchar();
				break;
			case 3:
				if (ClearList(L) == OK)
					printf("���Ա�����������Ԫ����ɾ�������Ա���ճɹ���\n");
				else
					printf("���Ա����ʧ�ܣ�\n");
				getchar();
				getchar();
				break;
			case 4:
				if (ListEmpty(L) == TRUE)
					printf("���Ա�Ϊ�գ�\n");
				else if (ListEmpty(L) == FALSE)
					printf("���Ա�Ϊ�գ�\n");
				else if (ListEmpty(L) == INFEASIBLE)
					printf("���Ա����ڣ�\n");
				getchar();
				getchar();
				break;
			case 5:
				if (ListLength(L) == INFEASIBLE)
					printf("���Ա����ڣ�\n");
				else
					printf("���Ա�ĳ���Ϊ%d��\n", ListLength(L));
				getchar();
				getchar();
				break;
			case 6:
				printf("������Ҫ��ѯ��Ԫ��λ����ţ�");
				scanf("%d", &i);
				testans = GetElem(L, i, e);
				if (testans == OK)
					printf("���Ա�ĵ�%d��Ԫ��Ϊ%d��\n", i, e);
				else if (testans == ERROR)
					printf("����iֵ���Ϸ���\n");
				else if (testans == INFEASIBLE)
					printf("���Ա����ڣ�\n");
				getchar();
				getchar();
				break;
			case 7:
				printf("������Ҫ��ѯ��Ԫ�ص�ֵ��");
				scanf("%d", &e);
				testans = LocateElem(L, e);
				if (testans == ERROR)
					printf("����Ԫ�ز����ڣ�\n");
				else if (testans == INFEASIBLE)
					printf("���Ա����ڣ�\n");
				else
					printf("Ԫ��%d�����Ա��е�λ�����Ϊ%d��\n", e, testans);
				getchar();
				getchar();
				break;
			case 8:
				printf("������Ҫ��ѯǰ����Ԫ�ص�ֵ��");
				scanf("%d", &e);
				testans = PriorElem(L, e, pre);
				if (testans == OK)
					printf("Ԫ��%d��ǰ��Ϊ%d��\n", e, pre);
				else if (testans == ERROR)
					printf("��Ԫ��û��ǰ����\n");
				else if (testans == INFEASIBLE)
					printf("���Ա����ڣ�\n");
				getchar();
				getchar();
				break;
			case 9:
				printf("������Ҫ��ѯ��̵�Ԫ�ص�ֵ��");
				scanf("%d", &e);
				testans = NextElem(L, e, next);
				if (testans == OK)
					printf("Ԫ��%d�ĺ��Ϊ%d��\n", e, next);
				else if (testans == ERROR)
					printf("��Ԫ��û�к�̣�\n");
				else if (testans == INFEASIBLE)
					printf("���Ա����ڣ�\n");
				getchar();
				getchar();
				break;
			case 10:
				printf("���������λ�úͲ���Ԫ�ص�ֵ���ÿո������");
				scanf("%d %d", &i, &e);
				testans = ListInsert(L, i, e);
				if (testans == OK)
					printf("����ɹ���\n");
				else if (testans == ERROR)
					printf("����λ�ò���ȷ��\n");
				else if (testans == INFEASIBLE)
					printf("���Ա����ڣ�\n");
				getchar();
				getchar();
				break;
			case 11:
				printf("������Ҫɾ����Ԫ�ص�λ�ã�");
				scanf("%d", &i);
				testans = ListDelete(L, i, e);
				if (testans == OK)
					printf("ɾ���ɹ���ɾ����Ԫ����%d��\n", e);
				else if (testans == ERROR)
					printf("ɾ��λ�ò���ȷ��\n");
				else if (testans == INFEASIBLE)
					printf("���Ա����ڣ�\n");
				getchar();
				getchar();
				break;
			case 12:
				if (ListTraverse(L) == INFEASIBLE)
					printf("���Ա����ڣ�\n");
				getchar();
				getchar();
				break;
			case 13:
				printf("������Ŀ���ļ�����");
				scanf("%s", FileName);
				if (L.elem) {
					testans = SaveList(L, FileName);
					if (testans == OK)
						printf("���Ա��е�Ԫ���ѳɹ�д���ļ�%s��\n", FileName);
					else
						printf("д�ļ�ʧ�ܣ�\n");
				} else {
					testans = LoadList(L, FileName);
					if (testans == OK)
						printf("�ļ�%s�е������ѳɹ����뵽���Ա��У�\n", FileName);
					else
						printf("�ļ�д��ʧ�ܣ�\n");
				}
				getchar();
				getchar();
				break;
			case 14:
				if (MaxSubArray(L) == ERROR)
					printf("���Ա�Ϊ�գ�\n");
				else if (MaxSubArray(L) == INFEASIBLE)
					printf("���Ա����ڣ�\n");
				else
					printf("���Ա��������������������%d\n", MaxSubArray(L));
				getchar();
				getchar();
				break;
			case 15:
				printf("��������Ҫ��������������ĺͣ�");
				scanf("%d", &knum);
				if (SubArrayNum(L, knum) == ERROR)
					printf("���Ա��в��������������������飡\n");
				else if (SubArrayNum(L, knum) == INFEASIBLE)
					printf("���Ա����ڣ�\n");
				else
					printf("���Ա��к�Ϊ%d������������ĸ�����%d\n", knum, SubArrayNum(L, knum));
				getchar();
				getchar();
				break;
			case 16:
				if (SortList(L) == ERROR)
					printf("���Ա�Ϊ�գ�\n");
				else if (SortList(L) == INFEASIBLE)
					printf("���Ա����ڣ�\n");
				else
					printf("���Ա���С����������ɣ�\n");
				getchar();
				getchar();
				break;
			case 17:
				printf("������Ŀ���ļ�����");
				scanf("%s", FileName);
				if (L.elem) {
					testans = AdvancedSave(L, FileName);
					if (testans == OK)
						printf("���Ա������߼��ṹ(D,{R})������Ϣ�ѳɹ�д���ļ�%s��\n", FileName);
					else if (testans == INFEASIBLE)
						printf("���Ա����ڣ�\n");
					else
						printf("д�ļ�ʧ�ܣ�\n");
				}
				getchar();
				getchar();
				break;
			case 18:
				printf("��ѡ���ܣ�1.���Ա���ӣ�2.���Ա��Ƴ���3.���Ա����\n");
				scanf("%d", &choice);
				if (choice == 1) {
					printf("���������Ա����ƣ�");
					scanf("%s", ListName);
					printf("��������������Ա��Ԫ�أ��ÿո������");
					scanf("%d %d %d %d", &i, &e, &pre, &next);
					if (AddList(Lists, ListName) == OK)
						printf("��Ϊ%s�����Ա��Ѽ������Ա�����", ListName);
				} else if (choice == 2) {
					printf("���������Ա����ƣ�");
					scanf("%s", ListName);
					if (RemoveList(Lists, ListName) == OK)
						printf("��Ϊ%s�����Ա�����", ListName);
				} else if (choice == 3) {
					printf("���������Ա����ƣ�");
					scanf("%s", ListName);
					if (LocateList(Lists, ListName) == 0)
						printf("���Ա����в�����������Ա�");
					else
						printf("�����Ա��λ����2");
				}
				getchar();
				getchar();
				break;
			case 0:
				break;
		}
	}
	printf("��ӭ�´���ʹ�ñ�ϵͳ��\n");
	return 0;
}