#ifndef data_structure_project_2022_1
#define data_structure_project_2022_1
#include "definition.h"

void cnfopen(formula &original, char *input) {//����CNF�ļ�
	fp = fopen(input, "r");
	if (!fp)
		printf("cnf�ļ���ȡʧ�ܣ�\n");
	else {
		char c;
		int literal_read, literalnum_in_the_clause;
		int literal_read_temp[3500];//�洢���������
		literal *frontl = NULL;
		clause *frontc = NULL;
		while (fread(&c, sizeof(char), 1, fp))//���϶�ȡ�ַ���ֱ������pΪֹ
			if (c == 'p')
				break;
		fread(&c, sizeof(char), 5, fp);
		fscanf(fp, "%d", &original.literalnum);//����������
		fscanf(fp, "%d", &original.clausenum);//�����Ӿ���
		original.currentclausenum = original.clausenum;
		original.emptyclausenum = 0;
		for (int i = 1; i <= original.clausenum; i++) {
			literalnum_in_the_clause = 0;
			clause *newclause = (clause *)malloc(sizeof(clause));//�������Ӿ�
			if (i > 1)
				frontc->next = newclause;
			frontc = newclause;
			if (i == 1)
				original.head = newclause;//ȷ����ʽ��ͷ�Ӿ�
			while (fscanf(fp, "%d", &literal_read) != EOF && literal_read)
				literal_read_temp[++literalnum_in_the_clause] = literal_read;
			if (literalnum_in_the_clause > 0) {
				if (literalnum_in_the_clause == 1)
					original.singleclausenum++;
				literal *head = (literal *)malloc(sizeof(literal));
				head->word = literal_read_temp[1];
				head->next = NULL;
				newclause->head = head;
				frontl = head;
				for (int j = 2; j <= literalnum_in_the_clause; j++) { //����һ���Ӿ�����
					literal *newliteral = (literal *)malloc(sizeof(literal));
					newliteral->word = literal_read_temp[j];
					newliteral->next = NULL;
					frontl->next = newliteral;
					frontl = newliteral;
				}
			}
			newclause->next = NULL;
			newclause->num = literalnum_in_the_clause;
			literalnum_in_the_clause = 0;
		}
		printf("-----------------------------------------------------------\n");
		printf("cnf�ļ���ȡ��ɣ�������%d�����֣�%d���Ӿ䣬%d�����Ӿ䣡\n", original.literalnum, original.clausenum,
		       original.singleclausenum);
	}
	fclose(fp);
}

void printformula(formula &original) {//�����������ʽ
	clause *tempc = original.head;
	literal *templ = NULL;
	int clausecount = 0;
	printf("-----------------------------------------------------------\n");
	printf("cnf��ʽ������ʾ���£�������%d�����֣�%d���Ӿ䣺\n", original.literalnum, original.clausenum);
	while (tempc->next != NULL) {
		templ = tempc->head;
		printf("(�Ӿ�%d)   ", ++clausecount);
		while (templ->next != NULL) {
			printf("%d��", templ->word);
			templ = templ->next;
		}
		printf("%d\n", templ->word);
		tempc = tempc->next;
	}
	templ = tempc->head;
	printf("(�Ӿ�%d)   ", ++clausecount);
	while (templ->next != NULL) {
		printf("%d��", templ->word);
		templ = templ->next;
	}
	printf("%d\n", templ->word);
	printf("-----------------------------------------------------------\n");
	printf("������ɣ�\n");
}

void deleteformula(formula &original) {//ɾ��������ʽ
	clause *deletec = original.head, *deletetempc;
	while (deletec != NULL) {//��ͷ�Ӿ��������
		literal *deletel = deletec->head, *deletetempl;
		while (deletel != NULL) {//��ͷ�����������
			deletetempl = deletel->next;
			free(deletel);
			deletel = deletetempl;
		}
		deletetempc = deletec->next;
		free(deletec);
		deletec = deletetempc;
	}
	original.head = NULL;
	original.clausenum = 0;
	original.literalnum = 0;
	original.singleclausenum = 0;
	original.currentclausenum = 0;//��ʼ��
}

