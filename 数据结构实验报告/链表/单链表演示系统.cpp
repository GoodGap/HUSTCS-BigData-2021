#include <stdio.h>
#include <string.h>
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
typedef int status;//����Ԫ�����Ͷ���
typedef int ElemType;

typedef struct LNode {
	ElemType data;
	struct LNode *next;
} LNode, *LinkList;// ����ڵ�ṹ����ָ�붨��

typedef struct {
	struct {
		char name[30];
		LinkList L;
	} elem[100];
	int length;
} LISTS; //�����Ͻṹ����

status InitList(LinkList &L)
// ���Ա�L�����ڣ�����һ���յ����Ա�����OK�����򷵻�INFEASIBLE��
{
	if (L)
		return INFEASIBLE;
	else {
		L = (LinkList)malloc(sizeof(LNode));
		L->next = NULL;
		return OK;
	}
}

status DestroyList(LinkList &L)
// ������Ա���ڣ��������Ա�L���ͷ�����Ԫ�ؿռ䣬����OK�����򷵻�INFEASIBLE��
{
	if (!L)
		return INFEASIBLE;
	else {
		while (L) {
			LinkList q;
			q = L->next;
			free(L);
			L = q;
		}
		L = NULL;
		return OK;
	}
}

status ClearList(LinkList &L)
// ������Ա�L���ڣ�ɾ�����Ա�L�е�����Ԫ�أ�����OK�����򷵻�INFEASIBLE��
{
	if (!L)
		return INFEASIBLE;
	else {
		LinkList q, p;
		p = L->next;
		while (p) {
			q = p->next;
			free(p);
			p = q;
		}
		L->next = NULL;
		return OK;
	}
}

status ListEmpty(LinkList L)
// ������Ա�L���ڣ��ж����Ա�L�Ƿ�Ϊ�գ��վͷ���TRUE�����򷵻�FALSE��
// ������Ա�L�����ڣ�����INFEASIBLE��
{
	if (!L)
		return INFEASIBLE;
	else {
		if (L->next == NULL)
			return TRUE;
		else
			return FALSE;
	}
}

int ListLength(LinkList L) {
// ������Ա�L���ڣ��������Ա�L�ĳ��ȣ����򷵻�INFEASIBLE��
	if (!L)
		return INFEASIBLE;
	else {
		int count = 0;
		LinkList p = L;
		while (p->next != NULL) {
			p = p->next;
			count++;
		}
		return count;
	}
}

status GetElem(LinkList L, int i, ElemType &e) {
// ������Ա�L���ڣ���ȡ���Ա�L�ĵ�i��Ԫ�أ�������e�У�����OK��
// ���i���Ϸ�������ERROR��������Ա�L�����ڣ�����INFEASIBLE��
	if (!L)
		return INFEASIBLE;
	else {
		if (i < 1)
			return ERROR;
		else {
			int count = 0, flag = 0;
			LinkList p = L;
			while (p->next != NULL) {
				p = p->next;
				count++;
				if (count == i) {
					flag = 1;
					e = p->data;
					break;
				}
			}
			if (flag == 0)
				return ERROR;
			else
				return OK;
		}
	}
}

status LocateElem(LinkList L, ElemType e) {
// ������Ա�L���ڣ�����Ԫ��e�����Ա�L�е�λ����ţ�
// ���e�����ڣ�����ERROR�������Ա�L������ʱ������INFEASIBLE��
	if (!L)
		return INFEASIBLE;
	else {
		int count = 0, flag = 0;
		LinkList p = L;
		while (p->next != NULL) {
			p = p->next;
			count++;
			if (p->data == e) {
				flag = 1;
				break;
			}
		}
		if (flag == 0)
			return ERROR;
		else
			return count;
	}
}

