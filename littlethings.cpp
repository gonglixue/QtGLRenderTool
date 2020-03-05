#include "littlethings.h"


/******************* 1. Cube 立方体 ***********************/
Cube::Cube(): VBO(0){
  core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
}

Cube::~Cube(){
  if(VBO != 0)
    core->glDeleteBuffers(1, &VBO);
}

void Cube::init(){
  float vertices[] = {
    // positions        // textures        // normals
    //Back Face
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f, //bottom-left
     0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  0.0f,  0.0f, -1.0f, //top-right
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, -1.0f,

    //Front Face
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,

    //Left Face
    -0.5f,  0.5f,  0.5f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,

    //Right Face
     0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    //Bottom Face
    -0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f, -1.0f,  0.0f,

    //Top Face
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,
  };
  core->glGenBuffers(1, &VBO);

  core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
  core->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

}

void Cube::draw(GLboolean isTexture, GLboolean isNormal){
  core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
  core->glEnableVertexAttribArray(0);
  core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  if(isTexture){
    core->glEnableVertexAttribArray(1);
    core->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
  }
  if(isNormal){
    core->glEnableVertexAttribArray(2);
    core->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5*sizeof(float)));
  }

  core->glDrawArrays(GL_TRIANGLES, 0, 36);
}

/******************* 2. Plane 立方体 ***********************/
Plane::Plane(): VBO(0){
  core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
}

Plane::~Plane(){
  if(VBO != 0)
    core->glDeleteBuffers(1, &VBO);
}

void Plane::init(){
  float vertices[] = {
    // positions        // textures        // normals
//    -0.5f,  0.0f, -0.5f,   0.0f,  25.0f,  0.0f,  1.0f,  0.0f,
//     0.5f,  0.0f, -0.5f,  25.0f,  25.0f,  0.0f,  1.0f,  0.0f,
//     0.5f,  0.0f,  0.5f,  25.0f,   0.0f,  0.0f,  1.0f,  0.0f,
//     0.5f,  0.0f,  0.5f,  25.0f,   0.0f,  0.0f,  1.0f,  0.0f,
//    -0.5f,  0.0f,  0.5f,   0.0f,   0.0f,  0.0f,  1.0f,  0.0f,
//    -0.5f,  0.0f, -0.5f,   0.0f,  25.0f,  0.0f,  1.0f,  0.0f,
      -0.5f,  0.0f, -0.5f,   0.0f,  25.0f,  0.0f,  1.0f,  0.0f,
      -0.5f,  0.0f,  0.5f,   0.0f,   0.0f,  0.0f,  1.0f,  0.0f,
       0.5f,  0.0f,  0.5f,  25.0f,   0.0f,  0.0f,  1.0f,  0.0f,

       0.5f,  0.0f, -0.5f,  25.0f,  25.0f,  0.0f,  1.0f,  0.0f,
      -0.5f,  0.0f, -0.5f,   0.0f,  25.0f,  0.0f,  1.0f,  0.0f,
       0.5f,  0.0f,  0.5f,  25.0f,   0.0f,  0.0f,  1.0f,  0.0f,
  };
  core->glGenBuffers(1, &VBO);

  core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
  core->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

}

void Plane::draw(GLboolean isTexture, GLboolean isNormal){
  core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
  core->glEnableVertexAttribArray(0);
  core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  if(isTexture){
    core->glEnableVertexAttribArray(1);
    core->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
  }
  if(isNormal){
    core->glEnableVertexAttribArray(2);
    core->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5*sizeof(float)));
  }

  core->glDrawArrays(GL_TRIANGLES, 0, 6);
}

/******************* 3. Coordinate 立方体 ***********************/
Coordinate::Coordinate(): VBO(0){
  core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
  coord_shader_program = new MyShader("F:/Documents/QtProject/QtGLRenderTool/coordinate.vs",
                                      "F:/Documents/QtProject/QtGLRenderTool/coordinate.frag");

}

Coordinate::~Coordinate(){
  if(VBO != 0)
    core->glDeleteBuffers(1, &VBO);
  delete this->coord_shader_program;
  delete core;
}

void Coordinate::init(){

  float vertices[] = {
      // positions
      0.0f, 0.0f, 0.0f,
      10.0f, 0.0f, 0.0f,

      0.0f, 0.0f, 0.0f,
      0.0f, 10.0f, 0.0f,

      0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 10.0f
  };

  core->glGenBuffers(1, &VBO);

  core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
  core->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


}

