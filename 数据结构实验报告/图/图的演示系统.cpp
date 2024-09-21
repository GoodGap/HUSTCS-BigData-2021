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
#define MAX_VERTEX_NUM 20 //��󶥵���
#define INF 999999

typedef int status;
typedef int KeyType;
typedef enum {DG, DN, UDG, UDN} GraphKind;

typedef struct {
	KeyType  key;
	char others[20];
} VertexType; //�������Ͷ���

typedef struct ArcNode {  //�������Ͷ���
	int adjvex;              //����λ�ñ��
	struct ArcNode  *nextarc;	   //��һ������ָ��
} ArcNode;

typedef struct VNode {		//ͷ��㼰���������Ͷ���
	VertexType data;       	//������Ϣ
	ArcNode *firstarc;      //ָ���һ����
} VNode, AdjList[MAX_VERTEX_NUM];

typedef  struct {  //�ڽӱ�����Ͷ���
	AdjList vertices;     	 //ͷ�������
	int vexnum, arcnum;   	 //������������
	GraphKind  kind;         //ͼ������
} ALGraph;

typedef struct {
	struct {
		char name[30];
		ALGraph G;
	} elem[100];
	int length;
} GRAPHS; //����ͼ���Ͻṹ����

status CreateCraph(ALGraph &G, VertexType V[], KeyType VR[][2])
/*����V��VR����ͼT������OK�����V��VR����ȷ������ERROR
�������ͬ�Ĺؼ��֣�����ERROR����������ͨ������������������ʵ�ֱ�������*/
{
	int check[100] = {0}, order[100];
	G.vexnum = 0, G.arcnum = 0;
	for (int i = 0; V[i].key != -1; i++) {
		if (check[V[i].key] == 1)
			return ERROR;// �ж϶���ؼ����Ƿ��ظ�
		check[V[i].key] = 1;
		order[V[i].key] = i;
	}
	for (int i = 0; V[i].key != -1; i++) {
		G.vertices[i].data.key = V[i].key;
		strcpy(G.vertices[i].data.others, V[i].others);
		G.vertices[i].firstarc = NULL;
		G.vexnum++;
	}
	if (G.vexnum > 20 || G.vexnum < 1)
		return ERROR;
	int count = 0, flag;
	for (int i = 0; VR[i][0] != -1; i++) {
		if (check[VR[i][0]] != 1 || check[VR[i][1]] != 1)
			return ERROR;// �жϱߵ���Ϣ�Ƿ�Ϸ�
		count++;// count�����ܹ��м����ߵ���Ϣ
	}
	for (int i = count; i >= 0; i--) {
		if (VR[i][0] != VR[i][1]) {
			flag = 0;
			for (int j = 0; j < i; j++)
				if ((VR[j][0] == VR[i][0] && VR[j][1] == VR[i][1]) || (VR[j][1] == VR[i][0] && VR[j][0] == VR[i][1])) {
					flag = 1;
					break;
				}
			if (flag != 1) {
				ArcNode *newnode = (ArcNode *)malloc(sizeof(ArcNode));
				newnode->adjvex = order[VR[i][1]];// ������һ�����
				newnode->nextarc = NULL;
				if (check[VR[i][0]] == 1) {
					G.vertices[order[VR[i][0]]].firstarc = newnode;
					check[VR[i][0]] = 0;
				} else {
					ArcNode *temp;
					temp = G.vertices[order[VR[i][0]]].firstarc;
					while (temp->nextarc)
						temp = temp->nextarc;
					temp->nextarc = newnode;
				}
				newnode = (ArcNode *)malloc(sizeof(ArcNode));
				newnode->adjvex = order[VR[i][0]];// �����ڶ������
				newnode->nextarc = NULL;
				if (check[VR[i][1]] == 1) {
					G.vertices[order[VR[i][1]]].firstarc = newnode;
					check[VR[i][1]] = 0;
				} else {
					ArcNode *temp;
					temp = G.vertices[order[VR[i][1]]].firstarc;
					while (temp->nextarc)
						temp = temp->nextarc;
					temp->nextarc = newnode;
				}
				G.arcnum++;
			}
		}
	}
	return OK;
}

