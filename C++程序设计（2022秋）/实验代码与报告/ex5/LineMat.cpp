#include "LineMat.h"
#include <iostream>

CalLine::CalLine(const GraphInfo& info, int max_method){
	this->line_num = info.line_num;
	this->line_of_points = info.line_of_points;
	this->cvt_mat = CvtMat(max_method, info);
	this->start = info.start_stop;
	this->end = info.end_stop;

	// ��ʼ��can_convert����
	can_convert = std::vector<std::vector<int>>(line_num, std::vector<int>(line_num, 0));
	CvtTup new_cvt_tuple;
	for (int i = 0; i < line_of_points.size(); i++) {
		for (int j = 0; j < line_of_points[i].size(); j++) {
			for (int k = 0; k < line_of_points[i].size(); k++) {
				if (j != k) {
					can_convert[line_of_points[i][j]][line_of_points[i][k]] = 1;  // ����·�߼����ת��
					// ͬʱ��¼ת�˵ķ�ʽ
					new_cvt_tuple = CvtTup(
						NONE,
						NONE,
						line_of_points[i][j],
						i,
						line_of_points[i][k]
					);
					this->cvt_mat[line_of_points[i][j]][line_of_points[i][k]].push_back(new_cvt_tuple);
				}
			}
		}
	}
}

void CalLine::gen_convert_mat(const GraphInfo& info) {
	
}


CalMinCvtLine::CalMinCvtLine(const GraphInfo& info, int max_method):\
	CalLine(info, max_method) {
	
}

void CalMinCvtLine::gen_convert_mat(const GraphInfo& info) {//����ת�˾���
	CvtTup new_method, p_old, p_new;
	std::vector<std::vector<int>> can_convert_1 = can_convert;  // �������A
	std::vector<std::vector<int>> next_can_convert = can_convert;  // ��һ����
	for (int cnt = 2; cnt <= line_num - 1; cnt++) {  // ���㵽A6����
		//cntΪ������
		for (int i = 0; i < line_num; i++) {  // ���������ѭ���Ǽ�������
			for (int j = 0; j < line_num; j++) {
				if (i == j) continue;
				for (int k = 0; k < line_num; k++) {
					if (i == k || j == k) continue;
					if (can_convert[i][k] * can_convert_1[k][j] != 0) {
						// ��i����ת�˵�j
						next_can_convert[i][j] = 1;
						// ��¼ת�˷�ʽ: ������·�ߣ����ѡȡmax_method�ַ���
						std::vector<CvtTup>& i_k_method = cvt_mat[i][k], & k_j_method = cvt_mat[k][j];
						for (int m = 0; m < i_k_method.size(); m++) {
							for (int n = 0; n < k_j_method.size(); n++) {
								if (cvt_mat[i][j].size() < cvt_mat.max_method)
									cvt_mat[i][j].push_back(i_k_method[m] + k_j_method[n]);
									//��δ�ﵽ��󷽷�������Ӵ˷���
								else {  // ���򵥵�ջ�㷨
									while (i_k_method[m].cvt_num() + k_j_method[n].cvt_num() < \
										cvt_mat[i][j][cvt_mat[i][j].size() - 1].cvt_num() && cvt_mat[i][j].size() > 0) {
										cvt_mat[i][j].pop_back();
									}
									if (cvt_mat[i][j].size() < cvt_mat.max_method)
										cvt_mat[i][j].push_back(i_k_method[m] + k_j_method[n]);
										//���ָ���ת��·�ߣ���Ӵ˷���
								}
							}
						}
					}
				}
			}
		}
		can_convert = next_can_convert;  // ����
	}
}

std::vector<CompdLine> CalMinCvtLine::min_convert_line(const GraphInfo& info) {
	// ����info�е�����
	start = info.start_stop;
	end = info.end_stop;

	bool if_convert = true;  // �Ƿ���Ҫת��
	int start_line, end_line;  // �����յ����ڵ�·��
	std::vector<ctt> methods;  // ���·�ߵ��յ�·�ߵ�����ת�˷���
	std::vector<CompdLine> line_methods;  // �������ת�˷�������

	for (int i = 0; i < line_of_points[start].size(); i++) {  // ��������ѭ��ö�������յ����ڵ�·�߶�
		for (int j = 0; j < line_of_points[end].size(); j++) {
			if (line_of_points[start][i] == line_of_points[end][j]) {
				CvtTup min_ctt_cvt_method(
					start,
					end,
					line_of_points[start][i],
					-1,
					line_of_points[end][j]
				);
				line_methods.push_back(min_ctt_cvt_method.to_line(info));
				return line_methods;
			}
			start_line = line_of_points[start][i];
			end_line = line_of_points[end][j];
			if (can_convert[start_line][end_line] == 0)  // ����������ᱨ��
				throw "error in can_convert!";
			// ��ʱ�ǰ���ת�����������еģ������ڴ˻����ϼ�����̾���·��
			std::vector<CvtTup> methods = cvt_mat[start_line][end_line];
			for (int k = 0; k < methods.size(); k++) {
				methods[k].set_start(start);
				methods[k].set_end(end);
				line_methods.push_back(methods[k].to_line(info));
			}
		}
	}
	return line_methods;
}


