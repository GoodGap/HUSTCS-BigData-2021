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

	// û��ָ���Ա��Ĭ������
	~search_widget() = default;

	// ���ز�ѯ���λ��
	int get_x();
	int get_y();

private:
	Ui::search_widgetClass ui;

	// վ�����ݿ�
	std::vector<sp> data;

	// ��ѯ���λ��
	int pos_x, pos_y;

private:
	// �ַ���ģ��ƥ�䣬�������ݿ������ģ��ƥ����±�
	int fuzzy_match(std::string target);

	// ����ģ��ƥ�䲢����վ��λ��
	void search();

private slots:
	// �������ģ��ͨ��
	// �������
	void set_start();

	// �������ģ��ͨ��
	// �����յ�
	void set_end();

signals:
	void send_search_pos(int x, int y, int kind);
};