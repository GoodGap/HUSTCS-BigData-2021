#include <stdio.h>
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
typedef int status;//数据元素种类定义
typedef int ElemType;

typedef struct {
	ElemType *elem;
	int length;
	int listsize;
} SqList;//线性表结构定义

typedef struct {
	struct {
		char name[30];
		SqList L;
	} elem[10];
	int length;
} LISTS;//线性表集合结构定义

LISTS Lists;

status InitList(SqList &L) {
	// 线性表L不存在，构造一个空的线性表，返回OK，否则返回INFEASIBLE。
	if (!L.elem) {
		L.elem = (ElemType *)malloc(LIST_INIT_SIZE * sizeof(ElemType));
		L.length = 0;
		L.listsize = LIST_INIT_SIZE;
		return OK;
	} else
		return INFEASIBLE;
}

status DestroyList(SqList &L) {
	// 如果线性表L存在，销毁线性表L，释放数据元素的空间，返回OK，否则返回INFEASIBLE。
	if (!L.elem)
		return INFEASIBLE;
	else {
		free(L.elem);
		L.elem = NULL;
		return OK;
	}
}

status ClearList(SqList &L) {
	// 如果线性表L存在，删除线性表L中的所有元素，返回OK，否则返回INFEASIBLE。
	if (!L.elem)
		return INFEASIBLE;
	else {
		L.length = 0;
		return OK;
	}
}

status ListEmpty(SqList L) {
	// 如果线性表L存在，判断线性表L是否为空，空就返回TRUE，否则返回FALSE；
	// 如果线性表L不存在，返回INFEASIBLE。
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
	// 如果线性表L存在，返回线性表L的长度，否则返回INFEASIBLE。
	if (!L.elem)
		return INFEASIBLE;
	else
		return L.length;
}

status GetElem(SqList L, int i, ElemType &e) {
	// 如果线性表L存在，获取线性表L的第i个元素，保存在e中，返回OK；
	// 如果i不合法，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
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
	// 如果线性表L存在，查找元素e在线性表L中的位置序号并返回该序号；
	// 如果e不存在，返回0；当线性表L不存在时，返回INFEASIBLE（即-1）。
	if (!L.elem)
		return INFEASIBLE;
	else {
		int flag = -1;
		for (int i = 0; i < L.length; i++)
			if (L.elem[i] == e) {
				// 遍历线性表中元素，若有相同就跳出
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
	// 如果线性表L存在，获取线性表L中元素e的前驱，保存在pre中，返回OK；
	// 如果没有前驱，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
	if (!L.elem)
		return INFEASIBLE;
	else {
		int loc;
		for (loc = 0; loc < L.length; loc++)
			if (L.elem[loc] == e)
				break;// 搜索元素位置
		if (loc == 0 || loc == L.length)
			return ERROR;// 元素位置为首位
		else {
			pre = L.elem[loc - 1];
			return OK;
		}
	}
}

status NextElem(SqList L, ElemType e, ElemType &next) {
	// 如果线性表L存在，获取线性表L元素e的后继，保存在next中，返回OK；
	// 如果没有后继，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
	if (!L.elem)
		return INFEASIBLE;
	else {
		int loc;
		for (loc = 0; loc < L.length; loc++)
			if (L.elem[loc] == e)
				break;// 搜索元素位置
		if (loc == L.length - 1 || loc == L.length)
			return ERROR;// 元素位置为末位
		else {
			next = L.elem[loc + 1];
			return OK;
		}
	}
}

status ListInsert(SqList &L, int i, ElemType e) {
	// 如果线性表L存在，将元素e插入到线性表L的第i个元素之前，返回OK；
	// 当插入位置不正确时，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
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
					// 如果L.length已经达到最大，则需要扩容
				}
				for (int j = L.length - 2; j >= i - 1; j--)
					L.elem[j + 1] = L.elem[j];
				// 将后面的元素全部后移一位
				L.elem[i - 1] = e;
			}
			return OK;
		}
	}
}