CalMinDistLine::CalMinDistLine(const GraphInfo& info, int max_method) :\
	CalLine(info, max_method){
	
}

void CalMinDistLine::gen_convert_mat(const GraphInfo& info) {//����ת�˾���
	CvtTup new_method, p_old, p_new;
	std::vector<std::vector<int>> can_convert_1 = can_convert;  // �������A
	std::vector<std::vector<int>> next_can_convert = can_convert;  // ��һ����
	for (int cnt = 2; cnt <= line_num - 1; cnt++) {  // ���㵽A6����
		for (int i = 0; i < line_num; i++) {
			for (int j = 0; j < line_num; j++) {
				if (i == j) continue;
				for (int k = 0; k < line_num; k++) {
					if (i == k || j == k) continue;
					if (can_convert[i][k] * can_convert_1[k][j] != 0) {
						// ������i����ת�˵�j
						next_can_convert[i][j] = 1;
						// ��¼ת�˷�ʽ: �����µ�·�ߣ����ѡȡmax_method�ַ���
						std::vector<CvtTup>& i_k_method = cvt_mat[i][k], &k_j_method = cvt_mat[k][j];
						for (int m = 0; m < i_k_method.size(); m++) {
							for (int n = 0; n < k_j_method.size(); n++) {
								if (cvt_mat[i][j].size() < cvt_mat.max_method)
									cvt_mat[i][j].push_back(i_k_method[m] + k_j_method[n]);
								else {  // ���򵥵�ջ�㷨
									while (i_k_method[m].cvt_num() + k_j_method[n].cvt_num() <\
										cvt_mat[i][j][cvt_mat[i][j].size() - 1].cvt_num() && cvt_mat[i][j].size() > 0) {
										cvt_mat[i][j].pop_back();
									}
									if (cvt_mat[i][j].size() < cvt_mat.max_method)
										cvt_mat[i][j].push_back(i_k_method[m] + k_j_method[n]);
								}
							}
						}
					}
				}
			}
		}
		can_convert = next_can_convert;  // ����
	}
}

CompdLine CalMinDistLine::min_dist_line(const GraphInfo& info) {
	// ����info�е�����
	start = info.start_stop;
	end = info.end_stop;

	bool if_convert = true;  // �Ƿ���Ҫת��
	int start_line, end_line;  // �����յ����ڵ�·��
	std::vector<ctt> methods;  // ���·�ߵ��յ�·�ߵ�����ת�˷���
	double min_dist = (double)INT16_MAX;  // ��̾���
	CvtTup min_tup;  // ��̾����Ӧ·��

	for (int i = 0; i < line_of_points[start].size(); i++) {  // ��������ѭ��ö�������յ����ڵ�·�߶�
		for (int j = 0; j < line_of_points[end].size(); j++) {
			if (line_of_points[start][i] == line_of_points[end][j]) {
				CvtTup min_ctt_cvt_method(
					start,
					end,
					line_of_points[start][i],
					-1,
					line_of_points[end][j]
				);
				return min_ctt_cvt_method.to_line(info);
			}
			start_line = line_of_points[start][i];
			end_line = line_of_points[end][j];
			if (can_convert[start_line][end_line] == 0)
				throw "error in can_convert!";
			
			// ��ʱ�ǰ���ת�����������еģ������ڴ˻����ϼ�����̾���·��
			std::vector<CvtTup> methods = cvt_mat[start_line][end_line];
			for (int k = 0; k < methods.size(); k++) {
				methods[k].set_start(start);
				methods[k].set_end(end);
				if (methods[k].length(info) < min_dist) {
					min_dist = methods[k].length(info);
					min_tup = methods[k];
				}
			}
		}
	}
	return min_tup.to_line(info);
}

double pt_dist(QPoint p1, QPoint p2) {
	int dist = pow(p1.x() - p2.x(), 2) + \
		pow(p1.y() - p2.y(), 2);
	return sqrt(dist);
}