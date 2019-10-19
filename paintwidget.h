#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QPoint>
#include <QImage>
#include <QtDebug>
#include <QColorDialog>
#include <QToolButton>
#include <QLabel>

class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(QWidget *parent = nullptr);
    enum shape
    {
        Line=1,
        Rect,
        Ellipse,
        Pen,
        Erase,
    };
    int weight;
    int style;
    QLabel *mousexy;

public  slots:
    void setShape(PaintWidget::shape t)//接受从mainwindow发射过来的信号
    {
        if(t!=type)
            type = t;
        qDebug()<<type;//检查是否接收成功
    }
    void setColor(QColor c);
    void setStyle(int);
    void openI();
    void saveI();
    void Clear();
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void paint(QImage &theImage);
signals:
private:
    PaintWidget::shape type;
    QImage image;
    QImage tempImage;
    QRgb backColor;
    QPoint lastPoint;
    QPoint endPoint;
    bool isDrawing;
    QColor Color;
};

#endif // PAINTWIDGET_H
