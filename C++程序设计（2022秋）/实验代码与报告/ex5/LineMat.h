#pragma once
#include <vector>
#include <QPoint>
#include <QList>
#include <iostream>

#define UNKNOWN -2
#define NO_LINE_NUMBER -1  // 复合路线不需要表明是哪一条线
#define NONE -1  // 没有使用某个结构体成员

// 前向声明
class Line;
class CompdLine;

// 计算两点距离
double pt_dist(QPoint p1, QPoint p2);

// 集合一些信息的结构体
typedef struct GraphInfoStruct GraphInfo;
struct GraphInfoStruct {
	// 存储用户选择的起点和终点
	int start_stop = NONE, end_stop = NONE;
	// 路线总数
	int line_num = NONE;
	// 哪一条路线,不使用为-1
	int line_number = NONE;
	// 每个点所在的路线
	std::vector<std::vector<int>> line_of_points;
	// 每条路线上点的序列，按读入顺序存储
	std::vector<std::vector<int>> line_info;
	// 站点信息
	QList<QPoint> points;
};

// 同一条路线上的一段路线
class Line {
private:
	int line_number;  // 是哪一条路线

public:
	// 路线上的站点(按顺序)
	std::vector<int> stop_seq;  

	// 起点、终点站
	int start_stop, end_stop;  

public:
	Line() = default;

	// 传入起点站和终点站以及两个站点所在的路线、路线信息，构造同一条路线上的一条路
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

	// 没有指针，释放
	~Line() = default;

	// 计算路线长度
	int length(const GraphInfo& info) {
		int len = 0;
		for (int i = 1; i < stop_seq.size(); i++)
			len += pt_dist(info.points[stop_seq[i - 1]], info.points[stop_seq[i]]);
		return len;
	}
};

// 存放最终路线的类
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
			for (int i = 1; i < part_line.stop_seq.size(); i++)  // 从startstop后一个点开始
				this->stop_seq.push_back(part_line.stop_seq[i]);
			this->end_stop = part_line.end_stop;
		}
	}
};

// 转乘三元组
struct convert_tuple;
typedef struct convert_tuple* ctt;
struct convert_tuple {
	int from;
	int by;
	int to;
	ctt next;  // 链表

	convert_tuple(): from(-1), by(-1), to(-1), next(nullptr) {
	}

	convert_tuple(int from, int by, int to): next(nullptr) {
		this->from = from;
		this->by = by;
		this->to = to;
	}

	// 深拷贝构造函数，只复制三元组
	convert_tuple(const ctt& one_tuple) {
		this->from = one_tuple->from;
		this->by = one_tuple->by;
		this->to = one_tuple->to;
		this->next = nullptr;
	}

	~convert_tuple() {
		// 释放空间
		if (next == nullptr) return;
		delete next;
		next = nullptr;
	}

	// 添加一个新的转乘三元组链接到链的末尾
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

	// 计算从当前三元组到表尾的长度
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

// 在ctt基础上增加了start/end,方便后面计算路径长度
class CvtTup {
private:
	// 为None时不启用
	int start, end;
	
	// 表头
	ctt cvt_tup;  

public:
	CvtTup() : cvt_tup(nullptr), start(-1), end(-1) {};

	CvtTup(int start, int end, int from, int by, int to): start(start), end(end) {
		cvt_tup = new convert_tuple(from, by, to);
	}

	// 深拷贝构造
	CvtTup(const CvtTup& origin) {
		cvt_tup = new convert_tuple(origin.cvt_tup);
		//std::cout << "cvttup ctor" << std::endl;
		ctt p_old = origin.cvt_tup;
		ctt p_new = cvt_tup;
		while (p_old->next) {  // 结束时p_new指向新路线的尾部
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

	// 深拷贝赋值
	CvtTup& operator=(const CvtTup& origin) {
		if (this->cvt_tup != nullptr) {
			delete this->cvt_tup;
			this->cvt_tup = nullptr;
		}
		cvt_tup = new convert_tuple(origin.cvt_tup);
		ctt p_old = origin.cvt_tup;
		ctt p_new = cvt_tup;
		while (p_old->next) {  // 结束时p_new指向新路线的尾部
			p_old = p_old->next;
			p_new->next = new convert_tuple(p_old);
			p_new = p_new->next;
		}
		start = origin.start;
		end = origin.end;
		return *this;
	}

	// 深拷贝移动赋值
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

	// 设置起点和终点
	void set_start(int start) { this->start = start; }

	void set_end(int end) { this->end = end; }

	// 计算ctt链转乘次数
	int cvt_num() {
		return cvt_tup->length();
	}

	// 计算路径长度
	int length(const GraphInfo& info) {
		int len = 0, cur_start = start;
		GraphInfo cur_info = info;
		ctt p = cvt_tup;
		if (p->by == -1) {  // 表明可以直达
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
		CvtTup right_value_cp(right_value);  // 深拷贝
		this->cvt_tup->append(right_value_cp.cvt_tup);
		this->end = right_value.end;
	}

	CvtTup operator+(const CvtTup& right_value) {
		CvtTup add_tup(*this);
		add_tup += right_value;
		return add_tup;
	}

	// 转换为路线
	CompdLine to_line(const GraphInfo& info) {
		if (start == NONE || end == NONE)
			throw "error: CvtTup has not set start or end!";
		int cur_start = start;
		GraphInfo cur_info = info;
		ctt p = cvt_tup;
		if (p->by == -1) {  // 表明可以直达
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

// 转乘矩阵
class CvtMat {
private:
	std::vector<std::vector<std::vector<CvtTup>>> cvt_mat;

public:
	int max_method;  // 存储最多可以存几条路线

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

// 计算路线的相关信息的基类
class CalLine{
protected:
	// 起点和终点
	int start, end;

	// 路线数
	int line_num;

	// 每个点所在的路线，索引从0开始
	std::vector<std::vector<int>> line_of_points;

	// 从某条线到另一条线的转乘方式,ctt数组组成了所有方式, 目前只存储转乘数/距离最短的，索引从0开始
	CvtMat cvt_mat;  

	// 判断从某条线到另一条线能否转乘，索引从0开始
	std::vector<std::vector<int>> can_convert;

public:
	CalLine() = default;

	CalLine(const GraphInfo& info, int max_method);

	~CalLine() = default;

	// 生成can_convert矩阵，即A
	void gen_convert_mat(const GraphInfo& info);  

};

// 计算最小转乘的类
class CalMinCvtLine : public CalLine {

public:
	CalMinCvtLine(const GraphInfo& info, int max_method);

	void gen_convert_mat(const GraphInfo& info);

	// 计算最小转乘路线
	std::vector<CompdLine> min_convert_line(const GraphInfo& info);
};

// 计算最短距离路线的类
class CalMinDistLine: public CalLine {

public:
	CalMinDistLine(const GraphInfo& info, int max_method);

	void gen_convert_mat(const GraphInfo& info);

	// 计算最短距离
	CompdLine min_dist_line(const GraphInfo& info);
};