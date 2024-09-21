#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Project5.h"
#include "file_widget.h"
#include "search_widget.h"
#include "point_label.h"
#include "LineMat.h"

class Project5 : public QMainWindow
{
    Q_OBJECT

public:
    Project5(QWidget *parent = nullptr);
    ~Project5();

private:
    void mousePressEvent(QMouseEvent* e);

    // ���������յ��Ӧ�����վ
    void find_nearest();

private:
    // qt-designer���ǰ��������
    Ui::Project5Class ui;

    // ��ͼ��ȡģ��
    file_widget* file_ui;

    // վ��ģ������ģ��
    search_widget* search_ui;

    // ��ͼģ��
    point_label* graph_label;

    // �洢վ��,������0��ʼ
    QList<QPoint> points;

    // �洢·������
    int line_num;

    // �洢ÿ�������ڵ�·�ߣ�������0��ʼ
    std::vector<std::vector<int>> line_of_points;

    // �洢ÿ��·���ϵĵ㣬������0��ʼ
    std::vector<std::vector<int>> line_info;

    // ���
    QPoint start_point;

    // �յ�
    QPoint end_point;

    // ������վ
    int start_stop;  // 0-38

    // ����յ�վ
    int end_stop;  // 0-38

    // ͨ��ģ��ƥ���ѯվ��λ����Ϣ
    int pos_x = -1, pos_y = -1;

signals:
    // ����min_cvt_line����ͼģ��
    void push_min_cvt_line(std::vector<CompdLine> min_cvt_line);
    
    // ����min_dist_line����ͼģ��
    void push_min_dist_line(CompdLine min_dist_line);

private slots:
    // ���ص�ͼ
    void load_map();

    // �رմ���
    void close_window();

    // ������Сת��
    void min_convert();

    // ������̾���
    void min_dist();

    // ��ʾ����վ����ҽ���
    void search_stop();

    // ����վ���ѯ��λ����Ϣ,kind=0Ϊ��㣬1Ϊ�յ�
    void recv_search_pos(int x, int y, int kind);
};