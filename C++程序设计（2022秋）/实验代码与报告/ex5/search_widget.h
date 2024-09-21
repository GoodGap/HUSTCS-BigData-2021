#pragma once

#include <QWidget>
#include "ui_search_widget.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

typedef struct store_points sp;
struct store_points {
	std::string name;
	int x;
	int y;
};

class search_widget : public QWidget
{
	Q_OBJECT

public:
	search_widget(QWidget *parent = nullptr);

	// 没有指针成员，默认析构
	~search_widget() = default;

	// 返回查询点的位置
	int get_x();
	int get_y();

private:
	Ui::search_widgetClass ui;

	// 站点数据库
	std::vector<sp> data;

	// 查询点的位置
	int pos_x, pos_y;

private:
	// 字符串模糊匹配，返回数据库中最大模糊匹配的下标
	int fuzzy_match(std::string target);

	// 调用模糊匹配并保存站点位置
	void search();

private slots:
	// 负责和主模块通信
	// 传递起点
	void set_start();

	// 负责和主模块通信
	// 传递终点
	void set_end();

signals:
	void send_search_pos(int x, int y, int kind);
};