#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

//各类常量定义如下
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
#define LIST_INIT_SIZE 100
#define LISTINCREMENT  10
typedef int status;//数据元素类型定义
typedef int ElemType;

typedef struct LNode {
	ElemType data;
	struct LNode *next;
} LNode, *LinkList;// 链表节点结构及其指针定义

typedef struct {
	struct {
		char name[30];
		LinkList L;
	} elem[100];
	int length;
} LISTS; //链表集合结构定义

status InitList(LinkList &L)
// 线性表L不存在，构造一个空的线性表，返回OK，否则返回INFEASIBLE。
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
// 如果线性表存在，销毁线性表L，释放数据元素空间，返回OK，否则返回INFEASIBLE。
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
// 如果线性表L存在，删除线性表L中的所有元素，返回OK，否则返回INFEASIBLE。
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
// 如果线性表L存在，判断线性表L是否为空，空就返回TRUE，否则返回FALSE；
// 如果线性表L不存在，返回INFEASIBLE。
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
// 如果线性表L存在，返回线性表L的长度，否则返回INFEASIBLE。
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
// 如果线性表L存在，获取线性表L的第i个元素，保存在e中，返回OK；
// 如果i不合法，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
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
// 如果线性表L存在，查找元素e在线性表L中的位置序号；
// 如果e不存在，返回ERROR；当线性表L不存在时，返回INFEASIBLE。
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
// 如果线性表L存在，获取线性表L中元素e的前驱，保存在pre中，返回OK；
// 如果没有前驱，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
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
// 如果线性表L存在，获取线性表L元素e的后继，保存在next中，返回OK；
// 如果没有后继，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
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
// 如果线性表L存在，将元素e插入到线性表L‘的第i个元素之前，返回OK；
// 当插入位置不正确时，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
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
// 如果线性表L存在，删除线性表L的第i个元素，并保存在e中，返回OK；
// 当删除位置不正确时，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
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
// 如果线性表L存在，依次显示线性表中的元素，每个元素间空一格，返回OK；
// 如果线性表L不存在，返回INFEASIBLE。
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
			printf("线性表为空！\n");
		return OK;
	}
}

