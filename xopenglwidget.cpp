#include "xopenglwidget.h"
#include <QPalette>
#include "model.h"
#include "light.h"
#include "resourcemanager.h"

const QVector3D CAMERA_POSITION(0.0f, 0.0f, 3.0f);
const QVector3D LIGHT_POSITION(0.0f, 3.0f, 3.0f);

const int XOpenGLWidget_WIDTH = 815;
const int XOpenGLWidget_HEIGHT = 500;

Light *light;
Model *objModel;
SkyBox *skyBox;

XOpenGLWidget::XOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent){
  this->setGeometry(20,40, XOpenGLWidget_WIDTH, XOpenGLWidget_HEIGHT);
}

XOpenGLWidget::~XOpenGLWidget(){
  if(camera)
    delete camera;

}

void XOpenGLWidget::handleKeyPressEvent(QKeyEvent *event){
  GLuint key = event->key();
  if(key >= 0 && key <= 1024)
    this->keys[key] = GL_TRUE;

}

void XOpenGLWidget::handleKeyReleaseEvent(QKeyEvent *event){
  GLuint key = event->key();
  if(key >= 0 && key <= 1024)
      this->keys[key] = GL_FALSE;
}

void XOpenGLWidget::changeObjModel(QString fileName){
  objModel->init(fileName);
}

void XOpenGLWidget::initializeGL(){
  /*********** OGL核心 ***********/
  core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
  core->glEnable(GL_DEPTH_TEST);

  /*********** 初始化模型细节参数  *************/
  isOpenLighting = GL_TRUE;
  isLineMode = GL_FALSE;
  modelScaling = 0.001f;

  /*********** 键鼠响应及时间帧数操作  *************/
  for(GLuint i = 0; i != 1024; ++i) //初始化键盘按键
    keys[i] = GL_FALSE;

  deltaTime = 0.0f;
  lastFrame = 0.0f;

  isFirstMouse = GL_TRUE;
  isLeftMousePress = GL_FALSE;
  lastX = width() / 2.0f;
  lastY = height() / 2.0f;

  time.start();

  /************ 摄像机 ***********/
  camera = new Camera(CAMERA_POSITION);

  /*********** 灯  *************/
  light = new Light();
  light->init();

  /************ obj模型 ***********/
  objModel = new Model();
  objModel->init(":/res/Model/huapen/penzi.obj");

   /************ 载入shader ***********/
  ResourceManager::loadShader("model", ":/res/shader/model.vert", ":/res/shader/model.frag");
  ResourceManager::loadShader("light", ":/res/shader/light.vert", ":/res/shader/light.frag");

  ResourceManager::getShader("model").use().setInteger("material.ambientMap", 0);
  ResourceManager::getShader("model").use().setInteger("material.diffuseMap", 1);
  ResourceManager::getShader("model").use().setVector3f("light.position", LIGHT_POSITION);

  QMatrix4x4 model;
  model.translate(LIGHT_POSITION);
  model.scale(0.1f);
  ResourceManager::getShader("light").use().setMatrix4f("model", model);

  /************ 天空盒 ***********/
  skyBox = new SkyBox();

  /************ 背景颜色参数调控 ***********/
  core->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  core->glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
}

void XOpenGLWidget::resizeGL(int w, int h){
  core->glViewport(0, 0, w, h);
}

void XOpenGLWidget::paintGL(){
  /*********** 计算两次帧数之间的时间间隔  ***************/
  GLfloat currentFrame = (GLfloat)time.elapsed()/100;
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  this->processInput(deltaTime);
  this->updateGL();

  /*********  绘制灯 ************/
  ResourceManager::getShader("light").use();
  light->drawLight();

  /*********  绘制花盆 ************/
  ResourceManager::getShader("model").use();
  objModel->draw(this->isOpenLighting);


}

void XOpenGLWidget::processInput(GLfloat dt){
  if (keys[Qt::Key_W])
    camera->processKeyboard(FORWARD, dt);
  if (keys[Qt::Key_S])
    camera->processKeyboard(BACKWARD, dt);
  if (keys[Qt::Key_A])
    camera->processKeyboard(LEFT, dt);
  if (keys[Qt::Key_D])
    camera->processKeyboard(RIGHT, dt);
  if (keys[Qt::Key_E])
    camera->processKeyboard(UP, dt);
  if (keys[Qt::Key_Q])
    camera->processKeyboard(DOWN, dt);

}

void XOpenGLWidget::updateGL(){
  if(this->isLineMode)
    core->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    core->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  QMatrix4x4 projection, view;
  projection.perspective(camera->zoom, (GLfloat)width()/(GLfloat)height(), 0.1f, 200.f);
  view = camera->getViewMatrix();

  ResourceManager::getShader("light").use().setMatrix4f("projection", projection);
  ResourceManager::getShader("light").use().setMatrix4f("view", camera->getViewMatrix());

  ResourceManager::getShader("model").use().setMatrix4f("projection", projection);
  ResourceManager::getShader("model").use().setMatrix4f("view", camera->getViewMatrix());
  ResourceManager::getShader("model").use().setVector3f("viewPos", camera->position);


  skyBox->render(view, projection);

  QMatrix4x4 scaling;
  scaling.scale(modelScaling);
  ResourceManager::getShader("model").use().setMatrix4f("model", scaling);
}

void XOpenGLWidget::mouseMoveEvent(QMouseEvent *event){
  GLint xpos = event->pos().x();
  GLint ypos = event->pos().y();
  if(isLeftMousePress){
    if (isFirstMouse){
      lastX = xpos;
      lastY = ypos;
      isFirstMouse = GL_FALSE;
    }

    GLint xoffset = xpos - lastX;
    GLint yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;
    camera->processMouseMovement(xoffset, yoffset);

  }
}

void XOpenGLWidget::mousePressEvent(QMouseEvent *event){
  if(event->button() == Qt::LeftButton)//注意是button（）不是buttons（）；
    isLeftMousePress = GL_TRUE;
}

void XOpenGLWidget::mouseReleaseEvent(QMouseEvent *event){
  if(event->button() == Qt::LeftButton){ //注意是button（）不是buttons（）；
    isLeftMousePress = GL_FALSE;
    isFirstMouse = GL_TRUE;
  }
}

void XOpenGLWidget::wheelEvent(QWheelEvent *event){
  QPoint offset = event->angleDelta();
  camera->processMouseScroll(offset.y()/20.0f);
}
