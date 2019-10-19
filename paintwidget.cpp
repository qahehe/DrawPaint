#include "paintwidget.h"
#include  <QPainter>  //画图的一个类
#include <QMouseEvent>
#include <QFileDialog>  //文件弹窗
#include <QColorDialog>  //颜色弹窗
#include <QToolButton>
#include <QLabel>
/*
 * 子窗口
*/
PaintWidget::PaintWidget(QWidget *parent) :
    QWidget(parent)
{
    isDrawing = false;
    type = PaintWidget::Line;//默认为线类型
    image = QImage(1440,960,QImage::Format_RGB32); //初始图像
    backColor = qRgb(255,255,255);
    image.fill(backColor);
    tempImage = image;//tempImage也填充为白色
    weight=1;  //初始笔宽为1
    style=1;
    //setColor(this->Color);
    mousexy=new QLabel;
    mousexy->resize(100,30);
    QFont ss(QStringLiteral("微软雅黑"),20);
    mousexy->setFont(ss); //字体格式
    mousexy->setStyleSheet("color:red");//鼠标坐标颜色
}

void PaintWidget::setStyle(int q)  //设定风格
{
    this->style=q;
}
void PaintWidget::setColor(QColor c) //设定颜色
{
    this->Color=c;
}

void PaintWidget :: paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if(isDrawing == true)
        painter.drawImage(0,0,tempImage);//如果正在绘图，既鼠标点击或者正在移动，画在tempImage上
    else
        painter.drawImage(0,0,image);//如果鼠标释放，将图保存在image上
}

void PaintWidget :: mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        lastPoint = event->pos();
        isDrawing = true;//鼠标点击开始绘图，移动表示正在绘图
    }
}

void PaintWidget :: mouseMoveEvent(QMouseEvent *event)
{
    QString pStr("%1,%2");
    pStr = pStr.arg(event->pos().x()).arg(event->pos().y());
    mousexy->setText(pStr);
    if(event->buttons() & Qt::LeftButton)
    {//鼠标为左键且正在移动
        endPoint = event->pos();
        tempImage = image;
        if(type == Pen || type==Erase)//如果工具为画笔，不用双缓冲直接画在画板上
            paint(image);
        else //否则用双缓冲绘图
            paint(tempImage);
    }
}
void PaintWidget :: mouseReleaseEvent(QMouseEvent *) //释放鼠标
{
     isDrawing = false;
     if(type != Pen)
        paint(image);
}

//打开操作
void PaintWidget::openI()
{
    QPainter p(&image);
    QString picPath = QFileDialog::getOpenFileName(this,tr("打开"),"","Image Files(*.jpg *.png)");
    //弹出文件打开对话框
    if(!picPath.isEmpty())//用户选择了文件
    {
        QPixmap pixmap;
        pixmap.load(picPath);//加载图片
        p.drawPixmap(0,32,pixmap);//添加工具栏的空间
    }
    update();//重绘
}

//保存操作
void PaintWidget::saveI()
{
    QPainter p(&image);
    //弹出文件保存对话框
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存"), "new.jpg", "Image (*.jpg *.png *.bmp)");
    if (fileName.length()>0)
    {
        QPixmap pixmap(size());//新建窗体大小的pixmap
        QPainter painter(&pixmap);//将pixmap作为画布
        painter.fillRect(QRect(0, 0, size().width(), size().height()), Qt::white);//设置绘画区域、画布颜色
        this->render(&painter);//将窗体渲染到painter，再由painter画到画布
        pixmap.copy(QRect(0,32,size().width(),size().height()-32)).save(fileName);//不包含工具栏
    }
    update(); //重绘
}

//清除操作
void PaintWidget::Clear()
{
    QPainter p(&image);
    QPixmap pixmap(size());
    pixmap.fill(Qt::white);
    p.drawPixmap(0,32,pixmap);
    update(); //强制重绘
}


void PaintWidget::paint(QImage &theImage)
{
    QPainter p(&theImage);//引用图片
    QPen apen;
    apen.setWidth(weight); //笔宽
    apen.setStyle((Qt::PenStyle)style); //样式选择
    apen.setColor(this->Color); //颜色选择
    p.setPen(apen);//-----------设置绘图工具画笔线条初始宽度为1
    switch (type)
    {
        case  PaintWidget::Line:
        {
            p.drawLine(lastPoint,endPoint);
            break;
        }
        case PaintWidget::Rect:
        {
            p.drawRect(lastPoint.x(),lastPoint.y(),endPoint.x()-lastPoint.x(),endPoint.y()-lastPoint.y());
            break;
        }
        case PaintWidget::Ellipse:
        {
            p.drawEllipse(lastPoint.x(),lastPoint.y(),endPoint.x()-lastPoint.x(),endPoint.y()-lastPoint.y());
            break;
        }
        case PaintWidget::Pen:
        {
            p.drawLine(lastPoint,endPoint);
            lastPoint = endPoint;
            break;
        }

        case PaintWidget::Erase:
        {
            QColor c(255,255,255);
            apen.setColor(c);
            apen.setWidth(20);
            p.setPen(apen);
            p.drawLine(lastPoint,endPoint);
            lastPoint = endPoint;
            break;
        }
        default :
        {
            break;
        }
    }
    update();//重绘
}
