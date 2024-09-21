#include "point_label.h"
#include <Windows.h>

point_label::point_label(QMainWindow* parent): QLabel(parent), points_state(false), \
    line_state(false), line_num(0), min_cvt_line_state(false), min_dist_line_state(false) {
    point_msg = new point_dialog(this);
    connect(this, &point_label::gen_point_area_sig, this, &point_label::gen_point_area);
}

point_label::~point_label(){
    delete point_msg;
}

void point_label::draw_start_point(QPoint start_point){
    this->start_point = start_point;
    update();
}

void point_label::draw_end_point(QPoint end_point)
{
    this->end_point = end_point;
    update();
}

void point_label::paintEvent(QPaintEvent* e){
    QPainter painter(this);
    QPen pen;
	if (points_state) {
        pen.setColor(Qt::green);
        pen.setWidth(15);
        pen.setJoinStyle(Qt::RoundJoin);
        pen.setCapStyle(Qt::RoundCap);
        painter.setPen(pen);
        for (int i = 0; i < points.size(); i++)
            painter.drawPoint(points[i]);
	}
    if (line_state && points_state) {
        pen.setColor(Qt::green);
        pen.setWidth(5);
        painter.setPen(pen);
        for (int i = 0; i < points.size(); i++)
            for (int j = i; j < points.size(); j++)
                if (stop_mat[i][j] > 0)
                    painter.drawLine(points[i], points[j]);
    }
    if (!start_point.isNull()) {
        pen.setColor(Qt::red);
        pen.setWidth(15);
        pen.setJoinStyle(Qt::RoundJoin);
        pen.setCapStyle(Qt::RoundCap);
        painter.setPen(pen);
        painter.drawPoint(start_point);
    }
    if (!end_point.isNull()) {
        pen.setColor(Qt::blue);
        pen.setWidth(15);
        pen.setJoinStyle(Qt::RoundJoin);
        pen.setCapStyle(Qt::RoundCap);
        painter.setPen(pen);
        painter.drawPoint(end_point);
    }
    if (min_cvt_line_state) {
        pen.setColor(Qt::yellow);
        pen.setWidth(5);
        painter.setPen(pen);
        std::vector<int> pt_series = min_cvt_line;
        for (int i = 1; i < pt_series.size(); i++)
            painter.drawLine(points[pt_series[i - 1]], points[pt_series[i]]);
    }
    if (min_dist_line_state) {
        pen.setColor(Qt::yellow);
        pen.setWidth(5);
        painter.setPen(pen);
        std::vector<int> pt_series = min_dist_line;
        for (int i = 1; i < pt_series.size(); i++)
            painter.drawLine(points[pt_series[i - 1]], points[pt_series[i]]);
    }
}

bool point_label::eventFilter(QObject* obj, QEvent* event)
{
    // 获取光标位置
    QCursor* cursor;
    int x = cursor->pos().x() - 100;
    int y = cursor->pos().y() - 100;
    for (int i = 0; i < points_area.size(); i++)
        if (points_area[i] == obj)
            if (event->type() == QEvent::Enter)
                point_msg->disp_dialog(x, y-50, points[i]);
    return QObject::eventFilter(obj, event);
}

void point_label::gen_point_area(){
    QLabel* cur_area;
    for (int i = 0; i < points.size(); i++) {
        cur_area = new QLabel(this);
        //cur_area->setAttribute(Qt::WA_Hover, true);
        cur_area->installEventFilter(this);
        cur_area->move(points[i].x() - 5, points[i].y() - 5);
        cur_area->setGeometry(points[i].x() - 7, points[i].y() - 7, 15, 15);
        cur_area->setStyleSheet("QLabel{border:2px solid rgb(0, 0, 200);}");
        cur_area->show();
        points_area.push_back(cur_area);
    }
}

void point_label::store_lines(std::vector<std::vector<int>> stop_mat, int line_num, std::vector<std::vector<int>> line_of_points) {
    this->stop_mat = stop_mat;
    this->line_num = line_num;
    line_state = true;
    this->line_of_points = line_of_points;
    update();
}

void point_label::store_points(QList<QPoint> points) {
    this->points = points;
    emit gen_point_area_sig();
    points_state = true;
    update();
}

void point_label::store_min_cvt_line(std::vector<CompdLine> min_cvt_line) {
    min_cvt_line_state = false;
    min_dist_line_state = false;
    this->min_cvt_line = min_cvt_line[0].stop_seq;  // 还未实现显示多种方案的效果
    min_cvt_line_state = true;  // 最终保持在最后一种方案
    update();
}

void point_label::store_min_dist_line(CompdLine min_dist_line) {
    // 调用前先擦除所有以前的痕迹
    min_cvt_line_state = false;
    min_dist_line_state = false;
    this->min_dist_line = min_dist_line.stop_seq;
    min_dist_line_state = true;  // 一直保持直到下一次选点
    update();
}