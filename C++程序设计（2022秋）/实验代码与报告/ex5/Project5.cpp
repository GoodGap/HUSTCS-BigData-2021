#include "Project5.h"
#include "file_widget.h"
#include "point_label.h"
#include <iostream>
#include <QPainter>
#include <QPen>

Project5::Project5(QWidget *parent)
    : QMainWindow(parent), line_num(0)
{
    ui.setupUi(this);

    // ��ʼ����ģ��
    file_ui = new file_widget();
    search_ui = new search_widget();
    graph_label = new point_label(this);
    graph_label->move(12, 50);
    graph_label->resize(1020, 700);
    graph_label->setStyleSheet("QLabel{border:2px solid rgb(200, 0, 0);}");

    // ��ͼģ��Ͷ�ȡ��ͼģ��֮���ͨ�š�һ�������ݾͻ�ͼ�����پ�����ǰ��
    connect(this->file_ui, &file_widget::true_stop_file, this->graph_label, &point_label::store_points);
    connect(this->file_ui, &file_widget::true_line_file, this->graph_label, &point_label::store_lines);
    
    // ����ģ��͵�ǰģ���ͨ�ţ����ڴ���������Ϣ
    connect(this->search_ui, &search_widget::send_search_pos, this, &Project5::recv_search_pos);

    // ��ǰģ��ͻ�ͼģ���ͨ��
    // ��ǰģ�����LineMat����������·��Ȼ���͸���ͼģ��
    connect(this, &Project5::push_min_cvt_line, this->graph_label, &point_label::store_min_cvt_line);
    connect(this, &Project5::push_min_dist_line, this->graph_label, &point_label::store_min_dist_line);
}

Project5::~Project5(){
    delete file_ui;
    delete search_ui;
    delete graph_label;
}

void Project5::load_map()
{
    file_ui->show();
}

void Project5::close_window()
{
    this->close();
}

void Project5::find_nearest() {
    if (start_point.isNull() || end_point.isNull()) throw "undefined start and stop";
    long long max_len_start = INT32_MAX;
    long long max_len_end = INT32_MAX;
    long long start_dist, end_dist;
    for (int i = 0; i < points.size(); i++) {
        start_dist = pow(start_point.x() - points[i].x(), 2) + \
            pow(start_point.y() - points[i].y(), 2);
        end_dist = pow(end_point.x() - points[i].x(), 2) + \
            pow(end_point.y() - points[i].y(), 2);
        if (start_dist < max_len_start) {
            max_len_start = start_dist;
            start_stop = i;
        }
        if (end_dist < max_len_end) {
            max_len_end = end_dist;
            end_stop = i;
        }
    }
}

void Project5::min_convert()
{
    // �����Ϣ��׼��
    line_of_points = this->file_ui->get_line_of_points();
    line_num = this->file_ui->get_line_num();
    points = this->file_ui->get_points();
    line_info = this->file_ui->get_line_info();

    // ˢ�������յ�
    find_nearest();

    // ׼�����ݵĽṹ��
    GraphInfo graph_info = {
        start_stop, end_stop,
        line_num,
        NONE,
        line_of_points,
        line_info,
        points
    };

    // �����㷨
    if (start_stop == end_stop)
        std::cout << "�����յ���ͬ" << std::endl;
    else {
        CalMinCvtLine solution(graph_info, 3);
        solution.gen_convert_mat(graph_info);
        std::vector<CompdLine> final_line = solution.min_convert_line(graph_info);
        emit push_min_cvt_line(final_line);
    }
}

void Project5::min_dist()
{
    line_of_points = this->file_ui->get_line_of_points();
    line_num = this->file_ui->get_line_num();
    points = this->file_ui->get_points();
    line_info = this->file_ui->get_line_info();

    // ˢ�������յ�
    find_nearest();
    // ׼�����ݵĽṹ��
    GraphInfo graph_info = {
        start_stop, end_stop,
        line_num,
        NONE,
        line_of_points,
        line_info,
        points
    };

    // �����㷨
    if (start_stop == end_stop)
        std::cout << "start_stop equal end_stop" << std::endl;
    else {
        CalMinDistLine solution(graph_info, 10);
        solution.gen_convert_mat(graph_info);
        CompdLine final_line = solution.min_dist_line(graph_info);
        emit push_min_dist_line(final_line);
    }
}

void Project5::recv_search_pos(int x, int y, int kind) {
    pos_x = x;
    pos_y = y;
    if (kind == 0) {
        start_point = QPoint(pos_x - 12, pos_y - 50);
        graph_label->draw_start_point(start_point);
    }
    else {
        end_point = QPoint(pos_x - 12, pos_y - 50);
        graph_label->draw_end_point(end_point);
    }
}

void Project5::search_stop() {
    search_ui->show();
}

void Project5::mousePressEvent(QMouseEvent* e){
    if (e->button() == Qt::LeftButton) {
        start_point = QPoint(e->x() - 12, e->y() - 50);
        graph_label->draw_start_point(start_point);
    }
    else if (e->button() == Qt::RightButton) {
        end_point = QPoint(e->x() - 12, e->y() - 50);
        graph_label->draw_end_point(end_point);
    }
}
