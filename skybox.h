#ifndef SKYBOX_H
#define SKYBOX_H

#include <QOpenGLFunctions_3_3_Core>

struct SkyBoxData;
class SkyBox
{
public:
    SkyBox();
    ~SkyBox();

    void render(const QMatrix4x4 &viewMatrix, const QMatrix4x4 &projectionMatrix);

private:
    void init();

private:
      QOpenGLFunctions_3_3_Core *core;
    SkyBoxData *d;
};

#endif // SKYBOX_H