status DestroyGraph(ALGraph &G)
/*��������ͼG,ɾ��G��ȫ������ͱ�*/
{
	for (int i = 0; i < G.vexnum; i++) {
		ArcNode *temp = G.vertices[i].firstarc, *pre = temp;
		while (temp) {
			temp = temp->nextarc;
			free(pre);
			pre = temp;
		}
		G.vertices[i].firstarc = NULL;
	}
	G.vexnum = 0;
	G.arcnum = 0;
	return OK;
}

int LocateVex(ALGraph G, KeyType u)
//����u��ͼG�в��Ҷ��㣬���ҳɹ�����λ�򣬷��򷵻�-1��
{
	for (int i = 0; i < G.vexnum; i++)
		if (u == G.vertices[i].data.key)
			return i;
	return -1;
}

status PutVex(ALGraph &G, KeyType u, VertexType value)
//����u��ͼG�в��Ҷ��㣬���ҳɹ����ö���ֵ�޸ĳ�value������OK��
//�������ʧ�ܻ�ؼ��ֲ�Ψһ������ERROR
{
	int pos, flag = 0;
	for (pos = 0; pos < G.vexnum; pos++)
		if (u == G.vertices[pos].data.key) {
			flag = 1;
			break;
		}
	if (!flag)
		return ERROR;
	for (int i = 0; i < G.vexnum; i++)
		if (value.key == G.vertices[i].data.key)
			return ERROR;
	G.vertices[pos].data = value;
	return OK;
}

int FirstAdjVex(ALGraph G, KeyType u)
//����u��ͼG�в��Ҷ��㣬���ҳɹ����ض���u�ĵ�һ�ڽӶ���λ�򣬷��򷵻�-1��
{
	int firstvexorder = -1;
	for (int i = 0; i < G.vexnum; i++)
		if (u == G.vertices[i].data.key) {
			firstvexorder = G.vertices[i].firstarc->adjvex;
			break;
		}
	return firstvexorder;
}

int NextAdjVex(ALGraph G, KeyType v, KeyType w)
//����v��ͼG�в��Ҷ��㣬���ҳɹ����ض���v���ڽӶ��������w����һ�ڽӶ����λ�򣬲���ʧ�ܷ���-1��
{
	int flag = 0, vo;
	for (vo = 0; vo < G.vexnum; vo++)
		if (v == G.vertices[vo].data.key) {
			flag = 1;
			break;
		}
	if (!flag)
		return -1;
	flag = 0;
	for (int i = 0; i < G.vexnum; i++)
		if (w == G.vertices[i].data.key) {
			flag = 1;
			break;
		}
	if (!flag)
		return -1;// �ֱ��ж�v��w��Ӧ�Ķ����Ƿ����
	ArcNode *temp = G.vertices[vo].firstarc;
	while (temp->nextarc) {
		if (G.vertices[temp->adjvex].data.key == w) {
			temp = temp->nextarc;
			for (int i = 0; i < G.vexnum; i++)
				if (G.vertices[temp->adjvex].data.key == G.vertices[i].data.key) {
					return i;
				}
			temp = temp->nextarc;
		}
		return -1;
	}
}

status InsertVex(ALGraph &G, VertexType v)
//��ͼG�в��붥��v���ɹ�����OK,���򷵻�ERROR
{
	if (G.vexnum >= 20)
		return ERROR;
	int i;
	for (i = 0; i < G.vexnum; i++)
		if (v.key == G.vertices[i].data.key)
			return ERROR;
	G.vertices[i].data = v;
	G.vertices[i].firstarc = NULL;
	G.vexnum++;
	return OK;
}

