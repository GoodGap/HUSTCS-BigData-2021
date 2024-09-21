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

	// û��ָ���Ա��Ĭ������
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
	// ��վ���ļ����Ұѵ���qpoint������ʽ�洢��������
	int read_stop_file(QString file_name);

	// ��·���ļ�����·����line����洢��������
	int read_line_file(QString file_name);

private:
	// uiǰ�˶���
	Ui::file_widgetClass ui;

	// ·���ļ���
	QString line_file_name = "";

	// վ���ļ���
	QString stop_file_name = "";

	// �洢վ��,������0��ʼ
	QList<QPoint> points;

	// �洢ÿ��·�ߵ���Ϣ��������0��ʼ��ÿ��·�ߵ���Ϣ��һ��vector��˳��洢����·���ϵ�վ��
	std::vector<std::vector<int>> line_info;

	// �洢վ�㹹�ɵ�����ͼ��������0��ʼ
	std::vector<std::vector<int>> stop_mat;
	
	// ·������
	int line_num;

	// �洢ÿ�������ڵ�·��
	//����0��վ��ͬʱ����2��3�����ϣ���line_of_points[0] = {2,3}
	std::vector<std::vector<int>> line_of_points;

signals:
	// վ���ļ���ȷ
	void true_stop_file(QList<QPoint> points);

	// ·���ļ���ȷ
	void true_line_file(std::vector<std::vector<int>> stop_mat, int line_num, std::vector<std::vector<int>> line_of_points);

public slots:
	// ��ȡվ����Ϣ�ļ�
	void stop_input();

	// ��ȡ·����Ϣ�ļ�
	void line_input();

	// ���ȷ���鿴�ļ�û�����Ⲣ�ҹرմ���
	void check_file();
};