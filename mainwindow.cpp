#include "mainwindow.h"
#include "paintwidget.h"
#include <QAction>
#include <QActionGroup>
#include <QToolBar>
#include <QStringLiteral>  //中文乱码处理
#include <QMessageBox>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) : //主窗口
    QMainWindow(parent)
{
    QToolBar *bar = this->addToolBar("Tools");//工具栏
    QActionGroup *group = new QActionGroup(bar);//动作组，触发后之前的按钮失效

    //打开
    QAction *open= new QAction("Open", bar);
    open->setIcon(QIcon(":/new/prefix1/images/open.png")); //图标的文档地址
    open->setStatusTip(tr("open a new file"));  //提示
    open->setToolTip(tr("Open"));  //下方长条显示功能提示
    open->setShortcut(QKeySequence(tr("Ctrl+O"))); //热键
    group->addAction(open);
    bar->addAction(open);

    //保存
    QAction *save= new QAction("Save", bar);
    save->setIcon(QIcon(":/new/prefix1/images/save.png"));
    save->setStatusTip(tr("save a file"));
    save->setToolTip(tr("Save"));
    save->setShortcut(QKeySequence(tr("Ctrl+S")));
    group->addAction(save);
    bar->addAction(save);

    //画线
    QAction *drawLineAction = new QAction("Line", bar);
    drawLineAction->setIcon(QIcon(":/new/prefix1/images/line.png"));
    drawLineAction->setToolTip(tr("Line"));
    drawLineAction->setStatusTip(tr("Draw a line."));
    drawLineAction->setCheckable(true);
    drawLineAction->setChecked(true);//因为在paintwidget构造函数里面默认为线类型，所以这里默认为触发了画线动作
    group->addAction(drawLineAction);
    bar->addAction(drawLineAction);

    //画矩形
    QAction *drawRectAction = new QAction("Rectangle", bar);
    drawRectAction->setIcon(QIcon(":/new/prefix1/images/rect.png"));
    drawRectAction->setToolTip(tr("Rectangle"));
    drawRectAction->setStatusTip(tr("Draw a rectangle."));
    drawRectAction->setCheckable(true);
    group->addAction(drawRectAction);
    bar->addAction(drawRectAction);

    //画椭圆形
    QAction *drawEllipseAction = new QAction("Ellipse",bar);
    drawEllipseAction->setIcon(QIcon(":/new/prefix1/images/ellipse.png"));
    drawEllipseAction->setToolTip(tr("Ellipse"));
    drawEllipseAction->setStatusTip(tr("Draw a ellipse."));
    drawEllipseAction->setCheckable(true);
    group->addAction(drawEllipseAction);
    bar->addAction(drawEllipseAction);

    //画笔工具
    QAction *drawPenAction = new QAction("Pen",bar);
    drawPenAction->setIcon(QIcon(":/new/prefix1/images/lines.png"));
    drawPenAction->setText(tr("Pen"));
    drawPenAction->setStatusTip(tr("Draw a penline."));
    drawPenAction->setCheckable(true);
    group->addAction(drawPenAction);
    bar->addAction(drawPenAction);

    //橡皮擦
    QAction *eraseAction = new QAction("Erase",bar);
    eraseAction->setIcon(QIcon(":/new/prefix1/images/erase.png"));
    eraseAction->setStatusTip(tr("erase everything."));
    eraseAction->setText(tr("Erase"));
    save->setToolTip(tr("Erase"));
    group->addAction(eraseAction);
    bar->addAction(eraseAction);

    //调色板
    colorBtn =new QToolButton; //设置按钮的格式
    QPixmap pixmap(32,32);
    pixmap.fill(Qt::black); //初始样式上的默认黑色图标
    colorBtn->setIcon(QIcon(pixmap)); //把默认图标画上去
    colorBtn->setStatusTip(tr("change pen color"));
    colorBtn->setText(tr("Paintdeck"));
    bar->addWidget(colorBtn);//添加到工具栏

    //标签
    spinLabel =new QLabel(QStringLiteral("线宽:"));
    bar->addWidget(spinLabel);

    //笔宽设置，范围0-100
    widthSpinBox = new QSpinBox; //数值调整菜单
    widthSpinBox->setValue(1);
    widthSpinBox->resize(16,16); //调整不了大小
    widthSpinBox->setStatusTip(tr("change pen width"));
    widthSpinBox->setMaximum(100);
    widthSpinBox->setMinimum(0);
    bar->addWidget(widthSpinBox);

    //标签2
    styleLabel = new QLabel(QStringLiteral("画笔样式:"));
    bar->addWidget(styleLabel);

    //笔样式设置
    cbox = new QComboBox; //下拉菜单，包括5种内置笔形
    cbox->addItem(tr("SolodLine") , static_cast<int>(Qt::SolidLine));
    cbox->addItem(tr("DashLine") , static_cast<int>(Qt::DashLine));
    cbox->addItem(tr("DotLine") , static_cast<int>(Qt::DotLine));
    cbox->addItem(tr("DashDotLine") , static_cast<int>(Qt::DashDotLine));
    cbox->addItem(tr("DashDotDotLine") , static_cast<int>(Qt::DashDotDotLine));
    //cbox->addItem(tr("DashDotDotLine") , static_cast<int>(Qt::CustomDashLine));
    cbox->setStatusTip(tr("change pen style"));
    bar->addWidget(cbox);

    //一键清除图画
    clearBtn =new QToolButton; //设置按钮的格式
    clearBtn->setStatusTip(tr("clear image"));
    clearBtn->setText(tr("清除"));
    bar->addWidget(clearBtn);//添加到工具栏

    statusBar(); //状态栏，说明信息
    bar->setMovable(false); //固定菜单

    area = new PaintWidget; //主窗口调用widget窗口类
    resize(1440,960);
    scrollArea = new QScrollArea; //滚动条
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(area);
    scrollArea->widget()->setMinimumSize(1440,960);
    setCentralWidget(scrollArea);

    //把菜单部件等连接到窗口上
    connect(open,SIGNAL(triggered()),this,SLOT(OpenTriggered()));

    connect(save,SIGNAL(triggered()),this,SLOT(SaveTriggered()));

    connect(drawLineAction,SIGNAL(triggered()),this,SLOT(drawLineActionTriggered()));

    connect(drawRectAction,SIGNAL(triggered()),this,SLOT(drawRectActionTriggered()));

    connect(drawEllipseAction,SIGNAL(triggered()),this,SLOT(drawEllipseActionTriggered()));

    connect(drawPenAction,SIGNAL(triggered()),this,SLOT(drawPenActionTriggered()));

    connect(colorBtn,SIGNAL(clicked(bool)),this,SLOT(showColor()));

    connect(eraseAction,SIGNAL(triggered()),this,SLOT(EraseTriggered()));

    connect(widthSpinBox,SIGNAL(valueChanged(int)),this,SLOT(setWidth(int)));

    connect(cbox,SIGNAL(activated(int)),this,SLOT(showStyle()));

    connect(clearBtn,SIGNAL(clicked(bool)),this,SLOT(ClearTriggered()));

    connect(this,SIGNAL(drawShape(PaintWidget::shape)),//发送触发的信号给paintwidget
            area,SLOT(setShape(PaintWidget::shape)));

    //鼠标
    this->setMouseTracking(true);
    bar->addWidget(this->area->mousexy);
}