status DeleteVex(ALGraph &G, KeyType v)
//��ͼG��ɾ���ؼ���v��Ӧ�Ķ����Լ���صĻ����ɹ�����OK,���򷵻�ERROR
{
	int i, flag = 0;
	ArcNode *pre, *next, *temp;
	for (i = 0; i < G.vexnum; i++)
		if (v == G.vertices[i].data.key) {
			flag = 1; // i��ɾ���Ľ��λ��
			break;
		}
	if (!flag || G.vexnum <= 1)// ��֤ɾ����ͼ����
		return ERROR;
	// �������ͷŸýڵ������ռ�
	temp = G.vertices[i].firstarc, pre = temp;
	while (temp) {
		temp = temp->nextarc;
		free(pre);
		G.arcnum--;
		pre = temp;
	}
	G.vertices[i].firstarc = NULL;

	for (int j = i + 1; j < G.vexnum; j++)
		G.vertices[j - 1] = G.vertices[j];//����ȫ��ǰ��
	G.vexnum--;
	for (int j = 0; j < G.vexnum; j++) {
		if (!G.vertices[j].firstarc)
			continue;// ��������ڻ�
		temp = G.vertices[j].firstarc;
		while (temp) {
			if (temp->adjvex == i)
				temp->adjvex = -999; //��-999���Ҫɾ�Ļ����
			else if (temp->adjvex > i)
				temp->adjvex--;//���ǰ��1
			temp = temp->nextarc;
		}
		pre = G.vertices[j].firstarc;
		next = pre->nextarc; // ������һɾȥλ��Ϊ-999�Ľ��
		if (!next) { // ֻ��һ���������
			if (pre->adjvex == -999) {
				G.vertices[j].firstarc = NULL;
				free(pre);
			}
			continue;
		} else { // �ж�����
			//�����жϵ�һ����Ҫ��Ҫɾ
			if (pre->adjvex == -999) {
				G.vertices[j].firstarc = next;
				free(pre);
				pre = next;
				next = next->nextarc;
			}
			//Ȼ������������е�
			while (next) {
				if (next->adjvex == -999) {
					pre->nextarc = next->nextarc;
					free(next);
				}
				next = next->nextarc;
			}
		}
	}
	return OK;
}

status InsertArc(ALGraph &G, KeyType v, KeyType w)
//��ͼG�����ӻ�<v,w>���ɹ�����OK,���򷵻�ERROR
{
	int vo, wo; // �ֱ���v��w��λ��
	for (vo = 0; vo < G.vexnum; vo++)
		if (v == G.vertices[vo].data.key)
			break;
	if (vo == G.vexnum)
		return ERROR;
	for (wo = 0; wo < G.vexnum; wo++)
		if (w == G.vertices[wo].data.key)
			break;
	if (wo == G.vexnum)
		return ERROR;
	ArcNode *temp = G.vertices[vo].firstarc;
	while (temp) { //�ж��Ƿ��Ѵ���·��<v,w>
		if (G.vertices[temp->adjvex].data.key == w)
			return ERROR;
		temp = temp->nextarc;
	}

	ArcNode *newnode = (ArcNode *)malloc(sizeof(ArcNode));
	newnode->adjvex = wo;
	newnode->nextarc = G.vertices[vo].firstarc;
	G.vertices[vo].firstarc = newnode;
	newnode = (ArcNode *)malloc(sizeof(ArcNode));
	newnode->adjvex = vo;
	newnode->nextarc = G.vertices[wo].firstarc;
	G.vertices[wo].firstarc = newnode;
	G.arcnum++;
	return OK;
}

status DeleteArc(ALGraph &G, KeyType v, KeyType w)
//��ͼG��ɾ����<v,w>���ɹ�����OK,���򷵻�ERROR
{
	int vo, wo, flag = 0;
	for (vo = 0; vo < G.vexnum; vo++)
		if (v == G.vertices[vo].data.key)
			break;
	if (vo == G.vexnum)
		return ERROR;
	for (wo = 0; wo < G.vexnum; wo++)
		if (w == G.vertices[wo].data.key)
			break;
	if (wo == G.vexnum)
		return ERROR;

	ArcNode *temp = G.vertices[vo].firstarc, *pre, *next;
	while (temp) { //�ж�·��<v,w>�Ƿ����
		if (G.vertices[temp->adjvex].data.key == w) {
			flag = 1;
			break;
		}
		temp = temp->nextarc;
	}
	if (!flag)
		return ERROR;

	pre = G.vertices[vo].firstarc;
	next = pre->nextarc;
	if (G.vertices[pre->adjvex].data.key == w) {
		G.vertices[vo].firstarc = next;
		free(pre);
	} else {
		while (next) {
			if (G.vertices[next->adjvex].data.key == w) {
				pre->nextarc = next->nextarc;
				free(next);
				break;
			}
			pre = pre->nextarc;
			next = next->nextarc;
		}
	}

	pre = G.vertices[wo].firstarc;
	next = pre->nextarc;
	if (G.vertices[pre->adjvex].data.key == v) {
		G.vertices[wo].firstarc = next;
		free(pre);
	} else {
		while (next) {
			if (G.vertices[next->adjvex].data.key == v) {
				pre->nextarc = next->nextarc;
				free(next);
				break;
			}
			pre = pre->nextarc;
			next = next->nextarc;
		}
	}
	G.arcnum--;
	return OK;
}


