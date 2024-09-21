#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

// 各类常量定义如下
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2

typedef int status;
typedef int KeyType;
typedef struct {
	KeyType  key;
	char others[20];
} TElemType; //二叉树结点类型定义

typedef struct BiTNode { //二叉链表结点定义
	TElemType  data;
	struct BiTNode *lchild, *rchild;
} BiTNode, *BiTree;

typedef struct {
	struct {
		char name[30];
		BiTree T;
	} elem[100];
	int length;
} TREES; //二叉树集合结构定义

int count = 0;// 统计已经创建了几个结点
int dep[1000] = {0};

BiTree CreateBiTNode(TElemType definition[]) {
// 创建一个新的二叉链表结点
	if (definition[count].key) {
		BiTree newnode = (BiTree)malloc(sizeof(BiTNode));
		newnode->data = definition[count++];
		newnode->lchild = CreateBiTNode(definition);
		newnode->rchild = CreateBiTNode(definition);
		return newnode;
	} else {
		count++;
		return NULL;
	}
}

status CreateBiTree(BiTree &T, TElemType definition[])
/*根据带空枝的二叉树先根遍历序列definition构造一棵二叉树，将根节点指针赋值给T并返回OK，
如果有相同的关键字，返回ERROR。此题允许通过增加其它函数辅助实现本关任务*/
{
	if (T)
		return INFEASIBLE;
	int check[1000];
	for (int i = 0; definition[i].key != -1; i++) {
		if (definition[i].key != 0 && check[definition[i].key] == 1)
			return ERROR;
		check[definition[i].key] = 1;
	}
	count = 0;
	T = CreateBiTNode(definition);
	return OK;
}

status ClearBiTree(BiTree &T)
//将二叉树设置成空，并删除所有结点，释放结点空间
{
	if (T) {
		if (T->lchild) {
			ClearBiTree(T->lchild);
		}
		if (T->rchild) {
			ClearBiTree(T->rchild);
		}
		free(T);
		T = NULL;
		return OK;
	} else
		return INFEASIBLE;
}

int BiTreeDepth(BiTree T)
//求二叉树T的深度
{
	if (T == NULL)
		return 0;
	else {
		if (T->lchild != NULL && T->rchild != NULL)
			return (BiTreeDepth(T->lchild) > BiTreeDepth(T->rchild) ? BiTreeDepth(T->lchild) : BiTreeDepth(T->rchild)) + 1;
		else if (T->lchild == NULL && T->rchild != NULL)
			return BiTreeDepth(T->rchild) + 1;
		else if (T->rchild == NULL && T->lchild != NULL)
			return BiTreeDepth(T->lchild) + 1;
		else
			return 1;
	}
}

BiTNode *LocateNode(BiTree T, KeyType e)
//查找结点
{
	if (!T)
		return NULL;
	else {
		BiTree temp;
		if (e == T->data.key)
			return T;
		else {
			if (T->lchild) {
				temp = LocateNode(T->lchild, e);
				if (temp)
					return temp;
			}
			if (T->rchild) {
				temp = LocateNode(T->rchild, e);
				if (temp)
					return temp;
			}
			return NULL;
		}
	}
}

status check(BiTree T, KeyType newvalue, KeyType key)
// 检查新的赋值是否会产生冲突
{
	int r = 1;
	if (T) {
		if (T->data.key == newvalue && T->data.key != key) {
			r = 0;
			return r;
		}
		r = check(T->lchild, newvalue, key);
		if (!r)
			return r;
		r = check(T->rchild, newvalue, key);
		if (!r)
			return r;
	}
	return r;
}

status Assign(BiTree &T, KeyType e, TElemType value)
//实现结点赋值。此题允许通过增加其它函数辅助实现本关任务
{
	BiTree ans;
	ans = LocateNode(T, e);
	if (ans) {
		if (check(T, value.key, e)) {
			ans->data = value;
			return OK;
		} else
			return ERROR;// 复制后关键字不唯一
	} else
		return ERROR;
}

BiTNode *GetSibling(BiTree T, KeyType e)
//实现获得兄弟结点
{
	if (T->lchild && T->lchild->data.key == e) {
		if (T->rchild)
			return T->rchild;
	} else if (T->rchild && T->rchild->data.key == e) {
		if (T->lchild)
			return T->lchild;
	} else {
		BiTree temp;
		if (T->lchild) {
			temp = GetSibling(T->lchild, e);
			if (temp)
				return temp;
		}
		if (T->rchild) {
			temp = GetSibling(T->rchild, e);
			if (temp)
				return temp;
		}
	}
	return NULL;
}

