#include "mainwindow.h"

#include <QApplication>
#include "flowchar_widget.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

    //有个bug,连接线移动中间的节点，不停的闪或者超过边界了
    //内存泄漏
    //视图放大时拖动图元有重影
    FlowCharWidget flowchar_widget;
    flowchar_widget.show();
    return a.exec();
}
