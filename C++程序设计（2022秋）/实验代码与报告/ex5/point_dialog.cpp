#include "point_dialog.h"
#include <qtimer.h>

point_dialog::point_dialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

void point_dialog::disp_dialog(int x, int y, QPoint point){
	this->move(x, y);
	std::string msg = std::to_string(x) + "," + std::to_string(y);
	ui.StopMsg->setText(QString::fromStdString(msg));
	this->show();
	if (this)
		QTimer::singleShot(1500, this, SLOT(close()));
}