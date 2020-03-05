#ifndef LITTLETHINGS_H
#define LITTLETHINGS_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFramebufferObject>
#include <QVector4D>

#include "myshader.h"

/*
  这个类存储了一些 小玩意，为这些单独小玩意单独建类又好麻烦
  目前这些小玩意有：
  1.cube立方体
  2.plane平面
  3.Coordinate坐标系
  4.uniform buffer object 主要保证着色器中view与projection矩阵的唯一性，不再重复指定，
  5.Quad四边形类
  6.Shadow 阴影制造类，这里有两种生成阴影贴图的方法，一种是使用QOpenGLFrameBufferObejct，Qt自带的帧缓冲，一种是core核心机制自带的帧缓冲，各有优劣
*/


/************* 1. Cube立方体 ******************/

class Cube{
public:
  Cube();
  ~Cube();
  void init();
  void draw(GLboolean isTexture = GL_FALSE,GLboolean isNormal = GL_FALSE);
private:
  QOpenGLFunctions_3_3_Core *core;
  GLuint VBO;
};

/************* 2. plane二维平面 ******************/

class Plane{
public:
  Plane();
  ~Plane();
  void init();
  void draw(GLboolean isTexture = GL_FALSE,GLboolean isNormal = GL_FALSE);
private:
  QOpenGLFunctions_3_3_Core *core;
  GLuint VBO;
};

/************ 3. 坐标类，一个简单的显示xyz坐标的类 **************/
class Coordinate{
public:
  Coordinate();
  ~Coordinate();
  void init();
  void draw(QMatrix4x4 model, QMatrix4x4 view, QMatrix4x4 projection);
private:
  QOpenGLFunctions_3_3_Core *core;
  GLuint VBO;
  MyShader* coord_shader_program;

};

/************ 4. ubo类，一个uniform buffer object类 **************/
class UBufferObject{
public:
  UBufferObject();
  ~UBufferObject();
  void init(int ubo_size);
  void bindBufferRange(int bindingIndex, int offset, int size);
  void subDataMat4(int offset, GLvoid* data);
private:
  QOpenGLFunctions_3_3_Core *core;
  GLuint UBO;
};

/************ 5. 四边形类，二维平面类**************/
class Quad{
public:
  Quad();
  ~Quad();
  void init();
  void draw(GLboolean isTexture = GL_FALSE);
private:
  QOpenGLFunctions_3_3_Core *core;
  GLuint VBO;
};

/************ 6. Shadow阴影类 **************/
class Shadow{
public:
  Shadow();
  ~Shadow();
  void initQFBO();
  void bindQFBO();
  void releaseQFBO();//记得用完后调节 glviewport()!
  GLuint getDepthMapQFBO(); //返回计算得到的深度图

  void initFBO();//四个函数是一组操作
  void bindFBO();
  void releaseFBO();
  GLuint getDepthMapFBO();
private:
  QOpenGLFunctions_3_3_Core *core;
  QOpenGLFramebufferObject *qFBO;

  GLuint depthMap;//core自带的帧缓冲机制
  GLuint depthMapFBO;

  GLuint SHADOW_WIDTH;  //深度贴图的尺寸
  GLuint SHADOW_HEIGHT;
};


#endif // LITTLETHINGS_H
