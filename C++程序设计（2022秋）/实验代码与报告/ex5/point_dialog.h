#pragma once

#include <QDialog>
#include "ui_point_dialog.h"

class point_dialog : public QDialog
{
	Q_OBJECT

public:
	point_dialog(QWidget *parent = nullptr);

	// 没有指针对象，自动析构
	~point_dialog() = default;  

	// 展示对话框
	void disp_dialog(int x, int y, QPoint point);

private:
	Ui::Coordinate ui;
};