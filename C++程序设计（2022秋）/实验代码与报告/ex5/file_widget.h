/*
This file includes functions of reading map/stops/lines.
*/

#pragma once

#include <QWidget>
#include <QPoint>
#include <string>
#include <QString>
#include <QList>
#include "ui_file_widget.h"

class file_widget : public QWidget
{
	Q_OBJECT

public:
	file_widget(QWidget *parent = nullptr);

	// 没有指针成员，默认析构
	~file_widget() = default;

	// Api for getting line info.
	std::vector<std::vector<int>> get_line_info();

	// Api for getting points info.
	QList<QPoint> get_points();

	// Api for getting line_num info.
	int get_line_num();

	// Api for getting line_of_points info.
	std::vector<std::vector<int>> get_line_of_points();

private:
	// 打开站点文件并且把点以qpoint对象形式存储到数组中
	int read_stop_file(QString file_name);

	// 打开路线文件并把路线以line对象存储在数组中
	int read_line_file(QString file_name);

private:
	// ui前端对象
	Ui::file_widgetClass ui;

	// 路线文件名
	QString line_file_name = "";

	// 站点文件名
	QString stop_file_name = "";

	// 存储站点,索引从0开始
	QList<QPoint> points;

	// 存储每条路线的信息，索引从0开始。每条路线的信息用一个vector按顺序存储这条路线上的站点
	std::vector<std::vector<int>> line_info;

	// 存储站点构成的无向图，索引从0开始
	std::vector<std::vector<int>> stop_mat;
	
	// 路线总数
	int line_num;

	// 存储每个点所在的路线
	//例如0号站点同时处在2、3号线上，则line_of_points[0] = {2,3}
	std::vector<std::vector<int>> line_of_points;

signals:
	// 站点文件正确
	void true_stop_file(QList<QPoint> points);

	// 路线文件正确
	void true_line_file(std::vector<std::vector<int>> stop_mat, int line_num, std::vector<std::vector<int>> line_of_points);

public slots:
	// 读取站点信息文件
	void stop_input();

	// 读取路线信息文件
	void line_input();

	// 点击确定查看文件没有问题并且关闭窗口
	void check_file();
};