void visit(VertexType v) {
	printf(" %d %s", v.key, v.others);
}
int visited[100];// ��¼ĳ���Ƿ񱻷��ʹ�

status DFSTraverse(ALGraph &G, void (*visit)(VertexType))
//��ͼG������������������������ζ�ͼ�е�ÿһ������ʹ�ú���visit����һ�Σ��ҽ�����һ��
{
	if (G.vexnum < 1)
		return ERROR;
	ArcNode *temp;
	for (int i = 0; i < G.vexnum; i++)
		visited[i] = 0;
	for (int i = 0; i < G.vexnum; i++)
		if (!visited[i]) {
			visit(G.vertices[i].data);
			visited[i] = 1;
			temp = G.vertices[i].firstarc;
			while (temp) {
				if (!visited[temp->adjvex]) {
					visit(G.vertices[temp->adjvex].data);
					visited[temp->adjvex] = 1;
					temp = G.vertices[temp->adjvex].firstarc;
				} else
					break;
			}
		}
	return OK;
}

status BFSTraverse(ALGraph &G, void (*visit)(VertexType))
//��ͼG���й�������������������ζ�ͼ�е�ÿһ������ʹ�ú���visit����һ�Σ��ҽ�����һ��
{
	if (G.vexnum < 1)
		return ERROR;
	ArcNode *temp;
	int queue[100];// ��λ��
	int front = 0, rear = 0;
	for (int i = 0; i < G.vexnum; i++)
		visited[i] = 0;
	for (int i = 0; i < G.vexnum; i++)
		if (!visited[i]) {
			queue[front] = i;
			rear++;
			temp = G.vertices[i].firstarc;
			while (temp) {
				queue[rear++] = temp->adjvex;
				temp = temp->nextarc;
			}
			while (rear - front > 0) {
				if (!visited[queue[front]]) {
					visit(G.vertices[queue[front]].data);
					visited[queue[front]] = 1;
					temp = G.vertices[queue[front++]].firstarc;
					while (temp) {
						queue[rear++] = temp->adjvex;
						temp = temp->nextarc;
					}
				} else
					front++;
			}
		}
	return OK;
}