void MainWindow::showStyle()
{
    area->setStyle(cbox->itemData(cbox->currentIndex(),Qt::UserRole).toInt());
}

void MainWindow::drawLineActionTriggered()
{
    emit drawShape(PaintWidget::Line);//--------------发射信号
}

void MainWindow::drawRectActionTriggered()
{
    emit drawShape(PaintWidget::Rect);
}

void MainWindow::drawEllipseActionTriggered()
{
    emit drawShape(PaintWidget::Ellipse);
}
 void MainWindow::drawPenActionTriggered()
{
    emit drawShape(PaintWidget::Pen);
}
void MainWindow::OpenTriggered()
{
    this->area->openI();
}
void MainWindow::SaveTriggered()
{
    this->area->saveI();
}
void MainWindow::EraseTriggered()
{
    emit drawShape((PaintWidget::Erase));
}

void MainWindow::ClearTriggered()
{
    this->area->Clear();
}
void MainWindow::showColor() //颜色选择
{
    QColor color = QColorDialog::getColor(static_cast<int>(Qt::black) ,this, QStringLiteral("颜色选择框"));
    if(color.isValid())
    {
        area->setColor(color);
        QPixmap p(30,30);
        p.fill(color);
        colorBtn->setIcon(QIcon(p));
    }
}

void MainWindow::setWidth(int w) //设定笔宽
{
    area->weight=w;
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    int button=QMessageBox::question(this,QStringLiteral("退出程序"),QStringLiteral("确认退出程序"),QMessageBox::Yes | QMessageBox::No);
    if(button ==QMessageBox::Yes)
        event->accept();
    else
        event->ignore();
}
MainWindow::~MainWindow()
{

}
