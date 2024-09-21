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
#define MAX_VERTEX_NUM 20 //最大顶点数
#define INF 999999

typedef int status;
typedef int KeyType;
typedef enum {DG, DN, UDG, UDN} GraphKind;

typedef struct {
	KeyType  key;
	char others[20];
} VertexType; //顶点类型定义

typedef struct ArcNode {  //表结点类型定义
	int adjvex;              //顶点位置编号
	struct ArcNode  *nextarc;	   //下一个表结点指针
} ArcNode;

typedef struct VNode {		//头结点及其数组类型定义
	VertexType data;       	//顶点信息
	ArcNode *firstarc;      //指向第一条弧
} VNode, AdjList[MAX_VERTEX_NUM];

typedef  struct {  //邻接表的类型定义
	AdjList vertices;     	 //头结点数组
	int vexnum, arcnum;   	 //顶点数、弧数
	GraphKind  kind;         //图的类型
} ALGraph;

typedef struct {
	struct {
		char name[30];
		ALGraph G;
	} elem[100];
	int length;
} GRAPHS; //无向图集合结构定义

status CreateCraph(ALGraph &G, VertexType V[], KeyType VR[][2])
/*根据V和VR构造图T并返回OK，如果V和VR不正确，返回ERROR
如果有相同的关键字，返回ERROR。此题允许通过增加其它函数辅助实现本关任务*/
{
	int check[100] = {0}, order[100];
	G.vexnum = 0, G.arcnum = 0;
	for (int i = 0; V[i].key != -1; i++) {
		if (check[V[i].key] == 1)
			return ERROR;// 判断顶点关键字是否重复
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
			return ERROR;// 判断边的信息是否合法
		count++;// count代表总共有几条边的信息
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
				newnode->adjvex = order[VR[i][1]];// 创建第一个结点
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
				newnode->adjvex = order[VR[i][0]];// 创建第二个结点
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
/*销毁无向图G,删除G的全部顶点和边*/
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
//根据u在图G中查找顶点，查找成功返回位序，否则返回-1；
{
	for (int i = 0; i < G.vexnum; i++)
		if (u == G.vertices[i].data.key)
			return i;
	return -1;
}

status PutVex(ALGraph &G, KeyType u, VertexType value)
//根据u在图G中查找顶点，查找成功将该顶点值修改成value，返回OK；
//如果查找失败或关键字不唯一，返回ERROR
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
//根据u在图G中查找顶点，查找成功返回顶点u的第一邻接顶点位序，否则返回-1；
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
//根据v在图G中查找顶点，查找成功返回顶点v的邻接顶点相对于w的下一邻接顶点的位序，查找失败返回-1；
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
		return -1;// 分别判断v、w对应的顶点是否存在
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
//在图G中插入顶点v，成功返回OK,否则返回ERROR
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
//在图G中删除关键字v对应的顶点以及相关的弧，成功返回OK,否则返回ERROR
{
	int i, flag = 0;
	ArcNode *pre, *next, *temp;
	for (i = 0; i < G.vexnum; i++)
		if (v == G.vertices[i].data.key) {
			flag = 1; // i是删除的结点位序
			break;
		}
	if (!flag || G.vexnum <= 1)// 保证删除后图不空
		return ERROR;
	// 接下来释放该节点的链表空间
	temp = G.vertices[i].firstarc, pre = temp;
	while (temp) {
		temp = temp->nextarc;
		free(pre);
		G.arcnum--;
		pre = temp;
	}
	G.vertices[i].firstarc = NULL;

	for (int j = i + 1; j < G.vexnum; j++)
		G.vertices[j - 1] = G.vertices[j];//数组全部前移
	G.vexnum--;
	for (int j = 0; j < G.vexnum; j++) {
		if (!G.vertices[j].firstarc)
			continue;// 如果不存在弧
		temp = G.vertices[j].firstarc;
		while (temp) {
			if (temp->adjvex == i)
				temp->adjvex = -999; //用-999标记要删的弧结点
			else if (temp->adjvex > i)
				temp->adjvex--;//序号前移1
			temp = temp->nextarc;
		}
		pre = G.vertices[j].firstarc;
		next = pre->nextarc; // 下面逐一删去位次为-999的结点
		if (!next) { // 只有一条弧的情况
			if (pre->adjvex == -999) {
				G.vertices[j].firstarc = NULL;
				free(pre);
			}
			continue;
		} else { // 有多条弧
			//首先判断第一个点要不要删
			if (pre->adjvex == -999) {
				G.vertices[j].firstarc = next;
				free(pre);
				pre = next;
				next = next->nextarc;
			}
			//然后遍历后面所有点
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
//在图G中增加弧<v,w>，成功返回OK,否则返回ERROR
{
	int vo, wo; // 分别是v和w的位序
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
	while (temp) { //判断是否已存在路径<v,w>
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
//在图G中删除弧<v,w>，成功返回OK,否则返回ERROR
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
	while (temp) { //判断路径<v,w>是否存在
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
int visited[100];// 记录某点是否被访问过

status DFSTraverse(ALGraph &G, void (*visit)(VertexType))
//对图G进行深度优先搜索遍历，依次对图中的每一个顶点使用函数visit访问一次，且仅访问一次
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
//对图G进行广度优先搜索遍历，依次对图中的每一个顶点使用函数visit访问一次，且仅访问一次
{
	if (G.vexnum < 1)
		return ERROR;
	ArcNode *temp;
	int queue[100];// 存位序
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
//将图的数据写入到文件FileName中
{
	if (G.vexnum < 1)
		return INFEASIBLE;
	FILE *fp;
	fp = fopen(FileName, "w");
	ArcNode *temp;
	int count;
	fprintf(fp, "%d %d\n", G.vexnum, G.arcnum);
	for (int i = 0; i < G.vexnum; i++) { // 存储每个顶点的邻接点数
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

//读入文件FileName的图数据，创建图的邻接表
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
// 判断两个顶点是否直接相连，相邻返回1，否则返回999999（距离无限）
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
// 返回顶点v与顶点w的最短路径的长度
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
	//下面每次循环都添加一条最短路径
	for (i = 1; i < G.vexnum; i++) {
		t = 0;
		flag = -1;
		min = INF;
		for (j = 0; j < G.vexnum; j++)
			if (!visited[j] && d[j] < min) { //寻找未添加的最短路径
				min = d[j];
				flag = j;
				visited[flag] = 1;
			}
		if (flag == -1) //不连通
			break;
		for (j = 0; j < G.vexnum; j++) {
			temp = Near(G, flag, j);
			if (temp != INF)
				temp += min;
			if (!visited[j] && temp < d[j]) { //修改到j的最短路径
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
// 返回与顶点v距离小于k的顶点集合
{
	int count = 0;
	for (int i = 0; i < G.vexnum; i++)
		if (G.vertices[i].data.key != v && ShortestPathLength(G, v, G.vertices[i].data.key) < k) {
			visit(G.vertices[i].data);
			count++;
		}
	if (!count)
		printf("不存在与该顶点距离小于%d的顶点！\n", k);
}

int ConnectedComponentsNums(ALGraph &G)
// 返回无向图连通分量的个数
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
		printf("                 无向图操作演示系统 \n                计算机2110班  李嘉鹏\n");
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
		printf("    请选择操作[0~17]:\n");
		scanf("%d", &op);
		switch (op) {
			// 以下分别对应17个功能，根据输入情况执行对应的操作
			case 1:
				if (G.vexnum)
					printf("无向图创建失败！\n");
				else {
					printf("请输入顶点序列和关系对序列，分别以-1 nil和-1 -1结束：\n");
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
						printf("无向图创建成功！\n");
					else if (result == ERROR)
						printf("输入数据有误，无向图创建失败！\n");
				}
				getchar();
				getchar();
				break;
			case 2:
				if (!G.vexnum)
					printf("无向图为空！\n");
				else if (DestroyGraph(G) == OK)
					printf("无向图清空成功！\n");
				getchar();
				getchar();
				break;
			case 3:
				if (!G.vexnum)
					printf("无向图为空！\n");
				else {
					printf("请输入要查找顶点的关键字：");
					scanf("%d", &u);
					result = LocateVex(G, u);
					if (result == -1)
						printf("查找失败！\n");
					else
						printf("关键字为%d的结点在图中的位序为%d！\n", u, result);
				}
				getchar();
				getchar();
				break;
			case 4:
				if (!G.vexnum)
					printf("无向图为空！\n");
				else {
					printf("请输入要赋值顶点的关键字、新的关键字和名称，用空格隔开：");
					scanf("%d %d %s", &u, &value.key, value.others);
					result = PutVex(G, u, value);
					if (result == OK)
						printf("赋值成功！\n");
					else if (result == ERROR)
						printf("查找失败或关键字不唯一！\n");
				}
				getchar();
				getchar();
				break;
			case 5:
				if (!G.vexnum)
					printf("无向图为空！\n");
				else {
					printf("请输入要查找第一邻接点的顶点关键字：");
					scanf("%d", &u);
					result = FirstAdjVex(G, u);
					if (result == OK)
						printf("关键字为%d的顶点的第一邻接点位序为%d！\n", u, result);
					else if (result == -1)
						printf("查找失败或该顶点不存在邻接点！\n");
				}
				getchar();
				getchar();
				break;
			case 6:
				if (!G.vexnum)
					printf("无向图为空！\n");
				else {
					printf("请输入要查找的顶点关键字和其某一邻接点的关键字，用空格隔开：");
					scanf("%d %d", &v, &w);
					result = NextAdjVex(G, v, w);
					if (result == OK)
						printf("关键字为%d的顶点的邻接点%d的下一邻接点的位序为%d！\n", v, w);
					else if (result == -1)
						printf("查找失败！\n");
				}
				getchar();
				getchar();
				break;
			case 7:
				if (!G.vexnum)
					printf("无向图为空！\n");
				else {
					printf("请输入新顶点的关键字和名称，用空格隔开：");
					scanf("%d %s", &value.key, value.others);
					result = InsertVex(G, value);
					if (result == OK)
						printf("顶点 %d %s 插入成功！\n", value.key, value.others);
					else if (result == ERROR)
						printf("插入失败！\n");
				}
				getchar();
				getchar();
				break;
			case 8:
				if (!G.vexnum)
					printf("无向图为空！\n");
				else {
					printf("请输入要删除顶点的关键字：");
					scanf("%d", &v);
					result = DeleteVex(G, v);
					if (result == OK)
						printf("关键字%d对应的顶点与相关的弧删除成功！\n", v);
					else if (result == ERROR)
						printf("删除失败！\n");
				}
				getchar();
				getchar();
				break;
			case 9:
				if (!G.vexnum)
					printf("无向图为空！\n");
				else {
					printf("请输入要增加的弧两端顶点的关键字，用空格隔开：");
					scanf("%d %d", &v, &w);
					result = InsertArc(G, v, w);
					if (result == OK)
						printf("已成功增加顶点%d和%d之间的弧！\n", v, w);
					else if (result == ERROR)
						printf("弧增加失败！\n");
				}
				getchar();
				getchar();
				break;
			case 10:
				if (!G.vexnum)
					printf("无向图为空！\n");
				else {
					printf("请输入要删除的弧两端顶点的关键字，用空格隔开：");
					scanf("%d %d", &v, &w);
					result = DeleteArc(G, v, w);
					if (result == OK)
						printf("已成功删除顶点%d和%d之间的弧！\n", v, w);
					else if (result == ERROR)
						printf("弧删除失败！\n");
				}
				getchar();
				getchar();
				break;
			case 11:
				if (!G.vexnum)
					printf("无向图为空！\n");
				else {
					printf("\n-------------------深度优先搜索遍历结果-------------------\n\n");
					DFSTraverse(G, visit);
					printf("\n\n------------------------end-------------------------------\n");
				}
				getchar();
				getchar();
				break;
			case 12:
				if (!G.vexnum)
					printf("无向图为空！\n");
				else {
					printf("\n-------------------广度优先搜索遍历结果-------------------\n\n");
					BFSTraverse(G, visit);
					printf("\n\n------------------------end-------------------------------\n");
				}
				getchar();
				getchar();
				break;
			case 13:
				printf("请输入目标文件名：");
				scanf("%s", FileName);
				if (G.vexnum) {
					result = SaveGraph(G, FileName);
					if (result == OK)
						printf("无向图数据已成功写入文件%s！\n", FileName);
					else
						printf("写文件失败！\n");
				} else {
					result = LoadGraph(G, FileName);
					if (result == OK)
						printf("文件%s中的数据已成功读入到无向图中！\n", FileName);
					else
						printf("文件写入失败！\n");
				}
				getchar();
				getchar();
				break;
			case 14:
				if (!G.vexnum)
					printf("无向图为空！\n");
				else {
					printf("请输入顶点的关键字和k值（距离小于k），用空格隔开：");
					scanf("%d %d", &v, &k);
					printf("\n---------------与顶点%d距离小于%d的顶点集合-----------------\n\n", v, k);
					VerticesSetLessThanK(G, v, k);
					printf("\n\n------------------------end-------------------------------\n");
				}
				getchar();
				getchar();
				break;
			case 15:
				if (!G.vexnum)
					printf("无向图为空！\n");
				else {
					printf("请输入两个顶点的关键字，用空格隔开：");
					scanf("%d %d", &v, &w);
					result = ShortestPathLength(G, v, w);
					if (result == ERROR)
						printf("输入数据错或这两个顶点之间不存在通路！\n");
					else
						printf("顶点%d和%d之间最短路径的长度为%d！\n", v, w, result);
				}
				getchar();
				getchar();
				break;
			case 16:
				if (!G.vexnum)
					printf("无向图为空！\n");
				else {
					result = ConnectedComponentsNums(G);
					if (result == ERROR)
						printf("无向图中无顶点！\n");
					else
						printf("无向图中连通分量的个数为%d！\n", result);
				}
				getchar();
				getchar();
				break;
			case 17:
				printf("请选择功能：1.无向图添加；2.无向图移除；3.无向图查找\n");
				scanf("%d", &choice);
				if (choice == 1) {
					printf("请输入新无向图名称：");
					scanf("%s", GGG.elem[GGG.length].name);
					GGG.elem[GGG.length].G.arcnum = 0;
					GGG.elem[GGG.length].G.vexnum = 0;
					printf("请输入顶点序列和关系对序列，分别以-1 nil和-1 -1结束：\n");
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
						printf("无向图%s添加成功！\n", GGG.elem[GGG.length - 1].name);
					} else if (result == ERROR)
						printf("输入数据有误，无向图创建失败！\n");
				} else if (choice == 2) {
					if (GGG.length <= 0)
						printf("删除失败，无向图集合为空！\n");
					else {
						printf("请输入无向图名称：");
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
							printf("无向图%s已从无向图集合中删除！", GraphName);
						} else
							printf("无向图集合中没有名为%s的无向图！\n", GraphName);
					}
				} else if (choice == 3) {
					if (GGG.length <= 0)
						printf("无向图集合为空！\n");
					else {
						printf("请输入无向图名称：");
						scanf("%s", GraphName);
						int flag = 0;
						for (i = 0; i < GGG.length; i++)
							if (strcmp(GGG.elem[i].name, GraphName) == 0) {
								flag = 1;
								break;
							}
						if (flag) {
							G = GGG.elem[i].G;
							printf("查找成功！\n");
						} else
							printf("无向图集合中没有名为%s的无向图！\n", GraphName);
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