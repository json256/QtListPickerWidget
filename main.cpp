#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    QApplication::setFont(QFont("微软雅黑", 28), "QWidget");// QWidget's subclass
    a.setStyleSheet("MQListPicker > QLabel { font : 28pt 微软雅黑 }");// MQListPicker's direct children

    Widget w;
    w.setGeometry(100, 100, 600, 800);
//    w.setFont(QFont("微软雅黑", 28));
//    QCoreApplication::setAttribute(Qt::AA_UseStyleSheetPropagationInWidgetStyles, true);

//    w.setStyleSheet("QWidget { background-color : black }");
    w.setStyleSheet("Widget { background-color : black }");
    w.show();
    return a.exec();
}