void Coordinate::draw(QMatrix4x4 model, QMatrix4x4 view, QMatrix4x4 projection){
  coord_shader_program->bind();
  coord_shader_program->setUniformValue("model", model);
  coord_shader_program->setUniformValue("view", view);
  coord_shader_program->setUniformValue("projection", projection);

  core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
  core->glEnableVertexAttribArray(0);
  core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  core->glDrawArrays(GL_LINES, 0, 6);
  coord_shader_program->unbind();
}

/******************* 4. UBufferObject ubo缓冲对象 ***********************/

UBufferObject::UBufferObject(): UBO(0){
  core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
}

UBufferObject::~UBufferObject(){

}

void UBufferObject::init(int ubo_size){
  core->glGenBuffers(1, &UBO);
  core->glBindBuffer(GL_UNIFORM_BUFFER, UBO);
  core->glBufferData(GL_UNIFORM_BUFFER, ubo_size, NULL, GL_STATIC_DRAW);
  core->glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UBufferObject::bindBufferRange(int bindingIndex, int offset, int size){
    core->glBindBufferRange(GL_UNIFORM_BUFFER, bindingIndex, this->UBO, offset, size);
}

void UBufferObject::subDataMat4(int offset, GLvoid *data){
  core->glBindBuffer(GL_UNIFORM_BUFFER, UBO);
  core->glBufferSubData(GL_UNIFORM_BUFFER, offset, 4 * sizeof(QVector4D), data);
  core->glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


/******************* 5. Quad 四边形 ***********************/
Quad::Quad(): VBO(0){
  core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
}

Quad::~Quad(){
  if(VBO != 0)
    core->glDeleteBuffers(1, &VBO);
}


void Quad::init(){
  float vertices[] = {
  // positions    // textures
    -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
     1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,  1.0f, 1.0f,

     1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
    -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
  };
  core->glGenBuffers(1, &VBO);

  core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
  core->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

}

void Quad::draw(GLboolean isTexture){
  core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
  core->glEnableVertexAttribArray(0);
  core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  if(isTexture){
    core->glEnableVertexAttribArray(1);
    core->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3  *sizeof(float)));
  }

  core->glDrawArrays(GL_TRIANGLES, 0, 6);
}

/******************* 6. Shadow 阴影 ************************/
Shadow::Shadow(): qFBO(NULL), depthMap(0), depthMapFBO(0){
  core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
  /*
   * 一个遗留问题，深度图解析度为什么是1024，其余的不行
   * 在OpenGL2.0之前，在不支持非二次幂的纹理（GL_ARB_texture_non_power_of_two）的扩展的情况下，
   * 我们需要调整深度纹理的大小，使其恰好为二次幂。
   * 例如在1024x768的分辨率下，最大的二次幂纹理大小是1024x512.
   */
  SHADOW_WIDTH = 1024;
  SHADOW_HEIGHT = 1024;

}

Shadow::~Shadow(){

}

void Shadow::initQFBO(){
  qFBO = new QOpenGLFramebufferObject(QSize(SHADOW_WIDTH, SHADOW_HEIGHT), QOpenGLFramebufferObject::Depth, GL_TEXTURE_2D);
}

void Shadow::bindQFBO(){
  core->glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
  qFBO->bind();
  core->glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void Shadow::releaseQFBO(){
  qFBO->release();
}

GLuint Shadow::getDepthMapQFBO(){
  return qFBO->texture();
}

void Shadow::initFBO(){
  core->glGenTextures(1, &depthMap);
  core->glBindTexture(GL_TEXTURE_2D, depthMap);

  core->glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
  core->glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

  core->glGenFramebuffers(1, &depthMapFBO);
  core->glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
  core->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
  core->glDrawBuffer(GL_NONE);
  core->glReadBuffer(GL_NONE);
  core->glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Shadow::bindFBO(){
  core->glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT); //绘制深度图
  core->glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
  core->glClear(GL_DEPTH_BUFFER_BIT);
}

void Shadow::releaseFBO(){
  core->glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint Shadow::getDepthMapFBO(){
  return depthMap;
}
