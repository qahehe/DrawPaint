#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "paintwidget.h"
#include <QScrollArea>
#include <QToolButton>
#include <QSpinBox>
#include <QComboBox>
#include <QLabel>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QToolButton *colorBtn;
    QToolButton *clearBtn;
    QSpinBox *widthSpinBox;
    QComboBox *cbox;
    QLabel *styleLabel;
    QLabel *spinLabel;
protected:
signals:
    void drawShape(PaintWidget::shape newShape);
private slots: //工具栏触发事件
    void drawLineActionTriggered();
    void drawRectActionTriggered();
    void drawEllipseActionTriggered();
    void drawPenActionTriggered();
    void OpenTriggered();
    void SaveTriggered();
    void EraseTriggered();
    void ClearTriggered();
private:

    PaintWidget *area;//paintwidget类
    QScrollArea *scrollArea;//滚动条
public slots:
    void showColor();  //颜色转换器
    void setWidth(int);  //画笔笔宽转换标签
    void showStyle();
};
#endif // MAINWINDOW_H