status PriorElem(LinkList L, ElemType e, ElemType &pre) {
// ������Ա�L���ڣ���ȡ���Ա�L��Ԫ��e��ǰ����������pre�У�����OK��
// ���û��ǰ��������ERROR��������Ա�L�����ڣ�����INFEASIBLE��
	if (!L)
		return INFEASIBLE;
	else {
		LinkList p = L;
		int flag = 0;
		while (p->next != NULL) {
			p = p->next;
			if (p->next != NULL && p->next->data == e) {
				flag = 1;
				pre = p->data;
				break;
			}
		}
		if (!flag)
			return ERROR;
		else
			return OK;
	}
}

status NextElem(LinkList L, ElemType e, ElemType &next) {
// ������Ա�L���ڣ���ȡ���Ա�LԪ��e�ĺ�̣�������next�У�����OK��
// ���û�к�̣�����ERROR��������Ա�L�����ڣ�����INFEASIBLE��
	if (!L)
		return INFEASIBLE;
	else {
		LinkList p = L;
		int flag = 0;
		while (p->next != NULL) {
			p = p->next;
			if (p->data == e && p->next != NULL) {
				flag = 1;
				next = p->next->data;
				break;
			}
		}
		if (!flag)
			return ERROR;
		else
			return OK;
	}
}

status ListInsert(LinkList &L, int i, ElemType e) {
// ������Ա�L���ڣ���Ԫ��e���뵽���Ա�L���ĵ�i��Ԫ��֮ǰ������OK��
// ������λ�ò���ȷʱ������ERROR��������Ա�L�����ڣ�����INFEASIBLE��
	if (!L)
		return INFEASIBLE;
	else {
		LinkList p = L;
		int pos = 0, flag;
		while (p->next != NULL) {
			pos++;
			p = p->next;
		}
		if ((L->next != NULL && i < 1) || i > pos + 1)
			flag = 0;
		else {
			pos = 0;
			p = L;
			while (pos < i - 1) {
				pos++;
				p = p->next;
			}
			LinkList newnode = (LinkList)malloc(sizeof(LNode));
			newnode->data = e;
			newnode->next = p->next;
			p->next = newnode;
			flag = 1;
		}

		if (flag)
			return OK;
		else
			return ERROR;
	}
}

status ListDelete(LinkList &L, int i, ElemType &e) {
// ������Ա�L���ڣ�ɾ�����Ա�L�ĵ�i��Ԫ�أ���������e�У�����OK��
// ��ɾ��λ�ò���ȷʱ������ERROR��������Ա�L�����ڣ�����INFEASIBLE��
	if (!L)
		return INFEASIBLE;
	else {
		LinkList p = L, q;
		int pos = 0, flag = 1;
		while (p->next != NULL) {
			pos++;
			p = p->next;
		}
		if (i < 1 || i > pos)
			flag = 0;
		else {
			pos = 0;
			p = L;
			q = L->next;
			while (pos < i - 1) {
				pos++;
				p = p->next;
				q = q->next;
			}
			e = q->data;
			p->next = q->next;
			free(q);
		}

		if (flag)
			return OK;
		else
			return ERROR;
	}
}

status ListTraverse(LinkList L) {
// ������Ա�L���ڣ�������ʾ���Ա��е�Ԫ�أ�ÿ��Ԫ�ؼ��һ�񣬷���OK��
// ������Ա�L�����ڣ�����INFEASIBLE��
	if (!L)
		return INFEASIBLE;
	else {
		LinkList p = L->next;
		if (p != NULL) {
			printf("\n-----------all elements -----------------------\n");
			while (p != NULL) {
				printf("%d ", p->data);
				p = p->next;
			}
			printf("\n------------------ end ------------------------\n");
		} else
			printf("���Ա�Ϊ�գ�\n");
		return OK;
	}
}

status SaveList(LinkList L, char FileName[]) {
// ������Ա���ڣ���L��Ԫ��д��FileName�ļ��У�����OK�����򷵻�INFEASIBLE
	if (!L)
		return INFEASIBLE;
	else {
		FILE *fp;
		fp = fopen(FileName, "w");
		LinkList p = L;
		while (p->next != NULL) {
			fprintf(fp, "%d ", p->next->data);
			p = p->next;
		}
		fclose(fp);
		return OK;
	}
}

