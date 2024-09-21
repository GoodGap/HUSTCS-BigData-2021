#pragma execution_character_set("utf-8")

#include "Project5.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{ 
    QApplication a(argc, argv);
    Project5 w;
    w.show();
    return a.exec();
}