status check1(BiTree T, KeyType newvalue)
// 检查关键字是否冲突，若返回0则说明冲突
{
	int r = 1;
	if (T) {
		if (T->data.key == newvalue) {
			r = 0;
			return r;
		}
		r = check1(T->lchild, newvalue);
		if (!r)
			return r;
		r = check1(T->rchild, newvalue);
		if (!r)
			return r;
	}
	return r;
}

status InsertNode(BiTree &T, KeyType e, int LR, TElemType c)
//插入结点。此题允许通过增加其它函数辅助实现本关任务
{
	BiTree target, newnode;
	if (LR == -1) { // c作为根结点插入，原根结点作为c的右子树
		BiTree newnode = (BiTree)malloc(sizeof(BiTNode));
		newnode->data = c;
		newnode->rchild = T;
		newnode->lchild = NULL;
		T = newnode;
		return OK;
	} else {
		target = LocateNode(T, e);
		if (!target)
			return ERROR;
		else if (!check1(T, c.key))
			return ERROR;
		else {
			if (LR == 0) { // 插入结点c到T中，作为关键字为e的结点的左孩子
				if (!T)
					return INFEASIBLE;
				BiTree newnode = (BiTree)malloc(sizeof(BiTNode));
				newnode->data = c;
				newnode->rchild = target->lchild;
				target->lchild = newnode;
				newnode->lchild = NULL;
				return OK;
			} else { // 插入结点c到T中，作为关键字为e的结点的右孩子
				if (!T)
					return INFEASIBLE;
				BiTree newnode = (BiTree)malloc(sizeof(BiTNode));
				newnode->data = c;
				newnode->rchild = target->rchild;
				target->rchild = newnode;
				newnode->lchild = NULL;
				return OK;
			}
		}
	}
}

BiTNode *LocateParent(BiTree T, KeyType e) {
	if (!T)
		return NULL;
	else {
		if ((T->lchild && T->lchild->data.key == e) || (T->rchild && T->rchild->data.key == e))
			return T;
		else {
			BiTree temp = NULL;
			temp = LocateParent(T->lchild, e);
			if (temp)
				return temp;
			temp = NULL;
			temp = LocateParent(T->rchild, e);
			return temp;
		}
	}
}

status DeleteNode(BiTree &T, KeyType e)
//删除结点。此题允许通过增加其它函数辅助实现本关任务
{
	BiTree target = LocateNode(T, e);
	BiTree temp, parent;
	if (!target)
		return ERROR;
	else {
		if (target->lchild == NULL && target->rchild == NULL) {
			parent = LocateParent(T, e);
			if (parent) {
				if (parent->lchild == target)
					parent->lchild = NULL;
				else
					parent->rchild = NULL;
			} else // 删除的点是根节点
				T = temp;
			free(target);
		} else if (target->lchild != NULL && target->rchild == NULL) {
			temp = target->lchild;
			parent = LocateParent(T, e);
			if (parent) {
				if (parent->lchild == target)
					parent->lchild = temp;
				else
					parent->rchild = temp;
			} else
				T = temp;
			free(target);
		} else if (target->rchild != NULL && target->lchild == NULL) {
			temp = target->rchild;
			parent = LocateParent(T, e);
			if (parent) {
				if (parent->lchild == target)
					parent->lchild = temp;
				else
					parent->rchild = temp;
			} else
				T = temp;
			free(target);
		} else {
			parent = LocateParent(T, e);
			temp = target->lchild;
			if (parent) {
				if (parent->lchild == target)
					parent->lchild = temp;
				if (parent->rchild == target)
					parent->rchild = temp;
			} else
				T = temp;
			temp = target->lchild;
			while (temp->rchild)
				temp = temp->rchild;
			temp->rchild = target->rchild;
			free(target);
		}
		return OK;
	}
}

void visit(BiTree T) {
	printf(" %d,%s", T->data.key, T->data.others);
}

status PreOrderTraverse(BiTree T, void (*visit)(BiTree))
//先序遍历二叉树T
{
	if (T) {
		visit(T);
		PreOrderTraverse(T->lchild, visit);
		PreOrderTraverse(T->rchild, visit);
	}
	return OK;
}

int top = 0; // 栈顶指针
BiTree stack[1000];

status InOrderTraverse(BiTree T, void (*visit)(BiTree))
//中序遍历二叉树T
{
	BiTree p = T;
	while (p || top) {
		if (p) { // 根指针进栈，遍历左子树
			stack[top++] = p;
			p = p->lchild;
		} else { // 根指针退栈，访问根节点并遍历右子树
			p = stack[--top];
			visit(p);
			p = p->rchild;
		}
	}
	return OK;
}

