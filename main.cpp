/*
 *2019年10月19日
 * 弱智QT版画笔软件设计完成
*/
#include "mainwindow.h"
#include <QApplication>
#include <QStringLiteral>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont font(QStringLiteral("黑体"),20);  //全局字体模块设定
    a.setFont(font); //设置字体
    MainWindow w;
    //窗口标题设置setWindowTitle()
    w.setWindowTitle(QStringLiteral("弱智画笔工具")); //QStringLiteral()解决中文乱码问题
    w.setWindowIcon(QIcon(" myicon.ico"));  //设置icon
    w.show();
    return a.exec(); //运行程序
}