int findunitclause(formula &original) {//���ҵ��Ӿ�
	clause *find = original.head;
	if (!original.singleclausenum)//��ʽ��û�е��Ӿ�
		return 0;
	while (find != NULL) {
		if (find->num == 1)
			return find->head->word;
		find = find->next;
	}
	return 0;
}

void destroyclause(formula &original, int unitclause) { //ɾ�����а���ĳ���ֵ��Ӿ�
	clause *findc = original.head, *front;
	literal *findl = findc->head;
	while (findl != NULL && findc == original.head) {
		if (findl->word == unitclause) {
			original.head = findc->next; //ɾ����һ���Ӿ�
			if (findc->num == 1)
				original.singleclausenum--;//���Ӿ�����-1
			findc = findc->next;
			if (findc != NULL)
				findl = findc->head;
			else
				findl = NULL;
			original.currentclausenum--;
		} else
			findl = findl->next;
	}
	front = findc;
	if (findc != NULL)
		findc = findc->next;
	while (findc != NULL) { //�������������Ӿ�
		findl = findc->head;
		while (findl != NULL) {
			if (findl->word == unitclause) {
				front->next = findc->next;
				original.currentclausenum--;
				if (findc->num == 1)
					original.singleclausenum--;//���Ӿ�����-1
				break;
			}
			findl = findl->next;
		}
		if (findl == NULL)
			front = front->next;
		findc = findc->next;
	}
}

void deleteclause(formula &original, int negative) { //ȥ���Ӿ��еĸ�����
	int flag;//����ĳ�Ӿ����ҵ���������Ϊ1������Ϊ0
	clause *findc = original.head;
	literal *findl = original.head->head, *front;
	while (findc != NULL) {
		flag = 0;
		findl = findc->head;
		while (findl != NULL) {
			if (findl->word == negative) {
				flag = 1;
				findc->num--;
				if (!findc->num)
					original.emptyclausenum++;
				if (findl != findc->head) {
					front = findc->head;
					while (front->next != findl)
						front = front->next;
					front->next = findl->next;
					free(findl);
					front = front->next;
					findl = front;
				} else {
					if (findl->next == NULL) {
						free(findc->head);
						findc->head = NULL;
						break;
					} else {
						findc->head = findl->next;
						free(findl);
						findl = findc->head;
					}
				}
				continue;
			}
			findl = findl->next;
		}
		if (findc->num == 1 && flag == 1)
			original.singleclausenum++;
		findc = findc->next;
	}
}

int existemptyclause(formula &original) {//�ж��Ƿ���ڿ��Ӿ�
	clause *findc = original.head;
	while (findc != NULL) {
		if (!findc->num)
			return TRUE;
		findc = findc->next;
	}
	return FALSE;
}

int isemptyformula(formula &original) {//�жϹ�ʽ�Ƿ�Ϊ��
	if (!original.currentclausenum)
		return TRUE;
	return FALSE;
}

void copyformula(formula &temp, formula &original) {//����������ʽ���߶���д
	temp.clausenum = original.clausenum;
	temp.currentclausenum = original.currentclausenum;
	temp.literalnum = original.literalnum;
	temp.singleclausenum = original.singleclausenum;
	temp.emptyclausenum = original.emptyclausenum;//��ʼ��������������
	clause *copyc = original.head;
	literal *copyl;//����copyָ��ָ��ԭ������
	literal *frontl = NULL;
	clause *frontc = NULL;//����frontָ��ָ����������
	while (copyc != NULL) {
		clause *newclause = (clause *)malloc(sizeof(clause));
		newclause->num = copyc->num;
		copyl = copyc->head;
		while (copyl != NULL) {
			literal *newliteral = (literal *)malloc(sizeof(literal));
			newliteral->word = copyl->word;
			if (copyl == copyc->head) {
				newclause->head = newliteral;
				frontl = newliteral;
			} else {
				frontl->next = newliteral;
				frontl = frontl->next;
			}
			newliteral->next = NULL;
			copyl = copyl->next;
		}
		if (copyc == original.head) {
			frontc = newclause;
			temp.head = newclause;
		} else {
			frontc->next = newclause;
			frontc = frontc->next;
		}
		newclause->next = NULL;
		copyc = copyc->next;
	}
}

