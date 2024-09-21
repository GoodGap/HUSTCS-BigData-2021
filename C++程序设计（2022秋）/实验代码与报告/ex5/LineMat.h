#pragma once
#include <vector>
#include <QPoint>
#include <QList>
#include <iostream>

#define UNKNOWN -2
#define NO_LINE_NUMBER -1  // ����·�߲���Ҫ��������һ����
#define NONE -1  // û��ʹ��ĳ���ṹ���Ա

// ǰ������
class Line;
class CompdLine;

// �����������
double pt_dist(QPoint p1, QPoint p2);

// ����һЩ��Ϣ�Ľṹ��
typedef struct GraphInfoStruct GraphInfo;
struct GraphInfoStruct {
	// �洢�û�ѡ��������յ�
	int start_stop = NONE, end_stop = NONE;
	// ·������
	int line_num = NONE;
	// ��һ��·��,��ʹ��Ϊ-1
	int line_number = NONE;
	// ÿ�������ڵ�·��
	std::vector<std::vector<int>> line_of_points;
	// ÿ��·���ϵ�����У�������˳��洢
	std::vector<std::vector<int>> line_info;
	// վ����Ϣ
	QList<QPoint> points;
};

// ͬһ��·���ϵ�һ��·��
class Line {
private:
	int line_number;  // ����һ��·��

public:
	// ·���ϵ�վ��(��˳��)
	std::vector<int> stop_seq;  

	// ��㡢�յ�վ
	int start_stop, end_stop;  

public:
	Line() = default;

	// �������վ���յ�վ�Լ�����վ�����ڵ�·�ߡ�·����Ϣ������ͬһ��·���ϵ�һ��·
	Line(int start_stop, int end_stop, const GraphInfo& info) : \
		start_stop(start_stop), end_stop(end_stop), line_number(info.line_number) {
		std::vector<int> target = info.line_info[line_number];
		int start_stop_ind = -1, end_stop_ind = -1;
		for (int i = 0; i < target.size(); i++) {
			if (target[i] == start_stop)
				start_stop_ind = i;
			if (target[i] == end_stop)
				end_stop_ind = i;
		}
		if (start_stop_ind == -1 || end_stop_ind == -1)
			throw "stop ind error!";
		if (start_stop_ind < end_stop_ind)
			for (int i = start_stop_ind; i <= end_stop_ind; i++)
				stop_seq.push_back(target[i]);
		else
			for (int i = start_stop_ind; i >= end_stop_ind; i--)
				stop_seq.push_back(target[i]);
	}

	// û��ָ�룬�ͷ�
	~Line() = default;

	// ����·�߳���
	int length(const GraphInfo& info) {
		int len = 0;
		for (int i = 1; i < stop_seq.size(); i++)
			len += pt_dist(info.points[stop_seq[i - 1]], info.points[stop_seq[i]]);
		return len;
	}
};

// �������·�ߵ���
class CompdLine : public Line {
public:
	CompdLine(int start_stop, int end_stop, const GraphInfo& info) :\
		Line(start_stop, end_stop, info) {
	}
	~CompdLine() = default;

	void operator+=(Line part_line) {
		if (this->end_stop != part_line.start_stop)
			throw "two line can't be catted!";
		else {
			for (int i = 1; i < part_line.stop_seq.size(); i++)  // ��startstop��һ���㿪ʼ
				this->stop_seq.push_back(part_line.stop_seq[i]);
			this->end_stop = part_line.end_stop;
		}
	}
};

// ת����Ԫ��
struct convert_tuple;
typedef struct convert_tuple* ctt;
struct convert_tuple {
	int from;
	int by;
	int to;
	ctt next;  // ����

	convert_tuple(): from(-1), by(-1), to(-1), next(nullptr) {
	}

	convert_tuple(int from, int by, int to): next(nullptr) {
		this->from = from;
		this->by = by;
		this->to = to;
	}

	// ������캯����ֻ������Ԫ��
	convert_tuple(const ctt& one_tuple) {
		this->from = one_tuple->from;
		this->by = one_tuple->by;
		this->to = one_tuple->to;
		this->next = nullptr;
	}

	~convert_tuple() {
		// �ͷſռ�
		if (next == nullptr) return;
		delete next;
		next = nullptr;
	}

	// ���һ���µ�ת����Ԫ�����ӵ�����ĩβ
	void append(const ctt& new_ctt) {
		ctt p = this;
		while (p->next != nullptr)
			p = p->next;
		ctt p_new_ctt = new_ctt;
		while (p_new_ctt) {
			p->next = new convert_tuple(p_new_ctt);
			p = p->next;
			p_new_ctt = p_new_ctt->next;
		}
	}

	// ����ӵ�ǰ��Ԫ�鵽��β�ĳ���
	int length() {
		ctt p = this;
		int len = 0;
		while (p) {
			len++;
			p = p->next;
		}
		return len;
	}
};

// ��ctt������������start/end,����������·������
class CvtTup {
private:
	// ΪNoneʱ������
	int start, end;
	
	// ��ͷ
	ctt cvt_tup;  

public:
	CvtTup() : cvt_tup(nullptr), start(-1), end(-1) {};

	CvtTup(int start, int end, int from, int by, int to): start(start), end(end) {
		cvt_tup = new convert_tuple(from, by, to);
	}

	// �������
	CvtTup(const CvtTup& origin) {
		cvt_tup = new convert_tuple(origin.cvt_tup);
		//std::cout << "cvttup ctor" << std::endl;
		ctt p_old = origin.cvt_tup;
		ctt p_new = cvt_tup;
		while (p_old->next) {  // ����ʱp_newָ����·�ߵ�β��
			p_old = p_old->next;
			p_new->next = new convert_tuple(p_old);
			p_new = p_new->next;
		}
		start = origin.start;
		end = origin.end;
	}

