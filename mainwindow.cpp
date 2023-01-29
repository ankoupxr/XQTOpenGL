#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
  ui->setupUi(this); //如果ui和oglmanager的生成顺序相反，会失去manager的鼠标焦点，鼠标操作无效化
  oglManager = new XOpenGLWidget(this);

  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &MainWindow::updateOGL);
  timer->start(10);

  /******** 设置背景颜色 ***********/
  QPalette pal(this->palette());

  pal.setColor(QPalette::Background, QColor(99, 103, 106));
  this->setAutoFillBackground(true);
  this->setPalette(pal);
}

MainWindow::~MainWindow()
{
  delete ui;
}
void MainWindow::keyPressEvent(QKeyEvent *event){
  oglManager->handleKeyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event){
  oglManager->handleKeyReleaseEvent(event);
}


void MainWindow::on_pushButton3_clicked(){
  QString fileName = QFileDialog::getOpenFileName(this, tr("Select File"), ".");
  qDebug() << fileName;
  oglManager->changeObjModel(fileName);
}