void addunitclause(formula &original, int key) { //��ԭ��ʽ����ӵ��Ӿ�
	clause *newclause = (clause *)malloc(sizeof(clause));
	if (original.head == NULL) {
		original.head = newclause;
		newclause->next = NULL;
	} else {
		newclause->next = original.head;
		original.head = newclause;
	}
	literal *singleliteral = (literal *)malloc(sizeof(literal));
	singleliteral->word = key;
	singleliteral->next = NULL;
	newclause->head = singleliteral;
	newclause->num = 1;
	original.currentclausenum++;//��ǰ�Ӿ���+1
	original.singleclausenum++;//���Ӿ���+1
}

int DPLL(formula &original, int strategy) {//DPLL�㷨�����ֲ��Ժ�һ
	int unitclause = findunitclause(original), key, max = 0;
	int countposi[3500] = {0}, countnega[3500] = {0}; //�ֱ�ͳ���������ֳ��ִ���
	while (unitclause) {//�����ҵ��Ӿ�
		if (unitclause > 0)
			result[unitclause] = 1;//������ȡ��
		else
			result[-unitclause] = 0; //������ȡ��
		destroyclause(original, unitclause); //�ӹ�ʽ��ɾ�����а��������ֵ��Ӿ�
		if (isemptyformula(original))
			return OK;//�ж������Ƿ�Ϊ�ռ�
		deleteclause(original, -unitclause); //ɾ�������Ӿ��еĸ�����
		if (existemptyclause(original)) //�ж������Ƿ��п��Ӿ䣨�������㣩
			return ERROR;
		unitclause = findunitclause(original);
	}
	formula temp;
	copyformula(temp, original); //�����¹�ʽtemp������DPLL�����з�֧
	if (strategy == 1) { //�Ľ�ǰ���ԣ����մ�ǰ�����˳��ѡȡ��Ԫkey
		key = original.head->head->word;
		addunitclause(original, key);//��ԭ��ʽ�м����±�Ԫ
		addunitclause(temp, -key);//��ԭ��ʽ�м��븺���±�Ԫ
	} else if (strategy == 2) { //�Ľ�����ԣ�ѡȡ���ִ�������key
		clause *searchc = original.head;
		literal *searchl;
		while (searchc != NULL) {
			searchl = searchc->head;
			while (searchl != NULL) {
				if (searchl->word > 0)
					countposi[searchl->word]++;
				else
					countnega[-searchl->word]++;
				searchl = searchl->next;
			}
			searchc = searchc->next;
		}
		for (int i = 1; i <= original.literalnum; i++)
			if (max < countposi[i]) {
				key = i;
				max = countposi[i];
			}
		if (!max)//�����������֣������ҳ��ִ������ĸ�����
			for (int i = 1; i <= original.literalnum; i++)
				if (max < countnega[i]) {
					key = i;
					max = countnega[i];
				}
		addunitclause(original, key);//��ԭ��ʽ�м����±�Ԫ
		addunitclause(temp, -key);//��ԭ��ʽ�м��븺���±�Ԫ
	}
	if (DPLL(original, strategy))//���Ѳ���
		return OK;
	else
		return DPLL(temp, strategy);
}

void saveres(char *output, formula &original, int time, int DPLLstatus) {//����������.res�ļ�
	if ((fp = fopen(output, "w")) == NULL)
		printf("����ʧ�ܣ�\n");
	else {
		fprintf(fp, "s %d\nv ", DPLLstatus);
		for (int i = 1; i <= original.clausenum; i++) {
			if (result[i] > 0)
				fprintf(fp, "%d ", i);
			else
				fprintf(fp, "-%d ", i);
		}
		fprintf(fp, "\n");
		fprintf(fp, "t %d", time);
	}
	fclose(fp);
}
#endif