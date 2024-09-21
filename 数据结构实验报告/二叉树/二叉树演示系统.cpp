#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

// ���ೣ����������
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
} TElemType; //������������Ͷ���

typedef struct BiTNode { //���������㶨��
	TElemType  data;
	struct BiTNode *lchild, *rchild;
} BiTNode, *BiTree;

typedef struct {
	struct {
		char name[30];
		BiTree T;
	} elem[100];
	int length;
} TREES; //���������Ͻṹ����

int count = 0;// ͳ���Ѿ������˼������
int dep[1000] = {0};

BiTree CreateBiTNode(TElemType definition[]) {
// ����һ���µĶ���������
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
/*���ݴ���֦�Ķ������ȸ���������definition����һ�ö������������ڵ�ָ�븳ֵ��T������OK��
�������ͬ�Ĺؼ��֣�����ERROR����������ͨ������������������ʵ�ֱ�������*/
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
//�����������óɿգ���ɾ�����н�㣬�ͷŽ��ռ�
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
//�������T�����
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
//���ҽ��
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
// ����µĸ�ֵ�Ƿ�������ͻ
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
//ʵ�ֽ�㸳ֵ����������ͨ������������������ʵ�ֱ�������
{
	BiTree ans;
	ans = LocateNode(T, e);
	if (ans) {
		if (check(T, value.key, e)) {
			ans->data = value;
			return OK;
		} else
			return ERROR;// ���ƺ�ؼ��ֲ�Ψһ
	} else
		return ERROR;
}

BiTNode *GetSibling(BiTree T, KeyType e)
//ʵ�ֻ���ֵܽ��
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
// ���ؼ����Ƿ��ͻ��������0��˵����ͻ
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
//�����㡣��������ͨ������������������ʵ�ֱ�������
{
	BiTree target, newnode;
	if (LR == -1) { // c��Ϊ�������룬ԭ�������Ϊc��������
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
			if (LR == 0) { // ������c��T�У���Ϊ�ؼ���Ϊe�Ľ�������
				if (!T)
					return INFEASIBLE;
				BiTree newnode = (BiTree)malloc(sizeof(BiTNode));
				newnode->data = c;
				newnode->rchild = target->lchild;
				target->lchild = newnode;
				newnode->lchild = NULL;
				return OK;
			} else { // ������c��T�У���Ϊ�ؼ���Ϊe�Ľ����Һ���
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
//ɾ����㡣��������ͨ������������������ʵ�ֱ�������
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
			} else // ɾ���ĵ��Ǹ��ڵ�
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
//�������������T
{
	if (T) {
		visit(T);
		PreOrderTraverse(T->lchild, visit);
		PreOrderTraverse(T->rchild, visit);
	}
	return OK;
}

int top = 0; // ջ��ָ��
BiTree stack[1000];

status InOrderTraverse(BiTree T, void (*visit)(BiTree))
//�������������T
{
	BiTree p = T;
	while (p || top) {
		if (p) { // ��ָ���ջ������������
			stack[top++] = p;
			p = p->lchild;
		} else { // ��ָ����ջ�����ʸ��ڵ㲢����������
			p = stack[--top];
			visit(p);
			p = p->rchild;
		}
	}
	return OK;
}

status PostOrderTraverse(BiTree T, void (*visit)(BiTree))
//�������������T
{
	if (T) {
		PostOrderTraverse(T->lchild, visit);
		PostOrderTraverse(T->rchild, visit);
		visit(T);
	}
	return OK;
}

status LevelOrderTraverse(BiTree T, void (*visit)(BiTree))
//�������������T
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
// ��������洢
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
// ���������ȡ
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
//���������Ľ������д�뵽�ļ�FileName��
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

//�����ļ�FileName�Ľ�����ݣ�����������
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
// ���ظ��ڵ㵽Ҷ�ӽ������·����
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
// �����������������������
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
// ��ת������
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
		printf("             ������������ʾϵͳ \n            �����2110��  �����\n");
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
		printf("    ��ѡ�����[0~17]:\n");
		scanf("%d", &op);
		switch (op) {
			// ���·ֱ��Ӧ17�����ܣ������������ִ�ж�Ӧ�Ĳ���
			case 1:
				printf("���������֦�Ķ������ȸ��������У���-1 null������\n");
				i = 0;
				do {
					scanf("%d%s", &definition[i].key, definition[i].others);
				} while (definition[i++].key != -1);
				if (CreateBiTree(T, definition) == OK)
					printf("�����������ɹ���\n");
				else if (CreateBiTree(T, definition) == ERROR)
					printf("������ͬ�ؼ��֣�����������ʧ�ܣ�\n");
				else
					printf("�Ѵ��ڶ�����������ʧ�ܣ�\n");
				getchar();
				getchar();
				break;
			case 2:
				result = ClearBiTree(T);
				if (result == INFEASIBLE)
					printf("�����������ڣ�\n");
				if (result == OK)
					printf("��������ճɹ���\n");
				getchar();
				getchar();
				break;
			case 3:
				result = BiTreeDepth(T);
				if (!result)
					printf("�����������ڣ����Ϊ0����\n");
				else
					printf("�����������Ϊ%d��\n", result);
				getchar();
				getchar();
				break;
			case 4:
				if (!T)
					printf("�����������ڣ�\n");
				else {
					printf("������Ҫ���ҽ��Ĺؼ��֣�");
					scanf("%d", &e);
					resultnode = LocateNode(T, e);
					if (!resultnode)
						printf("�������в����ڹؼ���Ϊ%d�Ľ�㣡\n", e);
					else
						printf("���ҵ��ؼ���Ϊ%d�Ľ�㣬���ֵΪ%s��\n", e, resultnode->data.others);
				}
				getchar();
				getchar();
				break;
			case 5:
				if (!T)
					printf("�����������ڣ�\n");
				else {
					printf("������Ҫ��ֵ�Ľ��ؼ��֣��Լ��½����Ϣ���ؼ��֡����ֵ�����ÿո������");
					scanf("%d %d %s", &e, &value.key, value.others);
					result = Assign(T, e, value);
					if (result == OK)
						printf("��ֵ�ɹ���\n");
					else
						printf("��ֵʧ�ܣ�\n");
				}
				getchar();
				getchar();
				break;
			case 6:
				if (!T)
					printf("�����������ڣ�\n");
				else {
					printf("������Ҫ�����ֵܽ��Ľ��Ĺؼ��֣�");
					scanf("%d", &e);
					resultnode = GetSibling(T, e);
					if (!result)
						printf("�ؼ���Ϊ%d�Ľ�㲻�����ֵܽ�㣡\n", e);
					else
						printf("�ؼ���Ϊ%d�Ľ������ֵܽ�㣬���ֵΪ%s��\n", e, resultnode->data.others);
				}
				getchar();
				getchar();
				break;
			case 7:
				if (!T)
					printf("�����������ڣ�\n");
				else {
					printf("������Ŀ����Ĺؼ��֡�LR���½����Ϣ���ؼ��֡����ֵ�����ÿո����\n");
					printf("��LRΪ-1/0/1�ֱ������Ϊ��������ΪĿ���������Һ��Ӳ��룩��");
					scanf("%d %d %d %s", &e, &LR, &c.key, c.others);
					result = InsertNode(T, e, LR, c);
					if (result == OK)
						printf("����ɹ���\n");
					else
						printf("����ʧ�ܣ�\n", result);
				}
				getchar();
				getchar();
				break;
			case 8:
				if (!T)
					printf("�����������ڣ�\n");
				else {
					printf("������Ҫɾ�����Ĺؼ��֣�");
					scanf("%d", &e);
					result = DeleteNode(T, e);
					if (result == OK)
						printf("ɾ���ɹ���\n");
					if (result == ERROR)
						printf("ɾ��ʧ�ܣ�\n", result);
				}
				getchar();
				getchar();
				break;
			case 9:
				if (!T)
					printf("�����������ڣ�\n");
				else {
					printf("\n-----------------����������--------------------------\n");
					result = PreOrderTraverse(T, visit);
					if (result == OK)
						printf("\n\n----------------------end------------------------------\n");
					else
						printf("����ʧ�ܣ�\n", result);
				}
				getchar();
				getchar();
				break;
			case 10:
				if (!T)
					printf("�����������ڣ�\n");
				else {
					printf("\n-----------------����������--------------------------\n");
					result = InOrderTraverse(T, visit);
					if (result == OK)
						printf("\n\n----------------------end------------------------------\n");
					else
						printf("����ʧ�ܣ�\n", result);
				}
				getchar();
				getchar();
				break;
			case 11:
				if (!T)
					printf("�����������ڣ�\n");
				else {
					printf("\n-----------------����������--------------------------\n");
					result = PostOrderTraverse(T, visit);
					if (result == OK)
						printf("\n\n----------------------end------------------------------\n");
					else
						printf("����ʧ�ܣ�\n", result);
				}
				getchar();
				getchar();
				break;
			case 12:
				if (!T)
					printf("�����������ڣ�\n");
				else {
					printf("\n-----------------����������--------------------------\n");
					result = LevelOrderTraverse(T, visit);
					if (result == OK)
						printf("\n\n----------------------end------------------------------\n");
					else
						printf("����ʧ�ܣ�\n", result);
				}
				getchar();
				getchar();
				break;
			case 13:
				printf("������Ŀ���ļ�����");
				scanf("%s", FileName);
				if (T) {
					result = SaveBiTree(T, FileName);
					if (result == OK)
						printf("�����������ѳɹ�д���ļ�%s��\n", FileName);
					else
						printf("д�ļ�ʧ�ܣ�\n");
				} else {
					result = LoadBiTree(T, FileName);
					if (result == OK)
						printf("�ļ�%s�е������ѳɹ����뵽�������У�\n", FileName);
					else
						printf("�ļ�д��ʧ�ܣ�\n");
				}
				getchar();
				getchar();
				break;
			case 14:
				if (!T)
					printf("�����������ڣ�\n");
				else {
					printf("���ڵ㵽Ҷ�ӽ������·����Ϊ%d��", MaxPathSum(T, 0));
				}
				getchar();
				getchar();
				break;
			case 15:
				if (!T)
					printf("�����������ڣ�\n");
				else {
					printf("�������������Ĺؼ��֣��ÿո������");
					scanf("%d %d", &e1, &e2);
					resultnode = LowestCommonAncestor(T, e1, e2);
					if (resultnode == NULL)
						printf("���������û�й������ȣ�\n");
					else
						printf("�������������������ȵĽ��ֵΪ%s", resultnode->data.others);
				}
				getchar();
				getchar();
				break;
			case 16:
				if (!T)
					printf("�����������ڣ�\n");
				else {
					InvertTree(T);
					printf("��������ת��ɣ�\n");
				}
				getchar();
				getchar();
				break;
			case 17:
				printf("��ѡ���ܣ�1.��������ӣ�2.�������Ƴ���3.����������\n");
				scanf("%d", &choice);
				if (choice == 1) {
					printf("��������������ƣ�");
					scanf("%s", TTT.elem[TTT.length].name);
					TTT.elem[TTT.length].T = NULL;
					printf("�����������%s����֦���ȸ��������У���-1 null������");
					i = 0;
					TElemType newdefinition[100];
					do {
						scanf("%d%s", &newdefinition[i].key, newdefinition[i].others);
					} while (newdefinition[i++].key != -1);
					count = 0;
					if (CreateBiTree(TTT.elem[TTT.length].T, newdefinition) == OK) {
						TTT.length++;
						printf("��Ϊ%s�Ķ�������ӳɹ���\n", TTT.elem[TTT.length - 1].name);
					}
				} else if (choice == 2) {
					if (TTT.length <= 0)
						printf("ɾ��ʧ�ܣ�����������Ϊ�գ�\n");
					else {
						printf("��������������ƣ�");
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
							printf("��Ϊ%s�Ķ������ѴӶ�����������ɾ����", TreeName);
						} else
							printf("������������û����Ϊ%s�Ķ�������\n", TreeName);
					}
				} else if (choice == 3) {
					if (TTT.length <= 0)
						printf("����������Ϊ�գ�\n");
					else {
						printf("��������������ƣ�");
						scanf("%s", TreeName);
						int flag = 0;
						for (i = 0; i < TTT.length; i++)
							if (strcmp(TTT.elem[i].name, TreeName) == 0) {
								flag = 1;
								break;
							}
						if (flag) {
							T = TTT.elem[i].T;
							printf("���ҳɹ���\n");
						} else
							printf("������������û����Ϊ%s�Ķ�������\n", TreeName);
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