status ListDelete(SqList &L, int i, ElemType &e) {
	// 如果线性表L存在，删除线性表L的第i个元素，并保存在e中，返回OK；
	// 当删除位置不正确时，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
	if (!L.elem)
		return INFEASIBLE;
	else {
		if (i < 1 || i > L.length)
			return ERROR;
		else {
			e = L.elem[i - 1];
			for (int j = i - 1; j < L.length - 1; j++)
				L.elem[j] = L.elem[j + 1];
			// 将后面的元素全部前移一位
			L.length--;
			return OK;
		}
	}
}

status ListTraverse(SqList L) {
	// 如果线性表L存在，依次显示线性表中的元素，每个元素间空一格，返回OK；
	// 如果线性表L不存在，返回INFEASIBLE。
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
			printf("线性表为空！\n");
		return OK;
	}
}

status  SaveList(SqList L, char FileName[]) {
	// 如果线性表L存在，将L的元素写到FileName文件中，返回OK，否则返回INFEASIBLE。
	if (!L.elem)
		return INFEASIBLE;
	else {
		FILE *fp;
		fp = fopen(FileName, "w");
		for (int i = 0; i < L.length; i++)
			fprintf(fp, "%d ", L.elem[i]);
		// 不断读入数字，直到读入了L.length个
		fclose(fp);
		return OK;
	}
}

status  LoadList(SqList &L, char FileName[]) {
	// 如果线性表L不存在，将FileName文件中的元素写入L，返回OK，否则返回INFEASIBLE。
	if (L.elem)
		return INFEASIBLE;
	else {
		FILE *fp;
		fp = fopen(FileName, "r");
		L.elem = (ElemType *)malloc(LIST_INIT_SIZE * sizeof(ElemType));
		// 创建新的数据空间
		int len = 0;
		while (!feof(fp))
			fscanf(fp, "%d", &L.elem[len++]);
		// 从文件中写入数据到线性表中，直到文件结束
		L.length = len - 1;
		fclose(fp);
		return OK;
	}
}

status AddList(LISTS &Lists, char ListName[]) {
	// 在Lists中增加一个名称为ListName的空线性表
	Lists.elem[Lists.length].L.elem = NULL;
	Lists.elem[Lists.length].L.length = 0;
	InitList(Lists.elem[Lists.length++].L);
	// 线性表集合长度+1
	int t = 0;
	while (ListName[t] != 0) {
		Lists.elem[Lists.length - 1].name[t] = ListName[t];
		t++;
	}
	return OK;
}

status RemoveList(LISTS &Lists, char ListName[]) {
	// 在Lists中删除一个名称为ListName的线性表，成功返回OK，否则返回ERROR
	if (Lists.length < 1)
		return ERROR;
	else {
		int i, j, k, flag = 0;
		for (i = 1; i <= Lists.length; i++) {
			// 遍历线性表名称
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
		// 如果搜索成功则flag置为1，进行后续删除操作
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
			// 后续线性表位置全部前移一位
			Lists.length--;
			return OK;
		} else
			return ERROR;
	}
}

int LocateList(LISTS Lists, char ListName[]) {
	// 在Lists中查找一个名为ListName的线性表，成功返回其序号，否则返回0
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
		// 查找原理同上
		if (flag == 1) {
			return i;
		} else
			return 0;
	}
}

status MaxSubArray(SqList L) {
	// 在线性表L中求其连续子数组最大和，并返回这个值
	if (!L.elem)
		return INFEASIBLE;
	else {
		if (!L.length)
			return ERROR;
		else {
			int max = -9999999, tmax; //tmax用来暂存
			for (int i = 1; i <= L.length; i++)
				// 按子数组中所含元素个数进行遍历
				for (int j = 1; j <= L.length - i + 1; j++) {
					tmax = 0;
					for (int k = j; k <= j + i - 1; k++)
						tmax += L.elem[k - 1];
					if (tmax > max)
						max = tmax;// 不断更新最大值
				}
			return max;
		}
	}
}

status SubArrayNum(SqList L, int k) {
	// 在线性表L中搜索和为一定值的连续子数组，成功返回其个数，否则返回ERROR
	if (!L.elem)
		return INFEASIBLE;
	else {
		if (!L.length)
			return ERROR;
		else {
			int tmax, count = 0;// count用来计数
			for (int i = 1; i <= L.length; i++)
				// 按子数组中所含元素个数进行遍历
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
	// 对线性表L进行升序排序，成功返回OK，否则返回ERROR
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
			// 使用冒泡排序
			return OK;
		}
	}
}

