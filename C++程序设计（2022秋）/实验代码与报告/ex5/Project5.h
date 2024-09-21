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

    // 查找起点和终点对应的最近站
    void find_nearest();

private:
    // qt-designer设计前端主界面
    Ui::Project5Class ui;

    // 地图读取模块
    file_widget* file_ui;

    // 站点模糊搜索模块
    search_widget* search_ui;

    // 画图模块
    point_label* graph_label;

    // 存储站点,索引从0开始
    QList<QPoint> points;

    // 存储路线总数
    int line_num;

    // 存储每个点所在的路线，索引从0开始
    std::vector<std::vector<int>> line_of_points;

    // 存储每条路线上的点，索引从0开始
    std::vector<std::vector<int>> line_info;

    // 起点
    QPoint start_point;

    // 终点
    QPoint end_point;

    // 最近起点站
    int start_stop;  // 0-38

    // 最近终点站
    int end_stop;  // 0-38

    // 通过模糊匹配查询站点位置信息
    int pos_x = -1, pos_y = -1;

signals:
    // 发送min_cvt_line给画图模块
    void push_min_cvt_line(std::vector<CompdLine> min_cvt_line);
    
    // 发送min_dist_line给画图模块
    void push_min_dist_line(CompdLine min_dist_line);

private slots:
    // 加载地图
    void load_map();

    // 关闭窗口
    void close_window();

    // 计算最小转乘
    void min_convert();

    // 计算最短距离
    void min_dist();

    // 显示中文站点查找界面
    void search_stop();

    // 接受站点查询的位置信息,kind=0为起点，1为终点
    void recv_search_pos(int x, int y, int kind);
};