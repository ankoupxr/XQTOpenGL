#ifndef XOPENGLWIDGET_H
#define XOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QKeyEvent>
#include <QOpenGLFunctions_3_3_Core>
#include <QTime>
#include "camera.h"
#include "skybox.h"
#include <QMouseEvent>
#include <QDebug>


#include <model.h>

class XOpenGLWidget : public QOpenGLWidget
{
public:
    explicit XOpenGLWidget(QWidget *parent = nullptr);
    ~XOpenGLWidget();
    void handleKeyPressEvent(QKeyEvent *event);   //键盘按下事件
    void handleKeyReleaseEvent(QKeyEvent *event);  //键盘释放事件
    void changeObjModel(QString fileName);

    GLfloat modelScaling;// obj模型缩放比例
    GLboolean keys[1024];//获取键盘按键，实现多键触控
    GLboolean isOpenLighting;
    GLboolean isLineMode;
protected:
  void mouseMoveEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event) override;    //滚轮事件
  void mousePressEvent(QMouseEvent *event) override;  //鼠标按下事件
  void mouseReleaseEvent(QMouseEvent *event) override; //鼠标释放事件
  virtual void initializeGL();
  virtual void resizeGL(int w, int h);
  virtual void paintGL();
private:
    void processInput(GLfloat dt);//摄像机键盘处理函数，
    void updateGL();//opengl 更新函数

    QOpenGLFunctions_3_3_Core *core;

    GLboolean isFirstMouse;
    GLboolean isLeftMousePress;
    GLint lastX;
    GLint lastY;

    QTime time;
    GLfloat deltaTime;
    GLfloat lastFrame;//上一帧

    Camera *camera;
};

#endif // XOPENGLWIDGET_H