status LoadList(LinkList &L, char FileName[]) {
// ������Ա����ڣ���FileName�����ݶ��뵽L�У�����OK�����򷵻�INFEASIBLE
	if (L)
		return INFEASIBLE;
	else {
		L = (LinkList)malloc(sizeof(LNode));
		LinkList p = L;
		FILE *fp;
		fp = fopen(FileName, "r");
		while (!feof(fp)) {
			LinkList newnode = (LinkList)malloc(sizeof(LNode));
			newnode->next = NULL;
			p->next = newnode;
			fscanf(fp, "%d", &newnode->data);
			if (newnode->data == 0)
				p->next = NULL;
			p = p->next;
		}
		fclose(fp);
		return OK;
	}
}

status ReverseList(LinkList L)
// ��ת����L���ɹ�����OK�����������ڣ�����INFEASIBLE
{
	if (!L)
		return INFEASIBLE;
	else {
		if (L->next == NULL)
			printf("�������ڣ�\n");
		else {
			int temp[10000];
			int count = 0;
			LinkList p = L->next;
			while (p != NULL) {
				temp[++count] = p->data;
				p = p->next;
			}
			p = L->next;
			while (p != NULL) {
				p->data = temp[count--];
				p = p->next;
			}
		}
		return OK;
	}
}

status RemoveNthFromEnd(LinkList L, int n)
// ɾ������ĵ�����n����㣬�ɹ�����OK�����������ڣ�����INFEASIBLE��
// ��n��ֵ�Ƿ�������ERROR��������Ϊ�գ�����-3
{
	if (!L)
		return INFEASIBLE;
	else {
		if (L->next == NULL)
			return -3;
		else {
			int count = 0, i, e;
			LinkList p = L->next;
			while (p != NULL) {
				count++;
				p = p->next;
			}
			if (n > count)
				return ERROR;
			else
				return ListDelete(L, count - n + 1, e);
		}
	}
}

status sortList(LinkList L)
// ������L��С�������򣬳ɹ�����OK�����������ڣ�����INFEASIBLE��
// ������Ϊ�գ�����ERROR
{
	if (!L)
		return INFEASIBLE;
	else {
		if (L->next == NULL)
			return ERROR;
		else {
			int temp[1000];
			int count = 0, t;
			LinkList p = L->next;
			while (p != NULL) {
				temp[++count] = p->data;
				p = p->next;
			}
			p = L->next;
			for (int i = 1; i < count; i++)
				for (int j = 1; j <= count - i; j++)
					if (temp[j + 1] < temp[j]) {
						t = temp[j];
						temp[j] = temp[j + 1];
						temp[j + 1] = t;
					}
			count = 0;
			while (p != NULL) {
				p->data = temp[++count];
				p = p->next;
			}
			return OK;
		}
	}
}