	~CvtTup() {
		delete cvt_tup;
	}

	// �����ֵ
	CvtTup& operator=(const CvtTup& origin) {
		if (this->cvt_tup != nullptr) {
			delete this->cvt_tup;
			this->cvt_tup = nullptr;
		}
		cvt_tup = new convert_tuple(origin.cvt_tup);
		ctt p_old = origin.cvt_tup;
		ctt p_new = cvt_tup;
		while (p_old->next) {  // ����ʱp_newָ����·�ߵ�β��
			p_old = p_old->next;
			p_new->next = new convert_tuple(p_old);
			p_new = p_new->next;
		}
		start = origin.start;
		end = origin.end;
		return *this;
	}

	// ����ƶ���ֵ
	CvtTup& operator=(const CvtTup&& origin) {
		if (this->cvt_tup != nullptr) {
			delete this->cvt_tup;
			this->cvt_tup = nullptr;
		}
		cvt_tup = origin.cvt_tup;
		((CvtTup&)origin).cvt_tup = nullptr;
		start = origin.start;
		end = origin.end;
		return *this;
	}

	// ���������յ�
	void set_start(int start) { this->start = start; }

	void set_end(int end) { this->end = end; }

	// ����ctt��ת�˴���
	int cvt_num() {
		return cvt_tup->length();
	}

	// ����·������
	int length(const GraphInfo& info) {
		int len = 0, cur_start = start;
		GraphInfo cur_info = info;
		ctt p = cvt_tup;
		if (p->by == -1) {  // ��������ֱ��
			cur_info.line_number = p->from;
			len = Line(start, end, cur_info).length(cur_info);
		}
		else {
			//int last_line_num = -1;
			cur_info.line_number = p->from;
			while (p) {
				//cur_info.line_number = p->from;
				len += Line(cur_start, p->by, cur_info).length(cur_info);
				cur_start = p->by;
				cur_info.line_number = p->to;
				p = p->next;
			}
			len += Line(cur_start, end, cur_info).length(cur_info);
		}
		return len;
	}

	void operator+=(const CvtTup& right_value) {
		if (this->end != this->start) throw "error in cat CvtTuple!";
		CvtTup right_value_cp(right_value);  // ���
		this->cvt_tup->append(right_value_cp.cvt_tup);
		this->end = right_value.end;
	}

	CvtTup operator+(const CvtTup& right_value) {
		CvtTup add_tup(*this);
		add_tup += right_value;
		return add_tup;
	}

	// ת��Ϊ·��
	CompdLine to_line(const GraphInfo& info) {
		if (start == NONE || end == NONE)
			throw "error: CvtTup has not set start or end!";
		int cur_start = start;
		GraphInfo cur_info = info;
		ctt p = cvt_tup;
		if (p->by == -1) {  // ��������ֱ��
			cur_info.line_number = p->from;
			CompdLine cpd_line(start, end, cur_info);
			return cpd_line;
		}
		else {
			cur_info.line_number = p->from;
			CompdLine cpd_line(cur_start, p->by, cur_info);
			cur_start = p->by;
			cur_info.line_number = p->to;
			p = p->next;
			while (p) {
				cpd_line += Line(cur_start, p->by, cur_info);
				cur_start = p->by;
				cur_info.line_number = p->to;
				p = p->next;
			}
			cpd_line += Line(cur_start, end, cur_info);
			return cpd_line;
		}
	}
};

// ת�˾���
class CvtMat {
private:
	std::vector<std::vector<std::vector<CvtTup>>> cvt_mat;

public:
	int max_method;  // �洢�����Դ漸��·��

public:
	CvtMat() = default;
	CvtMat(int max_method, const GraphInfo& info) : max_method(max_method) {
		cvt_mat = std::vector<std::vector<std::vector<CvtTup>>>(info.line_num,
			std::vector<std::vector<CvtTup>>(info.line_num));
	}
	~CvtMat() = default;

	std::vector<std::vector<CvtTup>>& operator[](int ind) {
		return cvt_mat[ind];
	}
};

// ����·�ߵ������Ϣ�Ļ���
class CalLine{
protected:
	// �����յ�
	int start, end;

	// ·����
	int line_num;

	// ÿ�������ڵ�·�ߣ�������0��ʼ
	std::vector<std::vector<int>> line_of_points;

	// ��ĳ���ߵ���һ���ߵ�ת�˷�ʽ,ctt������������з�ʽ, Ŀǰֻ�洢ת����/������̵ģ�������0��ʼ
	CvtMat cvt_mat;  

	// �жϴ�ĳ���ߵ���һ�����ܷ�ת�ˣ�������0��ʼ
	std::vector<std::vector<int>> can_convert;

public:
	CalLine() = default;

	CalLine(const GraphInfo& info, int max_method);

	~CalLine() = default;

	// ����can_convert���󣬼�A
	void gen_convert_mat(const GraphInfo& info);  

};

// ������Сת�˵���
class CalMinCvtLine : public CalLine {

public:
	CalMinCvtLine(const GraphInfo& info, int max_method);

	void gen_convert_mat(const GraphInfo& info);

	// ������Сת��·��
	std::vector<CompdLine> min_convert_line(const GraphInfo& info);
};

// ������̾���·�ߵ���
class CalMinDistLine: public CalLine {

public:
	CalMinDistLine(const GraphInfo& info, int max_method);

	void gen_convert_mat(const GraphInfo& info);

	// ������̾���
	CompdLine min_dist_line(const GraphInfo& info);
};