status SaveGraph(ALGraph G, char FileName[])
//��ͼ������д�뵽�ļ�FileName��
{
	if (G.vexnum < 1)
		return INFEASIBLE;
	FILE *fp;
	fp = fopen(FileName, "w");
	ArcNode *temp;
	int count;
	fprintf(fp, "%d %d\n", G.vexnum, G.arcnum);
	for (int i = 0; i < G.vexnum; i++) { // �洢ÿ��������ڽӵ���
		temp = G.vertices[i].firstarc;
		count = 0;
		while (temp) {
			count++;
			temp = temp->nextarc;
		}
		fprintf(fp, "%d\n", count);
	}
	for (int i = 0; i < G.vexnum; i++) {
		fprintf(fp, "%d %s", G.vertices[i].data.key, G.vertices[i].data.others);
		temp = G.vertices[i].firstarc;
		while (temp) {
			fprintf(fp, " %d", temp->adjvex);
			temp = temp->nextarc;
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
	return OK;
}
status LoadGraph(ALGraph &G, char FileName[])

//�����ļ�FileName��ͼ���ݣ�����ͼ���ڽӱ�
{
	if (G.vexnum)
		return INFEASIBLE;
	int count[30];
	ArcNode *newnode, *temp;
	FILE *fp;
	fp = fopen(FileName, "r");
	fscanf(fp, "%d %d\n", &G.vexnum, &G.arcnum);
	for (int i = 0; i < G.vexnum; i++)
		fscanf(fp, "%d\n", &count[i]);
	for (int i = 0; i < G.vexnum; i++) {
		fscanf(fp, "%d %s", &G.vertices[i].data.key, G.vertices[i].data.others);
		if (count[i] == 0)
			G.vertices[i].firstarc = NULL;
		else {
			newnode = (ArcNode *)malloc(sizeof(ArcNode));
			fscanf(fp, " %d", &newnode->adjvex);
			G.vertices[i].firstarc = newnode;
			G.vertices[i].firstarc->nextarc = NULL;
			temp = G.vertices[i].firstarc;
			for (int j = 1; j < count[i]; j++) {
				newnode = (ArcNode *)malloc(sizeof(ArcNode));
				fscanf(fp, " %d", &newnode->adjvex);
				temp->nextarc = newnode;
				newnode->nextarc = NULL;
				temp = newnode;
			}
		}
		fscanf(fp, "\n");
	}
	fclose(fp);
	return OK;
}

int Near(ALGraph &G, int vo, int wo)
// �ж����������Ƿ�ֱ�����������ڷ���1�����򷵻�999999���������ޣ�
{
	ArcNode *temp = G.vertices[vo].firstarc;
	while (temp) {
		if (temp->adjvex == wo)
			return 1;
		temp = temp->nextarc;
	}
	return INF;
}

int ShortestPathLength(ALGraph &G, KeyType v, KeyType w)
// ���ض���v�붥��w�����·���ĳ���
{
	int i, j, flag, t, min, temp;
	int pre[50], d[50];
	int vo = LocateVex(G, v);
	int wo = LocateVex(G, w);
	if (vo == -1 || wo == -1)
		return ERROR;
	for (i = 0; i < G.vexnum; i++) {
		visited[i] = 0;
		pre[i] = 0;
		d[i] = Near(G, vo, i);
	}
	visited[vo] = 1;
	d[vo] = 0;
	//����ÿ��ѭ�������һ�����·��
	for (i = 1; i < G.vexnum; i++) {
		t = 0;
		flag = -1;
		min = INF;
		for (j = 0; j < G.vexnum; j++)
			if (!visited[j] && d[j] < min) { //Ѱ��δ��ӵ����·��
				min = d[j];
				flag = j;
				visited[flag] = 1;
			}
		if (flag == -1) //����ͨ
			break;
		for (j = 0; j < G.vexnum; j++) {
			temp = Near(G, flag, j);
			if (temp != INF)
				temp += min;
			if (!visited[j] && temp < d[j]) { //�޸ĵ�j�����·��
				d[j] = temp;
				pre[j] = flag;
				t++;
			}
		}
	}
	if (d[wo] == INF)
		return ERROR;
	else
		return d[wo];
}

void VerticesSetLessThanK(ALGraph &G, KeyType v, int k)
// �����붥��v����С��k�Ķ��㼯��
{
	int count = 0;
	for (int i = 0; i < G.vexnum; i++)
		if (G.vertices[i].data.key != v && ShortestPathLength(G, v, G.vertices[i].data.key) < k) {
			visit(G.vertices[i].data);
			count++;
		}
	if (!count)
		printf("��������ö������С��%d�Ķ��㣡\n", k);
}

int ConnectedComponentsNums(ALGraph &G)
// ��������ͼ��ͨ�����ĸ���
{
	if (G.vexnum < 1)
		return ERROR;
	int	ConnectedComponents = 0;
	ArcNode *temp;
	int queue[100];
	int front = 0, rear = 0;
	for (int i = 0; i < G.vexnum; i++)
		visited[i] = 0;
	for (int i = 0; i < G.vexnum; i++)
		if (!visited[i]) {
			queue[front] = i;
			rear++;
			temp = G.vertices[i].firstarc;
			while (temp) {
				queue[rear++] = temp->adjvex;
				temp = temp->nextarc;
			}
			while (rear - front > 0) {
				if (!visited[queue[front]]) {
					visited[queue[front]] = 1;
					temp = G.vertices[queue[front++]].firstarc;
					while (temp) {
						queue[rear++] = temp->adjvex;
						temp = temp->nextarc;
					}
				} else
					front++;
			}
			if (front == rear)
				ConnectedComponents++;
		}
	return ConnectedComponents;
}

int main() {
	int op = 1;
	int i, j, k, result, choice;
	char FileName[30], GraphName[30];
	ALGraph G;
	G.arcnum = 0;
	G.vexnum = 0;
	G.kind = UDG;
	VertexType V[30];
	VertexType value;
	KeyType VR[100][2];
	KeyType u, v, w;
	GRAPHS GGG;
	GGG.length = 0;
	while (op) {
		system("cls");
		printf("\n\n");
		printf("----------------------------------------------------------\n");
		printf("                 ����ͼ������ʾϵͳ \n                �����2110��  �����\n");
		printf("----------------------------------------------------------\n");
		printf("    1. CreateGraph           2. DestroyGraph\n");
		printf("    3. LocateVex             4. PutVex\n");
		printf("    5. FirstAdjVex           6. NextAdjVex\n");
		printf("    7. InsertVex             8. DeleteVex\n");
		printf("    9. InsertArc             10. DeleteArc\n");
		printf("    11. DFSTraverse          12. BFSTraverse\n");
		printf("    13. Save/Load            14. VerticesSetLessThanK\n");
		printf("    15. ShortestPathLength   16. ConnectedComponentsNums\n");
		printf("    17. MultiGraph           0. Exit\n");
		printf("----------------------------------------------------------\n");
		printf("    ��ѡ�����[0~17]:\n");
		scanf("%d", &op);
		switch (op) {
			// ���·ֱ��Ӧ17�����ܣ������������ִ�ж�Ӧ�Ĳ���
			case 1:
				if (G.vexnum)
					printf("����ͼ����ʧ�ܣ�\n");
				else {
					printf("�����붥�����к͹�ϵ�����У��ֱ���-1 nil��-1 -1������\n");
					i = 0;
					do {
						scanf("%d%s", &V[i].key, V[i].others);
					} while (V[i++].key != -1);
					i = 0;
					do {
						scanf("%d%d", &VR[i][0], &VR[i][1]);
					} while (VR[i++][0] != -1);
					result = CreateCraph(G, V, VR);
					if (result == OK)
						printf("����ͼ�����ɹ���\n");
					else if (result == ERROR)
						printf("����������������ͼ����ʧ�ܣ�\n");
				}
				getchar();
				getchar();
				break;
			case 2:
				if (!G.vexnum)
					printf("����ͼΪ�գ�\n");
				else if (DestroyGraph(G) == OK)
					printf("����ͼ��ճɹ���\n");
				getchar();
				getchar();
				break;
			case 3:
				if (!G.vexnum)
					printf("����ͼΪ�գ�\n");
				else {
					printf("������Ҫ���Ҷ���Ĺؼ��֣�");
					scanf("%d", &u);
					result = LocateVex(G, u);
					if (result == -1)
						printf("����ʧ�ܣ�\n");
					else
						printf("�ؼ���Ϊ%d�Ľ����ͼ�е�λ��Ϊ%d��\n", u, result);
				}
				getchar();
				getchar();
				break;
			case 4:
				if (!G.vexnum)
					printf("����ͼΪ�գ�\n");
				else {
					printf("������Ҫ��ֵ����Ĺؼ��֡��µĹؼ��ֺ����ƣ��ÿո������");
					scanf("%d %d %s", &u, &value.key, value.others);
					result = PutVex(G, u, value);
					if (result == OK)
						printf("��ֵ�ɹ���\n");
					else if (result == ERROR)
						printf("����ʧ�ܻ�ؼ��ֲ�Ψһ��\n");
				}
				getchar();
				getchar();
				break;
			case 5:
				if (!G.vexnum)
					printf("����ͼΪ�գ�\n");
				else {
					printf("������Ҫ���ҵ�һ�ڽӵ�Ķ���ؼ��֣�");
					scanf("%d", &u);
					result = FirstAdjVex(G, u);
					if (result == OK)
						printf("�ؼ���Ϊ%d�Ķ���ĵ�һ�ڽӵ�λ��Ϊ%d��\n", u, result);
					else if (result == -1)
						printf("����ʧ�ܻ�ö��㲻�����ڽӵ㣡\n");
				}
				getchar();
				getchar();
				break;
			case 6:
				if (!G.vexnum)
					printf("����ͼΪ�գ�\n");
				else {
					printf("������Ҫ���ҵĶ���ؼ��ֺ���ĳһ�ڽӵ�Ĺؼ��֣��ÿո������");
					scanf("%d %d", &v, &w);
					result = NextAdjVex(G, v, w);
					if (result == OK)
						printf("�ؼ���Ϊ%d�Ķ�����ڽӵ�%d����һ�ڽӵ��λ��Ϊ%d��\n", v, w);
					else if (result == -1)
						printf("����ʧ�ܣ�\n");
				}
				getchar();
				getchar();
				break;
			case 7:
				if (!G.vexnum)
					printf("����ͼΪ�գ�\n");
				else {
					printf("�������¶���Ĺؼ��ֺ����ƣ��ÿո������");
					scanf("%d %s", &value.key, value.others);
					result = InsertVex(G, value);
					if (result == OK)
						printf("���� %d %s ����ɹ���\n", value.key, value.others);
					else if (result == ERROR)
						printf("����ʧ�ܣ�\n");
				}
				getchar();
				getchar();
				break;
			case 8:
				if (!G.vexnum)
					printf("����ͼΪ�գ�\n");
				else {
					printf("������Ҫɾ������Ĺؼ��֣�");
					scanf("%d", &v);
					result = DeleteVex(G, v);
					if (result == OK)
						printf("�ؼ���%d��Ӧ�Ķ�������صĻ�ɾ���ɹ���\n", v);
					else if (result == ERROR)
						printf("ɾ��ʧ�ܣ�\n");
				}
				getchar();
				getchar();
				break;
			case 9:
				if (!G.vexnum)
					printf("����ͼΪ�գ�\n");
				else {
					printf("������Ҫ���ӵĻ����˶���Ĺؼ��֣��ÿո������");
					scanf("%d %d", &v, &w);
					result = InsertArc(G, v, w);
					if (result == OK)
						printf("�ѳɹ����Ӷ���%d��%d֮��Ļ���\n", v, w);
					else if (result == ERROR)
						printf("������ʧ�ܣ�\n");
				}
				getchar();
				getchar();
				break;
			case 10:
				if (!G.vexnum)
					printf("����ͼΪ�գ�\n");
				else {
					printf("������Ҫɾ���Ļ����˶���Ĺؼ��֣��ÿո������");
					scanf("%d %d", &v, &w);
					result = DeleteArc(G, v, w);
					if (result == OK)
						printf("�ѳɹ�ɾ������%d��%d֮��Ļ���\n", v, w);
					else if (result == ERROR)
						printf("��ɾ��ʧ�ܣ�\n");
				}
				getchar();
				getchar();
				break;
			case 11:
				if (!G.vexnum)
					printf("����ͼΪ�գ�\n");
				else {
					printf("\n-------------------������������������-------------------\n\n");
					DFSTraverse(G, visit);
					printf("\n\n------------------------end-------------------------------\n");
				}
				getchar();
				getchar();
				break;
			case 12:
				if (!G.vexnum)
					printf("����ͼΪ�գ�\n");
				else {
					printf("\n-------------------������������������-------------------\n\n");
					BFSTraverse(G, visit);
					printf("\n\n------------------------end-------------------------------\n");
				}
				getchar();
				getchar();
				break;
			case 13:
				printf("������Ŀ���ļ�����");
				scanf("%s", FileName);
				if (G.vexnum) {
					result = SaveGraph(G, FileName);
					if (result == OK)
						printf("����ͼ�����ѳɹ�д���ļ�%s��\n", FileName);
					else
						printf("д�ļ�ʧ�ܣ�\n");
				} else {
					result = LoadGraph(G, FileName);
					if (result == OK)
						printf("�ļ�%s�е������ѳɹ����뵽����ͼ�У�\n", FileName);
					else
						printf("�ļ�д��ʧ�ܣ�\n");
				}
				getchar();
				getchar();
				break;
			case 14:
				if (!G.vexnum)
					printf("����ͼΪ�գ�\n");
				else {
					printf("�����붥��Ĺؼ��ֺ�kֵ������С��k�����ÿո������");
					scanf("%d %d", &v, &k);
					printf("\n---------------�붥��%d����С��%d�Ķ��㼯��-----------------\n\n", v, k);
					VerticesSetLessThanK(G, v, k);
					printf("\n\n------------------------end-------------------------------\n");
				}
				getchar();
				getchar();
				break;
			case 15:
				if (!G.vexnum)
					printf("����ͼΪ�գ�\n");
				else {
					printf("��������������Ĺؼ��֣��ÿո������");
					scanf("%d %d", &v, &w);
					result = ShortestPathLength(G, v, w);
					if (result == ERROR)
						printf("�������ݴ������������֮�䲻����ͨ·��\n");
					else
						printf("����%d��%d֮�����·���ĳ���Ϊ%d��\n", v, w, result);
				}
				getchar();
				getchar();
				break;
			case 16:
				if (!G.vexnum)
					printf("����ͼΪ�գ�\n");
				else {
					result = ConnectedComponentsNums(G);
					if (result == ERROR)
						printf("����ͼ���޶��㣡\n");
					else
						printf("����ͼ����ͨ�����ĸ���Ϊ%d��\n", result);
				}
				getchar();
				getchar();
				break;
			case 17:
				printf("��ѡ���ܣ�1.����ͼ��ӣ�2.����ͼ�Ƴ���3.����ͼ����\n");
				scanf("%d", &choice);
				if (choice == 1) {
					printf("������������ͼ���ƣ�");
					scanf("%s", GGG.elem[GGG.length].name);
					GGG.elem[GGG.length].G.arcnum = 0;
					GGG.elem[GGG.length].G.vexnum = 0;
					printf("�����붥�����к͹�ϵ�����У��ֱ���-1 nil��-1 -1������\n");
					i = 0;
					do {
						scanf("%d%s", &V[i].key, V[i].others);
					} while (V[i++].key != -1);
					i = 0;
					do {
						scanf("%d%d", &VR[i][0], &VR[i][1]);
					} while (VR[i++][0] != -1);
					result = CreateCraph(GGG.elem[GGG.length].G, V, VR);
					if (result == OK) {
						GGG.length++;
						printf("����ͼ%s��ӳɹ���\n", GGG.elem[GGG.length - 1].name);
					} else if (result == ERROR)
						printf("����������������ͼ����ʧ�ܣ�\n");
				} else if (choice == 2) {
					if (GGG.length <= 0)
						printf("ɾ��ʧ�ܣ�����ͼ����Ϊ�գ�\n");
					else {
						printf("����������ͼ���ƣ�");
						scanf("%s", GraphName);
						int flag = 0;
						for (j = 0; j < GGG.length; j++)
							if (strcmp(GGG.elem[j].name, GraphName) == 0) {
								flag = 1;
								break;
							}
						if (flag) {
							for (int k = j; k < GGG.length - 1; k++) {
								GGG.elem[k].G = GGG.elem[k + 1].G;
								strcpy(GGG.elem[k].name, GGG.elem[k + 1].name);
							}
							GGG.length--;
							printf("����ͼ%s�Ѵ�����ͼ������ɾ����", GraphName);
						} else
							printf("����ͼ������û����Ϊ%s������ͼ��\n", GraphName);
					}
				} else if (choice == 3) {
					if (GGG.length <= 0)
						printf("����ͼ����Ϊ�գ�\n");
					else {
						printf("����������ͼ���ƣ�");
						scanf("%s", GraphName);
						int flag = 0;
						for (i = 0; i < GGG.length; i++)
							if (strcmp(GGG.elem[i].name, GraphName) == 0) {
								flag = 1;
								break;
							}
						if (flag) {
							G = GGG.elem[i].G;
							printf("���ҳɹ���\n");
						} else
							printf("����ͼ������û����Ϊ%s������ͼ��\n", GraphName);
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