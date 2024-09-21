#include "search_widget.h"

search_widget::search_widget(QWidget *parent)
	: QWidget(parent), pos_x(-1), pos_y(-1)
{
	ui.setupUi(this);

	// 读取数据库文件
	std::ifstream org_data("./data/organization.txt", std::ios::in);
	if (org_data) {
		std::string stop_name;
		int x, y;
		while (1) {
			org_data >> stop_name;
			if (org_data.fail()) break;
			org_data >> x >> y;
			data.push_back(sp{ stop_name, x, y });
		}
	}
	else
		throw "File open failed";
}

int search_widget::get_x() {
	return this->pos_x;
}

int search_widget::get_y() {
	return this->pos_y;
}

int search_widget::fuzzy_match(std::string target) {
	int max_n = -1, best_ind = -1;
	int cur_n;
	for (int i = 0; i < data.size(); i++) {
		cur_n = 0;
		for (int j = 0; j < target.size(); j++)
			if (data[i].name.find(target[j]) != std::string::npos)
				cur_n++;
		if (cur_n > max_n) {
			max_n = cur_n;
			best_ind = i;
		}
	}
	return best_ind;
}

void search_widget::search() {
	std::string stop_name = ui.text_search->toPlainText().toStdString();  // 获取查找名
	int best_match_ind = fuzzy_match(stop_name);
	pos_x = data[best_match_ind].x;
	pos_y = data[best_match_ind].y;
}

void search_widget::set_start() {
	search();
	emit send_search_pos(pos_x, pos_y, 0);  // 设为起点
	this->close();  // 查找完后关闭搜索框
}

void search_widget::set_end() {
	search();
	emit send_search_pos(pos_x, pos_y, 1);  // 设为终点
	this->close();
}