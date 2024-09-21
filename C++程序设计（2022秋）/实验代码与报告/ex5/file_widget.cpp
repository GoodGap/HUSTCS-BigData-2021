#include "file_widget.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <QString>
#include <QFile>
#include <QFileDialog.h>
#include <QIODevice>

file_widget::file_widget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

void file_widget::stop_input()
{
	QString file_name = QFileDialog::getOpenFileName(nullptr, "select stop file", ".", "*.txt");
	ui.textEditStop->append(file_name);
}

void file_widget::line_input()
{
	QString file_name = QFileDialog::getOpenFileName(nullptr, "select line file", ".", "*.txt");
	ui.textEditLine->append(file_name);
}

std::vector<std::vector<int>> file_widget::get_line_info() {
	return this->line_info;
}

QList<QPoint> file_widget::get_points() {
	return this->points;
}

int file_widget::get_line_num() {
	return this->line_num;
}
std::vector<std::vector<int>> file_widget::get_line_of_points() {
	return this->line_of_points;
}

int file_widget::read_stop_file(QString file_name)
{
	std::ifstream stop_file(file_name.toStdString(), std::ios::in);
	if (stop_file) {
		int x, y, n;
		stop_file >> n;
		line_of_points = std::vector<std::vector<int>>(n);
		for (int i = 0; i < n; i++) {
			stop_file >> x >> y;
			QPoint cur_point(x, y);
			points.append(cur_point);
		}
		stop_file.close();
		return 1;
	}
	else {
		std::cout << "stop file open failed" << std::endl;
		return 0;
	}
}

int file_widget::read_line_file(QString file_name)
{
	std::ifstream line_file(file_name.toStdString(), std::ios::in);
	if (line_file) {
		int n, len;
		line_file >> n;
		this->line_num = n;
		std::vector<int> line_len;  // 临时存储每条路线的长度
		for (int i = 0; i < n; i++) {
			line_file >> len;
			line_len.push_back(len);
		}
		// 把图存储在二维数组中
		stop_mat = std::vector<std::vector<int>>(points.size(), std::vector<int>(points.size(), 0));
		int last_stop = 0;  // 存储上一个读入的站点
		int cur_stop = 0;  // 存储当前读入站点
		std::vector<int> cur_line;  // 存储当前路线上的站点
		for (int i = 0; i < n; i++) {
			line_file >> last_stop;
			line_of_points[last_stop - 1].push_back(i);
			cur_line.clear();
			cur_line.push_back(last_stop - 1);
			for (int j = 1; j < line_len[i]; j++) {
				line_file >> cur_stop;
				cur_line.push_back(cur_stop-1);  // 按顺序存储每条路线上的站点
				line_of_points[cur_stop - 1].push_back(i);
				// 无向图存储
				stop_mat[last_stop-1][cur_stop-1] = 1;
				stop_mat[cur_stop-1][last_stop-1] = 1;
				last_stop = cur_stop;
			}
			line_info.push_back(cur_line);
		}
		line_file.close();
		return 1;
	}
	else {
		std::cout << "line file open failed" << std::endl;
		return 0;
	}
}

void file_widget::check_file()
{
	// 从TextEdit获取文件名
	this->stop_file_name = ui.textEditStop->toPlainText();
	this->line_file_name = ui.textEditLine->toPlainText();
	if (this->stop_file_name.isEmpty() || this->line_file_name.isEmpty())
		ui.CheckLabel->setText(QString::fromStdString("input file is empty or the directory has Chinese symbol!").toUtf8());
	else if (this->read_stop_file(this->stop_file_name) && this->read_line_file(this->line_file_name)) {  // 文件读入正确
		emit true_stop_file(this->points);
		emit true_line_file(stop_mat, this->line_num, this->line_of_points);
		this->close();
	}
	else
		ui.CheckLabel->setText(QString::fromStdString("input file is empty or the directory has Chinese symbol!").toUtf8());
}