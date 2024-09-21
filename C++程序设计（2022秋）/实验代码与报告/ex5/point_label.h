#pragma once

#include <QLabel>
#include <qmainwindow.h>
#include <qlist.h>
#include <qpainter.h>
#include <QPaintEvent>
#include "point_dialog.h"
#include "LineMat.h"

/*在主窗口上画点和线。该类只用于画图*/
class point_label : public QLabel{
	Q_OBJECT

public:
	point_label(QMainWindow* parent);

	~point_label();

	// 绘制起点
	void draw_start_point(QPoint start_point);

	// 绘制终点
	void draw_end_point(QPoint end_point);

private:
	void paintEvent(QPaintEvent* e) override;

	// 鼠标悬停事件触发对话框
	bool eventFilter(QObject* obj, QEvent* event) override;
	
private:
	// 存储站点，索引从0开始
	QList<QPoint> points;

	// 标识：是否已经接收站点
	bool points_state;

	// 存储路线图，索引从0开始
	std::vector<std::vector<int>> stop_mat;

	// 存储路线总数
	int line_num;

	// 标识：是否已经接收路线
	bool line_state;

	// 存储每个点所在的路线，索引从0开始
	std::vector<std::vector<int>> line_of_points;

	// 存储起点
	QPoint start_point;

	// 存储终点
	QPoint end_point;

	// 存储最小转乘路线
	std::vector<int> min_cvt_line;
	// 是否绘制最小转乘路线
	bool min_cvt_line_state;

	// 存储最短距离路线
	std::vector<int> min_dist_line;

	// 是否绘制最小转乘路线
	bool min_dist_line_state;

	// 鼠标悬停对话框
	point_dialog* point_msg;

	// 站点的包络矩形区域，每个区域是一个label，目的是方便后面鼠标悬停显示信息
	std::vector<QLabel*> points_area;

public slots:
	// 接受信号存储点
	void store_points(QList<QPoint> points);

	// 接收信号存储路线
	void store_lines(std::vector<std::vector<int>> stop_mat, int line_num, std::vector<std::vector<int>> line_of_points);

	// 接受数据绘制最小转乘路线
	void store_min_cvt_line(std::vector<CompdLine> min_cvt_line);
	
	// 接收信号绘制最短距离路线
	void store_min_dist_line(CompdLine min_dist_line);
	
	//根据站点生成每个点的包络区域
	void gen_point_area();
	
signals:
	// 接受到站点之后就发出信号绘制站点区域label方便后面鼠标悬浮事件的判断
	void gen_point_area_sig();
};