int main() {
	LinkList L;
	L = NULL;
	LISTS LLL;
	LLL.length = 0;
	int op = 1;
	int i, j, e, testans, pre, next;
	char FileName[30], ListName[30];
	while (op) {
		system("cls");
		printf("\n\n");
		printf("-------------------------------------------------\n");
		printf("          ������ʽ�洢�ṹ�����Ա�ʵ�� \n      ʵ����ʾϵͳ    �������2110��  �������\n");
		printf("-------------------------------------------------\n");
		printf("      1. InitList            2. DestroyList\n");
		printf("      3. ClearList           4. ListEmpty\n");
		printf("      5. ListLength          6. GetElem\n");
		printf("      7. LocateElem          8. PriorElem\n");
		printf("      9. NextElem            10. ListInsert\n");
		printf("      11. ListDelete         12. ListTraverse\n");
		printf("      13. Save/Load          14. ReverseList\n");
		printf("      15. RemoveNthFromEnd   16. SortList\n");
		printf("      17. MultiList          0. Exit\n");
		printf("-------------------------------------------------\n");
		printf("    ��ѡ�����[0~17]:\n");
		scanf("%d", &op);
		switch (op) {
			// ���·ֱ��Ӧ17�����ܣ������������ִ�ж�Ӧ�Ĳ���
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
					printf("���Ա����ڣ�\n");
				getchar();
				getchar();
				break;
			case 3:
				if (ClearList(L) == OK)
					printf("���Ա�����������Ԫ����ɾ�������Ա���ճɹ���\n");
				else
					printf("���Ա����ڣ�\n");
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
				if (L) {
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
				testans = ReverseList(L);
				if (testans == INFEASIBLE)
					printf("���Ա����ڣ�\n");
				else if (L->next != NULL)
					printf("����ת��ɣ�\n");
				getchar();
				getchar();
				break;
			case 15:
				printf("��������Ҫ�Ƴ���Ԫ���ǵ����ڼ�λ��");
				int choice;
				scanf("%d", &choice);
				testans = RemoveNthFromEnd(L, choice);
				if (testans == INFEASIBLE)
					printf("���Ա����ڣ�\n");
				else if (testans == ERROR)
					printf("����λ�ò��Ϸ���\n");
				else if (testans == -3)
					printf("����Ϊ�գ�\n");
				else
					printf("��Ԫ�����Ƴ���\n");
				getchar();
				getchar();
				break;
			case 16:
				testans = sortList(L);
				if (testans == INFEASIBLE)
					printf("���Ա����ڣ�\n");
				else if (testans == ERROR)
					printf("����Ϊ�գ�\n");
				else
					printf("�����Ѵ�С��������\n");
				getchar();
				getchar();
				break;
			case 17:
				printf("��ѡ���ܣ�1.���Ա���ӣ�2.���Ա��Ƴ���3.���Ա����\n");
				scanf("%d", &choice);
				if (choice == 1) {
					printf("���������Ա����ƣ�");
					scanf("%s", LLL.elem[LLL.length].name);
					printf("���������Ա�%s��Ԫ�ظ�����Ԫ��ֵ���Կո������");
					LLL.elem[LLL.length].L = NULL;
					int count;
					int tem[100];
					scanf("%d", &count);
					for (int i = 1; i <= count; i++)
						scanf("%d", &tem[i]);
					InitList(LLL.elem[LLL.length].L);
//					LLL.elem[LLL.length].L->next = NULL;
					for (int i = 1; i <= count; i++)
						ListInsert(LLL.elem[LLL.length].L, 1, tem[i]);
					ReverseList(LLL.elem[LLL.length].L);
					LLL.length++;
					printf("��Ϊ%s�����Ա����ɹ���\n", LLL.elem[LLL.length - 1].name);
				} else if (choice == 2) {
					if (LLL.length <= 0)
						printf("ɾ��ʧ�ܣ����Ա���Ϊ�գ�\n");
					else {
						printf("���������Ա����ƣ�");
						scanf("%s", ListName);
						int flag = 0;
						for (i = 0; i < LLL.length; i++)
							if (strcmp(LLL.elem[i].name, ListName) == 0) {
								flag = 1;
								break;
							}
						if (flag) {
							for (j = i; j = LLL.length - 1; j++) {
								LLL.elem[j].L = LLL.elem[j + 1].L;
								strcpy(LLL.elem[j].name, LLL.elem[j + 1].name);
							}
							LLL.length--;
							printf("��Ϊ%s�����Ա��Ѵ����Ա�����ɾ��", ListName);
						} else
							printf("���Ա�����û����Ϊ%s�����Ա�\n", ListName);
					}
				} else if (choice == 3) {
					if (LLL.length <= 0)
						printf("���Ա���Ϊ�գ�\n");
					else {
						printf("���������Ա����ƣ�");
						scanf("%s", ListName);
						int flag = 0;
						for (i = 0; i < LLL.length; i++)
							if (strcmp(LLL.elem[i].name, ListName) == 0) {
								flag = 1;
								break;
							}
						if (flag) {
							InitList(L);
							L = LLL.elem[i].L;
							printf("���ҳɹ���\n");
						} else
							printf("���Ա�����û����Ϊ%s�����Ա�\n", ListName);
					}
				} else
					printf("������ѡ��\n");
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