status SaveList(LinkList L, char FileName[]) {
// 如果线性表存在，将L的元素写到FileName文件中，返回OK，否则返回INFEASIBLE
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
// 如果线性表不存在，将FileName的数据读入到L中，返回OK，否则返回INFEASIBLE
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
// 反转链表L，成功返回OK；若链表不存在，返回INFEASIBLE
{
	if (!L)
		return INFEASIBLE;
	else {
		if (L->next == NULL)
			printf("链表不存在！\n");
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
// 删除链表的倒数第n个结点，成功返回OK；若链表不存在，返回INFEASIBLE；
// 若n的值非法，返回ERROR；若链表为空，返回-3
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
// 将链表L由小到大排序，成功返回OK；若链表不存在，返回INFEASIBLE；
// 若链表为空，返回ERROR
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
		printf("          基于链式存储结构的线性表实现 \n      实验演示系统    （计算机2110班  李嘉鹏）\n");
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
		printf("    请选择操作[0~17]:\n");
		scanf("%d", &op);
		switch (op) {
			// 以下分别对应17个功能，根据输入情况执行对应的操作
			case 1:
				if (InitList(L) == OK)
					printf("线性表创建成功！\n");
				else
					printf("线性表创建失败！\n");
				getchar();
				getchar();
				break;
			case 2:
				if (DestroyList(L) == OK)
					printf("数据元素空间释放完成，线性表销毁成功！\n");
				else
					printf("线性表不存在！\n");
				getchar();
				getchar();
				break;
			case 3:
				if (ClearList(L) == OK)
					printf("线性表中所有数据元素已删除，线性表清空成功！\n");
				else
					printf("线性表不存在！\n");
				getchar();
				getchar();
				break;
			case 4:
				if (ListEmpty(L) == TRUE)
					printf("线性表为空！\n");
				else if (ListEmpty(L) == FALSE)
					printf("线性表不为空！\n");
				else if (ListEmpty(L) == INFEASIBLE)
					printf("线性表不存在！\n");
				getchar();
				getchar();
				break;
			case 5:
				if (ListLength(L) == INFEASIBLE)
					printf("线性表不存在！\n");
				else
					printf("线性表的长度为%d！\n", ListLength(L));
				getchar();
				getchar();
				break;
			case 6:
				printf("请输入要查询的元素位置序号：");
				scanf("%d", &i);
				testans = GetElem(L, i, e);
				if (testans == OK)
					printf("线性表的第%d个元素为%d！\n", i, e);
				else if (testans == ERROR)
					printf("所给i值不合法！\n");
				else if (testans == INFEASIBLE)
					printf("线性表不存在！\n");
				getchar();
				getchar();
				break;
			case 7:
				printf("请输入要查询的元素的值：");
				scanf("%d", &e);
				testans = LocateElem(L, e);
				if (testans == ERROR)
					printf("所给元素不存在！\n");
				else if (testans == INFEASIBLE)
					printf("线性表不存在！\n");
				else
					printf("元素%d在线性表中的位置序号为%d！\n", e, testans);
				getchar();
				getchar();
				break;
			case 8:
				printf("请输入要查询前驱的元素的值：");
				scanf("%d", &e);
				testans = PriorElem(L, e, pre);
				if (testans == OK)
					printf("元素%d的前驱为%d！\n", e, pre);
				else if (testans == ERROR)
					printf("该元素没有前驱！\n");
				else if (testans == INFEASIBLE)
					printf("线性表不存在！\n");
				getchar();
				getchar();
				break;
			case 9:
				printf("请输入要查询后继的元素的值：");
				scanf("%d", &e);
				testans = NextElem(L, e, next);
				if (testans == OK)
					printf("元素%d的后继为%d！\n", e, next);
				else if (testans == ERROR)
					printf("该元素没有后继！\n");
				else if (testans == INFEASIBLE)
					printf("线性表不存在！\n");
				getchar();
				getchar();
				break;
			case 10:
				printf("请输入插入位置和插入元素的值，用空格隔开：");
				scanf("%d %d", &i, &e);
				testans = ListInsert(L, i, e);
				if (testans == OK)
					printf("插入成功！\n");
				else if (testans == ERROR)
					printf("插入位置不正确！\n");
				else if (testans == INFEASIBLE)
					printf("线性表不存在！\n");
				getchar();
				getchar();
				break;
			case 11:
				printf("请输入要删除的元素的位置：");
				scanf("%d", &i);
				testans = ListDelete(L, i, e);
				if (testans == OK)
					printf("删除成功，删除的元素是%d！\n", e);
				else if (testans == ERROR)
					printf("删除位置不正确！\n");
				else if (testans == INFEASIBLE)
					printf("线性表不存在！\n");
				getchar();
				getchar();
				break;
			case 12:
				if (ListTraverse(L) == INFEASIBLE)
					printf("线性表不存在！\n");
				getchar();
				getchar();
				break;
			case 13:
				printf("请输入目标文件名：");
				scanf("%s", FileName);
				if (L) {
					testans = SaveList(L, FileName);
					if (testans == OK)
						printf("线性表中的元素已成功写入文件%s！\n", FileName);
					else
						printf("写文件失败！\n");
				} else {
					testans = LoadList(L, FileName);
					if (testans == OK)
						printf("文件%s中的数据已成功读入到线性表中！\n", FileName);
					else
						printf("文件写入失败！\n");
				}
				getchar();
				getchar();
				break;
			case 14:
				testans = ReverseList(L);
				if (testans == INFEASIBLE)
					printf("线性表不存在！\n");
				else if (L->next != NULL)
					printf("链表反转完成！\n");
				getchar();
				getchar();
				break;
			case 15:
				printf("请输入想要移除的元素是倒数第几位：");
				int choice;
				scanf("%d", &choice);
				testans = RemoveNthFromEnd(L, choice);
				if (testans == INFEASIBLE)
					printf("线性表不存在！\n");
				else if (testans == ERROR)
					printf("所给位置不合法！\n");
				else if (testans == -3)
					printf("链表为空！\n");
				else
					printf("该元素已移除！\n");
				getchar();
				getchar();
				break;
			case 16:
				testans = sortList(L);
				if (testans == INFEASIBLE)
					printf("线性表不存在！\n");
				else if (testans == ERROR)
					printf("链表为空！\n");
				else
					printf("链表已从小到大排序！\n");
				getchar();
				getchar();
				break;
			case 17:
				printf("请选择功能：1.线性表添加；2.线性表移除；3.线性表查找\n");
				scanf("%d", &choice);
				if (choice == 1) {
					printf("请输入线性表名称：");
					scanf("%s", LLL.elem[LLL.length].name);
					printf("请输入线性表%s的元素个数和元素值，以空格隔开：");
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
					printf("名为%s的线性表插入成功！\n", LLL.elem[LLL.length - 1].name);
				} else if (choice == 2) {
					if (LLL.length <= 0)
						printf("删除失败，线性表集合为空！\n");
					else {
						printf("请输入线性表名称：");
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
							printf("名为%s的线性表已从线性表集合中删除", ListName);
						} else
							printf("线性表集合中没有名为%s的线性表！\n", ListName);
					}
				} else if (choice == 3) {
					if (LLL.length <= 0)
						printf("线性表集合为空！\n");
					else {
						printf("请输入线性表名称：");
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
							printf("查找成功！\n");
						} else
							printf("线性表集合中没有名为%s的线性表！\n", ListName);
					}
				} else
					printf("请重新选择！\n");
				getchar();
				getchar();
				break;
			case 0:
				break;
		}
	}
	printf("欢迎下次再使用本系统！\n");
	return 0;
}