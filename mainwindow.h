#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "xopenglwidget.h"
#include <QTimer>
#include <QFileDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void keyPressEvent(QKeyEvent *event);   //键盘按下事件
    void keyReleaseEvent(QKeyEvent *event);  //键盘释放事件
private:
    Ui::MainWindow *ui;
    XOpenGLWidget *oglManager;
private slots:
    void updateOGL(){
      oglManager->update();
    }
    void on_pushButton3_clicked();
};

#endif // MAINWINDOW_H
