#pragma once

#include <QLabel>
#include <qmainwindow.h>
#include <qlist.h>
#include <qpainter.h>
#include <QPaintEvent>
#include "point_dialog.h"
#include "LineMat.h"

/*���������ϻ�����ߡ�����ֻ���ڻ�ͼ*/
class point_label : public QLabel{
	Q_OBJECT

public:
	point_label(QMainWindow* parent);

	~point_label();

	// �������
	void draw_start_point(QPoint start_point);

	// �����յ�
	void draw_end_point(QPoint end_point);

private:
	void paintEvent(QPaintEvent* e) override;

	// �����ͣ�¼������Ի���
	bool eventFilter(QObject* obj, QEvent* event) override;
	
private:
	// �洢վ�㣬������0��ʼ
	QList<QPoint> points;

	// ��ʶ���Ƿ��Ѿ�����վ��
	bool points_state;

	// �洢·��ͼ��������0��ʼ
	std::vector<std::vector<int>> stop_mat;

	// �洢·������
	int line_num;

	// ��ʶ���Ƿ��Ѿ�����·��
	bool line_state;

	// �洢ÿ�������ڵ�·�ߣ�������0��ʼ
	std::vector<std::vector<int>> line_of_points;

	// �洢���
	QPoint start_point;

	// �洢�յ�
	QPoint end_point;

	// �洢��Сת��·��
	std::vector<int> min_cvt_line;
	// �Ƿ������Сת��·��
	bool min_cvt_line_state;

	// �洢��̾���·��
	std::vector<int> min_dist_line;

	// �Ƿ������Сת��·��
	bool min_dist_line_state;

	// �����ͣ�Ի���
	point_dialog* point_msg;

	// վ��İ����������ÿ��������һ��label��Ŀ���Ƿ�����������ͣ��ʾ��Ϣ
	std::vector<QLabel*> points_area;

public slots:
	// �����źŴ洢��
	void store_points(QList<QPoint> points);

	// �����źŴ洢·��
	void store_lines(std::vector<std::vector<int>> stop_mat, int line_num, std::vector<std::vector<int>> line_of_points);

	// �������ݻ�����Сת��·��
	void store_min_cvt_line(std::vector<CompdLine> min_cvt_line);
	
	// �����źŻ�����̾���·��
	void store_min_dist_line(CompdLine min_dist_line);
	
	//����վ������ÿ����İ�������
	void gen_point_area();
	
signals:
	// ���ܵ�վ��֮��ͷ����źŻ���վ������label���������������¼����ж�
	void gen_point_area_sig();
};