status  AdvancedSave(SqList L, char FileName[]) {
	// 对线性表L进行文件的存取功能
	if (!L.elem)
		return INFEASIBLE;
	else {
		FILE *fp;
		fp = fopen(FileName, "w");
		fprintf(fp, "线性表表长为：%d\n", L.length);
		fprintf(fp, "线性表存储方式为：顺序存储（数组）。\n其元素为：\n");
		fprintf(fp, "(首元素) ");
		for (int i = 0; i < L.length; i++)
			fprintf(fp, "%d ", L.elem[i]);
		fprintf(fp, " (尾元素)");
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
//	LA.length = 0;// 对线性表集合进行初始化
	while (op) {
		system("cls");
		printf("\n\n");
		printf("-------------------------------------------------\n");
		printf("          基于顺序存储结构的线性表实现 \n      实验演示系统    （计算机2110班  李嘉鹏）\n");
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
		printf("    请选择你的操作[0~18]:\n");
		scanf("%d", &op);
		switch (op) {
			// 以下分别对应18个功能，根据输入情况执行对应的操作
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
					printf("线性表销毁失败！\n");
				getchar();
				getchar();
				break;
			case 3:
				if (ClearList(L) == OK)
					printf("线性表中所有数据元素已删除，线性表清空成功！\n");
				else
					printf("线性表清空失败！\n");
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
				if (L.elem) {
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
				if (MaxSubArray(L) == ERROR)
					printf("线性表为空！\n");
				else if (MaxSubArray(L) == INFEASIBLE)
					printf("线性表不存在！\n");
				else
					printf("线性表中连续子数组的最大和是%d\n", MaxSubArray(L));
				getchar();
				getchar();
				break;
			case 15:
				printf("请输入想要查找连续子数组的和：");
				scanf("%d", &knum);
				if (SubArrayNum(L, knum) == ERROR)
					printf("线性表中不存在这样的连续子数组！\n");
				else if (SubArrayNum(L, knum) == INFEASIBLE)
					printf("线性表不存在！\n");
				else
					printf("线性表中和为%d的连续子数组的个数是%d\n", knum, SubArrayNum(L, knum));
				getchar();
				getchar();
				break;
			case 16:
				if (SortList(L) == ERROR)
					printf("线性表为空！\n");
				else if (SortList(L) == INFEASIBLE)
					printf("线性表不存在！\n");
				else
					printf("线性表由小到大排序完成！\n");
				getchar();
				getchar();
				break;
			case 17:
				printf("请输入目标文件名：");
				scanf("%s", FileName);
				if (L.elem) {
					testans = AdvancedSave(L, FileName);
					if (testans == OK)
						printf("线性表数据逻辑结构(D,{R})完整信息已成功写入文件%s！\n", FileName);
					else if (testans == INFEASIBLE)
						printf("线性表不存在！\n");
					else
						printf("写文件失败！\n");
				}
				getchar();
				getchar();
				break;
			case 18:
				printf("请选择功能：1.线性表添加；2.线性表移除；3.线性表查找\n");
				scanf("%d", &choice);
				if (choice == 1) {
					printf("请输入线性表名称：");
					scanf("%s", ListName);
					printf("请依次输入该线性表的元素，用空格隔开：");
					scanf("%d %d %d %d", &i, &e, &pre, &next);
					if (AddList(Lists, ListName) == OK)
						printf("名为%s的线性表已加入线性表集合中", ListName);
				} else if (choice == 2) {
					printf("请输入线性表名称：");
					scanf("%s", ListName);
					if (RemoveList(Lists, ListName) == OK)
						printf("名为%s的线性表不存在", ListName);
				} else if (choice == 3) {
					printf("请输入线性表名称：");
					scanf("%s", ListName);
					if (LocateList(Lists, ListName) == 0)
						printf("线性表集合中不存在这个线性表！");
					else
						printf("该线性表的位置是2");
				}
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