status PostOrderTraverse(BiTree T, void (*visit)(BiTree))
//后序遍历二叉树T
{
	if (T) {
		PostOrderTraverse(T->lchild, visit);
		PostOrderTraverse(T->rchild, visit);
		visit(T);
	}
	return OK;
}

status LevelOrderTraverse(BiTree T, void (*visit)(BiTree))
//按层遍历二叉树T
{
	BiTree que[1000];
	int front = 0;
	int rear = 1;
	if (T) {
		que[0] = T;
		while (rear - front > 0) {
			visit(que[front]);
			front++;
			if (que[front - 1]->lchild)
				que[rear++] = que[front - 1]->lchild;
			if (que[front - 1]->rchild)
				que[rear++] = que[front - 1]->rchild;
		}
	}
	return OK;
}

status Save(BiTree T, FILE *fp)
// 先序遍历存储
{
	if (T) {
		fprintf(fp, "%d %s\n", T->data.key, T->data.others);
		Save(T->lchild, fp);
		Save(T->rchild, fp);
	} else
		fprintf(fp, "0 null\n");
	return OK;
}

BiTree Load(FILE *fp)
// 先序遍历读取
{
	BiTree T;
	TElemType load;
	fscanf(fp, "%d %s", &load.key, load.others);
	if (load.key == 0)
		T = NULL;
	else {
		T = (BiTree)malloc(sizeof(BiTNode));
		T->data = load;
		T->lchild = Load(fp);
		T->rchild = Load(fp);
	}
	return T;
}

status SaveBiTree(BiTree T, char FileName[])
//将二叉树的结点数据写入到文件FileName中
{
	if (!T)
		return INFEASIBLE;
	else {
		FILE *fp;
		fp = fopen(FileName, "w");
		Save(T, fp);
		fclose(fp);
		return OK;
	}
}
status LoadBiTree(BiTree &T,  char FileName[])

//读入文件FileName的结点数据，创建二叉树
{
	if (T)
		return INFEASIBLE;
	else {
		FILE *fp;
		fp = fopen(FileName, "r");
		T = Load(fp);
		fclose(fp);
		return OK;
	}
}

status MaxPathSum(BiTree &T, int sum)
// 返回根节点到叶子结点的最大路径和
{
	if (!T->lchild && !T->rchild)
		return sum;
	else {
		if (T->lchild && T->rchild)
			return MaxPathSum(T->lchild, sum + T->lchild->data.key) > MaxPathSum(T->rchild,
			        sum + T->rchild->data.key) ? MaxPathSum(T->lchild, sum + T->lchild->data.key) : MaxPathSum(T->rchild,
			                sum + T->rchild->data.key);
		else if (!T->lchild)
			return MaxPathSum(T->lchild, sum + T->lchild->data.key);
		else
			return MaxPathSum(T->rchild, sum + T->rchild->data.key);
	}
}

void GetDepth(BiTree &T, int predep) {
	dep[T->data.key] = predep++;
	if (T->lchild)
		GetDepth(T->lchild, predep);
	if (T->rchild)
		GetDepth(T->rchild, predep);
}

BiTree LowestCommonAncestor(BiTree &T, KeyType e1, KeyType e2)
// 返回两个结点的最近公共祖先
{
	BiTree m, n;
	m = LocateNode(T, e1);
	n = LocateNode(T, e2);
	dep[1000] = {0};
	GetDepth(T, 1);
	if (dep[m->data.key] <= dep[n->data.key])
		return LocateParent(T, e1);
	else
		return LocateParent(T, e2);
}

void InvertTree(BiTree &T)
// 翻转二叉树
{
	if (T->lchild && T->rchild) {
		BiTree temp;
		temp = T->lchild;
		T->lchild = T->rchild;
		T->rchild = temp;
	}
	if (T->lchild)
		InvertTree(T->lchild);
	if (T->rchild)
		InvertTree(T->rchild);
}

int main() {
	int op = 1;
	int i, result, e, LR, e1, e2, choice, j;
	char FileName[30], TreeName[30];
	TElemType value, c;
	BiTree resultnode;
	BiTree T = NULL;
	TElemType definition[100];
	TREES TTT;
	TTT.length = 0;
	while (op) {
		system("cls");
		printf("\n\n");
		printf("-------------------------------------------------------\n");
		printf("             二叉树操作演示系统 \n            计算机2110班  李嘉鹏\n");
		printf("-------------------------------------------------------\n");
		printf("    1. CreateBitree            2. ClearBiTree\n");
		printf("    3. BiTreeDepth             4. LocateNode\n");
		printf("    5. Assign                  6. GetSibling\n");
		printf("    7. InsertNode              8. DeleteNode\n");
		printf("    9. PreOrderTraverse        10. InOrderTraverse\n");
		printf("    11. PostOrderTraverse      12. LevelOrderTraverse\n");
		printf("    13. Save/Load              14. MaxPathSum\n");
		printf("    15. LowestCommonAncestor   16. InvertTree\n");
		printf("    17. MultiTree              0. Exit\n");
		printf("-------------------------------------------------------\n");
		printf("    请选择操作[0~17]:\n");
		scanf("%d", &op);
		switch (op) {
			// 以下分别对应17个功能，根据输入情况执行对应的操作
			case 1:
				printf("请输入带空枝的二叉树先根遍历序列，以-1 null结束：\n");
				i = 0;
				do {
					scanf("%d%s", &definition[i].key, definition[i].others);
				} while (definition[i++].key != -1);
				if (CreateBiTree(T, definition) == OK)
					printf("二叉树创建成功！\n");
				else if (CreateBiTree(T, definition) == ERROR)
					printf("存在相同关键字，二叉树创建失败！\n");
				else
					printf("已存在二叉树，创建失败！\n");
				getchar();
				getchar();
				break;
			case 2:
				result = ClearBiTree(T);
				if (result == INFEASIBLE)
					printf("二叉树不存在！\n");
				if (result == OK)
					printf("二叉树清空成功！\n");
				getchar();
				getchar();
				break;
			case 3:
				result = BiTreeDepth(T);
				if (!result)
					printf("二叉树不存在（深度为0）！\n");
				else
					printf("二叉树的深度为%d！\n", result);
				getchar();
				getchar();
				break;
			case 4:
				if (!T)
					printf("二叉树不存在！\n");
				else {
					printf("请输入要查找结点的关键字：");
					scanf("%d", &e);
					resultnode = LocateNode(T, e);
					if (!resultnode)
						printf("二叉树中不存在关键字为%d的结点！\n", e);
					else
						printf("已找到关键字为%d的结点，结点值为%s！\n", e, resultnode->data.others);
				}
				getchar();
				getchar();
				break;
			case 5:
				if (!T)
					printf("二叉树不存在！\n");
				else {
					printf("请输入要赋值的结点关键字，以及新结点信息（关键字、结点值），用空格隔开：");
					scanf("%d %d %s", &e, &value.key, value.others);
					result = Assign(T, e, value);
					if (result == OK)
						printf("赋值成功！\n");
					else
						printf("赋值失败！\n");
				}
				getchar();
				getchar();
				break;
			case 6:
				if (!T)
					printf("二叉树不存在！\n");
				else {
					printf("请输入要查找兄弟结点的结点的关键字：");
					scanf("%d", &e);
					resultnode = GetSibling(T, e);
					if (!result)
						printf("关键字为%d的结点不存在兄弟结点！\n", e);
					else
						printf("关键字为%d的结点存在兄弟结点，结点值为%s！\n", e, resultnode->data.others);
				}
				getchar();
				getchar();
				break;
			case 7:
				if (!T)
					printf("二叉树不存在！\n");
				else {
					printf("请输入目标结点的关键字、LR和新结点信息（关键字、结点值），用空格隔开\n");
					printf("（LR为-1/0/1分别代表作为根结点或作为目标结点的左或右孩子插入）：");
					scanf("%d %d %d %s", &e, &LR, &c.key, c.others);
					result = InsertNode(T, e, LR, c);
					if (result == OK)
						printf("插入成功！\n");
					else
						printf("插入失败！\n", result);
				}
				getchar();
				getchar();
				break;
			case 8:
				if (!T)
					printf("二叉树不存在！\n");
				else {
					printf("请输入要删除结点的关键字：");
					scanf("%d", &e);
					result = DeleteNode(T, e);
					if (result == OK)
						printf("删除成功！\n");
					if (result == ERROR)
						printf("删除失败！\n", result);
				}
				getchar();
				getchar();
				break;
			case 9:
				if (!T)
					printf("二叉树不存在！\n");
				else {
					printf("\n-----------------先序遍历结果--------------------------\n");
					result = PreOrderTraverse(T, visit);
					if (result == OK)
						printf("\n\n----------------------end------------------------------\n");
					else
						printf("遍历失败！\n", result);
				}
				getchar();
				getchar();
				break;
			case 10:
				if (!T)
					printf("二叉树不存在！\n");
				else {
					printf("\n-----------------中序遍历结果--------------------------\n");
					result = InOrderTraverse(T, visit);
					if (result == OK)
						printf("\n\n----------------------end------------------------------\n");
					else
						printf("遍历失败！\n", result);
				}
				getchar();
				getchar();
				break;
			case 11:
				if (!T)
					printf("二叉树不存在！\n");
				else {
					printf("\n-----------------后序遍历结果--------------------------\n");
					result = PostOrderTraverse(T, visit);
					if (result == OK)
						printf("\n\n----------------------end------------------------------\n");
					else
						printf("遍历失败！\n", result);
				}
				getchar();
				getchar();
				break;
			case 12:
				if (!T)
					printf("二叉树不存在！\n");
				else {
					printf("\n-----------------层序遍历结果--------------------------\n");
					result = LevelOrderTraverse(T, visit);
					if (result == OK)
						printf("\n\n----------------------end------------------------------\n");
					else
						printf("遍历失败！\n", result);
				}
				getchar();
				getchar();
				break;
			case 13:
				printf("请输入目标文件名：");
				scanf("%s", FileName);
				if (T) {
					result = SaveBiTree(T, FileName);
					if (result == OK)
						printf("二叉树数据已成功写入文件%s！\n", FileName);
					else
						printf("写文件失败！\n");
				} else {
					result = LoadBiTree(T, FileName);
					if (result == OK)
						printf("文件%s中的数据已成功读入到二叉树中！\n", FileName);
					else
						printf("文件写入失败！\n");
				}
				getchar();
				getchar();
				break;
			case 14:
				if (!T)
					printf("二叉树不存在！\n");
				else {
					printf("根节点到叶子结点的最大路径和为%d！", MaxPathSum(T, 0));
				}
				getchar();
				getchar();
				break;
			case 15:
				if (!T)
					printf("二叉树不存在！\n");
				else {
					printf("请输入两个结点的关键字，用空格隔开：");
					scanf("%d %d", &e1, &e2);
					resultnode = LowestCommonAncestor(T, e1, e2);
					if (resultnode == NULL)
						printf("这两个结点没有公共祖先！\n");
					else
						printf("这两个结点最近公共祖先的结点值为%s", resultnode->data.others);
				}
				getchar();
				getchar();
				break;
			case 16:
				if (!T)
					printf("二叉树不存在！\n");
				else {
					InvertTree(T);
					printf("二叉树翻转完成！\n");
				}
				getchar();
				getchar();
				break;
			case 17:
				printf("请选择功能：1.二叉树添加；2.二叉树移除；3.二叉树查找\n");
				scanf("%d", &choice);
				if (choice == 1) {
					printf("请输入二叉树名称：");
					scanf("%s", TTT.elem[TTT.length].name);
					TTT.elem[TTT.length].T = NULL;
					printf("请输入二叉树%s带空枝的先根遍历序列，以-1 null结束：");
					i = 0;
					TElemType newdefinition[100];
					do {
						scanf("%d%s", &newdefinition[i].key, newdefinition[i].others);
					} while (newdefinition[i++].key != -1);
					count = 0;
					if (CreateBiTree(TTT.elem[TTT.length].T, newdefinition) == OK) {
						TTT.length++;
						printf("名为%s的二叉树添加成功！\n", TTT.elem[TTT.length - 1].name);
					}
				} else if (choice == 2) {
					if (TTT.length <= 0)
						printf("删除失败，二叉树集合为空！\n");
					else {
						printf("请输入二叉树名称：");
						scanf("%s", TreeName);
						int flag = 0;
						for (j = 0; j < TTT.length; j++)
							if (strcmp(TTT.elem[j].name, TreeName) == 0) {
								flag = 1;
								break;
							}
						if (flag) {
							for (int k = j; k < TTT.length - 1; k++) {
								TTT.elem[k].T = TTT.elem[k + 1].T;
								strcpy(TTT.elem[k].name, TTT.elem[k + 1].name);
							}
							TTT.length--;
							printf("名为%s的二叉树已从二叉树集合中删除！", TreeName);
						} else
							printf("二叉树集合中没有名为%s的二叉树！\n", TreeName);
					}
				} else if (choice == 3) {
					if (TTT.length <= 0)
						printf("二叉树集合为空！\n");
					else {
						printf("请输入二叉树名称：");
						scanf("%s", TreeName);
						int flag = 0;
						for (i = 0; i < TTT.length; i++)
							if (strcmp(TTT.elem[i].name, TreeName) == 0) {
								flag = 1;
								break;
							}
						if (flag) {
							T = TTT.elem[i].T;
							printf("查找成功！\n");
						} else
							printf("二叉树集合中没有名为%s的二叉树！\n", TreeName);
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