﻿#include "centerglwidget.h"

#include "littlethings.h"

Shadow *shadow;
Plane  *p_plane;
Quad *quad;
Cube *cube;

CenterGLWidget::CenterGLWidget(QWidget* parent):QOpenGLWidget(parent), camera(QVector3D(0, 0.5f, 5.0f)),
    VBO(QOpenGLBuffer::VertexBuffer), planeVBO(QOpenGLBuffer::VertexBuffer), quadVBO(QOpenGLBuffer::VertexBuffer)
{
    screenHeight = 500;
    screenHeight = 500;

    vShaderFile = "F:/Documents/QtProject/QtGLRenderTool/basiclambert.vert";
    fShaderFile = "F:/Documents/QtProject/QtGLRenderTool/basiclambert.frag";

    qDebug() << "vertex shader path: " << vShaderFile;
    qDebug() << "fragment shader path: " << fShaderFile;

    lightPos = QVector3D(0, 3.0f, 0.0f);
    objectColor = QVector3D(0.9f, 0.9f, 0.9f);
    lightColor = QVector3D(1.0f, 1.0f, 1.0f);

    arcball = false;
    xRotAngle = 0;
    yRotAngle = 0;
    zRotAngle = 0;
    model_scale = 1.0;
    model.setToIdentity();
    model.rotate(xRotAngle, QVector3D(1, 0, 0));
    model.rotate(yRotAngle, QVector3D(0, 1, 0));
    model.rotate(zRotAngle, QVector3D(0, 0, 1));
    model.scale(model_scale);

    // init plane
    std::vector<Vertex> plane_vertices;
    plane_vertices.reserve(6);

    plane_vertices[0].Position = QVector3D(-1.0f, 0, 1.0f);
    plane_vertices[1].Position = QVector3D(-1.0f, 0, -1.0f);
    plane_vertices[2].Position = QVector3D(1, 0, -1.0f);
    plane_vertices[3].Position = QVector3D(-1.0f, 0.0f, 1.0f);
    plane_vertices[4].Position = QVector3D(1, 0, -1.0f);
    plane_vertices[5].Position = QVector3D(1, 0, 1.0f);

    plane_vertices[0].Normal = QVector3D(0, 1.0f, 0);
    plane_vertices[1].Normal = QVector3D(0, 1.0f, 0);
    plane_vertices[2].Normal = QVector3D(0, 1.0f, 0);
    plane_vertices[3].Normal = QVector3D(0, 1.0f, 0);
    plane_vertices[4].Normal = QVector3D(0, 1.0f, 0);
    plane_vertices[5].Normal = QVector3D(0, 1.0f, 0);

    plane_vertices[0].TexCoords = QVector2D(0.0f, 1.0f);
    plane_vertices[1].TexCoords = QVector2D(0.0f, 0.0f);
    plane_vertices[2].TexCoords = QVector2D(1.0f, 0.0f);
    plane_vertices[3].TexCoords = QVector2D(0.0f, 1.0f);
    plane_vertices[4].TexCoords = QVector2D(1.0f, 0.0f);
    plane_vertices[5].TexCoords = QVector2D(1.0f, 1.0f);

    plane = MyMesh(plane_vertices);

    depthMap = 0;
    depthMapFBO = 0;


}

CenterGLWidget::~CenterGLWidget()
{
    cleanup();
}

void CenterGLWidget::cleanup()
{
    makeCurrent();
    delete shader_program;
    this->VAO.destroy();
    this->VBO.destroy();
    //this->planeVAO.destroy();
    //this->planeVBO.destroy();
    doneCurrent();
}

void CenterGLWidget::loadMeshFromFile(QFile &file)
{
    qDebug() << "begin CenterGLWidget::loadMeshFromFile";
    this->mesh.loadOBJ(file);
    QVector3D c(0.0f, 0.0f, 0.3f);
    this->mesh.normalize(1.0, c);

    // update plane y
    // MyMesh::UpdatePlaneY(plane, this->mesh.MinVert().y());

    setupVertexAttribs();
    update();
}

void CenterGLWidget::initializeGL()
{
    qDebug() << "CenterGLWidget Initialize";
//    makeCurrent();
//    initializeOpenGLFunctions();

    core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();

    core->glEnable(GL_DEPTH_TEST);
    //core->glDisable(GL_CULL_FACE);
    core->glClearColor(0, 0, 0.4f, 1);

    // load obj once program starts
    QString fn = QFileDialog::getOpenFileName(
                this,
                tr("Open an OBJ Model File"),
                QString(),
                tr("OBJ File(*.obj)")
                );
    if(!fn.isEmpty()){
        QFile file(fn);
        if(!file.open(QIODevice::ReadOnly)){
            QMessageBox::critical(this,
                                  tr("Error"),
                                  tr("Could not open this obj file"));
            return;
        }
        qDebug() << "prepare to read obj mesh";
        this->loadMeshFromFile(file);
    }

    // setupVertexAttribs();
    shader_program = new MyShader(vShaderFile, fShaderFile);
    simple_depth_shader_program = new MyShader("F:/Documents/QtProject/QtGLRenderTool/shadow_mapping_depth.vs",
                                               "F:/Documents/QtProject/QtGLRenderTool/shadow_mapping_depth.frag");

    debug_shader_program = new MyShader("F:/Documents/QtProject/QtGLRenderTool/debug_quad.vs",
                                        "F:/Documents/QtProject/QtGLRenderTool/debug_quad.frag");


    /* objects init */
    mesh.setUpAttribute();

    cube = new Cube();
    cube->init();
    p_plane = new Plane();
    p_plane->init();
    quad = new Quad();
    quad->init();
    shadow = new Shadow();
    shadow->initFBO();
    shadow->initQFBO();


    // initFBO();
    QMatrix4x4 lightOrtho, lightView, lightSpaceMatrixOrtho;
     float near_plane = 1.0f, far_plane = 12.0f;  // TODO: check
    lightOrtho.ortho(-10.0, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

    lightView.lookAt(QVector3D(-2.0f, 4.0f, -1.0f), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
    lightSpaceMatrixOrtho = lightOrtho * lightView;

    simple_depth_shader_program->bind();
    simple_depth_shader_program->setUniformValue("lightSpaceMatrix", lightSpaceMatrixOrtho);

    debug_shader_program->bind();
    debug_shader_program->setUniformValue("depthMap", 0);
    debug_shader_program->setUniformValue("near_plane", near_plane);
    debug_shader_program->setUniformValue("far_plane", far_plane);

    test_texture.generate("F:/Documents/QtProject/ShadowMapping/res/textures/brickwall.jpg");
}

void CenterGLWidget::setupVertexAttribs()
{

    if(!VAO.isCreated())
        VAO.create();
    if(!VBO.isCreated())
        VBO.create();
    if(!this->mesh.vertices.empty()){
        VAO.bind();
        VBO.bind();
        VBO.allocate(&this->mesh.vertices[0], this->mesh.vertices.size()*sizeof(Vertex));

        // POSITION
        core->glEnableVertexAttribArray(0);
        core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(0));
        // NORMALS
        core->glEnableVertexAttribArray(1);
        core->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, Normal)));
        // TEXCOORDS
        core->glEnableVertexAttribArray(2);
        core->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, TexCoords)));

        VAO.release();
    }

//    if(!this->planeVAO.isCreated())
//        this->planeVAO.create();
//    if(!this->planeVBO.isCreated())
//        this->planeVBO.create();

//    if(!this->plane.vertices.empty())
//    {
//        planeVAO.bind();
//        planeVBO.bind();
//        planeVBO.allocate(&this->plane.vertices[0], this->plane.vertices.size() * sizeof(Vertex));
//        f->glEnableVertexAttribArray(0);
//        f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(0));
//        // NORMALS
//        f->glEnableVertexAttribArray(1);
//        f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, Normal)));
//        // TEXCOORDS
//        f->glEnableVertexAttribArray(2);
//        f->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, TexCoords)));
//        planeVAO.release();
//    }




}

void CenterGLWidget::paintGL()
{
    core->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 view_mat = this->camera.GetViewMatrix();

    // arcball
    if(mouseLastPos != mouseCurPos)
    {
        qDebug() << "paintGL::arcball active";
        QVector3D va = getArcballVector(mouseLastPos.x(), mouseLastPos.y());
        QVector3D vb = getArcballVector(mouseCurPos.x(), mouseCurPos.y());

        float dot_va_vb = QVector3D::dotProduct(va, vb);
        float angle = 30.0f * acosf(3.0f < dot_va_vb ? 3.0f : dot_va_vb);

        QVector3D axis_in_camera_coord = QVector3D::crossProduct(va, vb);
        QMatrix4x4 camera2object =  (model).inverted();
        QVector3D axis_in_object_coord = camera2object * axis_in_camera_coord;

        model.rotate(angle, axis_in_object_coord);

        mouseLastPos = mouseCurPos;
    }



//    core->glViewport(0, 0, screenWidth, screenHeight);
//    core->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    debug_shader_program->bind();
//    core->glActiveTexture(GL_TEXTURE0);
//    core->glBindTexture(GL_TEXTURE_2D, depthMap);
//    // test_texture.bind();
//    qDebug() << "depthMap:" << depthMap << "\n";
//    //core->glBindTexture(GL_TEXTURE_2D, qFBO->texture());
//    renderQuad();

//    simple_depth_shader_program->bind();
//    simple_depth_shader_program->setUniformValue("model", model);
//    core->glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
//    core->glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
//    // qFBO->bind();
//    core->glClear(GL_DEPTH_BUFFER_BIT);
//    renderScenne();
//    core->glBindFramebuffer(GL_FRAMEBUFFER, 0);
//    // qFBO->release();
//    simple_depth_shader_program->unbind();

//    // shadow->bindQFBO();
//    shadow->bindFBO();
//    renderObjects(simple_depth_shader_program);
//    // shadow->releaseQFBO();
//    shadow->releaseFBO();

//    core->glViewport(0, 0, screenWidth, screenHeight);
//    core->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    debug_shader_program->bind();
//    core->glActiveTexture(GL_TEXTURE0);
//    core->glBindTexture(GL_TEXTURE_2D, shadow->getDepthMapFBO());
//    quad->draw(GL_TRUE);


    // 2. render
//    shader_program->bind();
//    shader_program->setUniformValue<QMatrix4x4>("model", model);
//    shader_program->setUniformValue("view", view_mat);
//    shader_program->setUniformValue("projection", projection);
//    shader_program->setUniformValue("lightPos", lightPos);
//    shader_program->setUniformValue("viewPos", camera.Position);
//    shader_program->setUniformValue("lightColor", lightColor);
//    shader_program->setUniformValue("objectColor", objectColor);

//    renderScenne();
//    shader_program->unbind();

//    simple_depth_shader_program->bind();
//    simple_depth_shader_program->setUniformValue("model", model);
//    renderScenne();
//    simple_depth_shader_program->unbind();


    core->glViewport(0, 0, screenWidth, screenHeight);
    core->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    debug_shader_program->bind();
    core->glActiveTexture(GL_TEXTURE0);
    core->glBindTexture(GL_TEXTURE_2D, shadow->getDepthMapFBO());
    quad->draw(GL_TRUE);

    shadow->bindFBO();
    renderObjects(simple_depth_shader_program);
    // shadow->releaseQFBO();
    // mesh.draw();
    shadow->releaseFBO();


}

void CenterGLWidget::renderScenne()
{


    if(!this->mesh.vertices.empty())
    {
        VAO.bind();
        // glDrawElements(GL_TRIANGLES, this->mesh.indices.size(), GL_UNSIGNED_INT, 0);
        core->glDrawArrays(GL_TRIANGLES, 0, this->mesh.vertices.size());
        VAO.release();
    }

//    if(true)
//    {
//        planeVAO.bind();
//        glDrawArrays(GL_TRIANGLES, 0, 6);
//        planeVAO.release();
//    }

}

void CenterGLWidget::renderQuad()
{
    if (!quadVAO.isCreated())
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        quadVAO.create();
        quadVBO.create();
        quadVAO.bind();
        quadVBO.allocate(quadVertices, sizeof(quadVertices));


        core->glEnableVertexAttribArray(0);
        core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        core->glEnableVertexAttribArray(1);
        core->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

        quadVAO.release();
    }
    quadVAO.bind();
    core->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    quadVAO.release();
}


void CenterGLWidget::resizeGL(int w, int h)
{
    screenHeight = h;
    screenWidth = w;
    core->glViewport(0, 0, w, h);
    projection.setToIdentity();
    projection.perspective(camera.Zoom, GLfloat(w)/h, 0.01f, 100.0f);

}

void CenterGLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if(angle != this->xRotAngle)
    {
        this->xRotAngle = angle;
        emit xRotationChanged(angle);
        update();
    }
}
void CenterGLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if(angle != this->yRotAngle)
    {
        this->yRotAngle = angle;
        emit yRotationChanged(angle);
        update();
    }
}
void CenterGLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if(angle != zRotAngle)
    {
        zRotAngle = angle;
        emit zRotationChanged(angle);
        update();
    }
}

void CenterGLWidget::setLightX(GLfloat light_x)
{
    this->lightPos.setX(light_x);
    update();
}
void CenterGLWidget::setLightY(GLfloat light_y)
{
    this->lightPos.setY(light_y);
    update();
}
void CenterGLWidget::setLightZ(GLfloat light_z)
{
    this->lightPos.setZ(light_z);
    update();
}


QSize CenterGLWidget::sizeHint() const
{
    return QSize(screenWidth, screenHeight);
}

void CenterGLWidget::mousePressEvent(QMouseEvent *event)
{
    //qDebug() << "press";
    this->mouseLastPos = event->pos();
    this->mouseCurPos = event->pos();
    arcball = true;
}

void CenterGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    arcball = false;
}

void CenterGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(arcball)
    {
        // qDebug() << "Mouse Event Left";
        this->mouseCurPos = event->pos();
        update();
        // qDebug() << "move";
    }

}

void CenterGLWidget::wheelEvent(QWheelEvent *event)
{

}

void CenterGLWidget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case 87: camera.ProcessKeyboard(FORWARD, 0.01); break;
        case 83: camera.ProcessKeyboard(BACKWARD, 0.01); break;
        case 65: camera.ProcessKeyboard(LEFT, 0.01); break;
        case 68: camera.ProcessKeyboard(RIGHT, 0.01); break;
    }

    update();
}

QVector3D CenterGLWidget::getArcballVector(int x, int y)
{
    QVector3D p(1.0f * x/screenWidth * 2 - 1.0f,
                1.0f * y/screenHeight * 2 - 1.0f,
                0);
    p.setY(-p.y());

    float op_sqr = p.x() * p.x() + p.y() * p.y();
    float arcball_radius_sqr = 1.0f * 1.0f;
    if(op_sqr <= arcball_radius_sqr)
        p.setZ(sqrtf(arcball_radius_sqr - op_sqr));
    else
        p.normalize();  // TODO: check
    return p;
}

void CenterGLWidget::renderObjects(MyShader* shader)
{
    shader->bind();
    QMatrix4x4 mmodel;
    mmodel.translate(0.0f, -0.5f, 0.0f);
    mmodel.scale(50.0f, 1.0f, 50.0f);
    shader->setUniformValue("model", model*mmodel);
    p_plane->draw(GL_TRUE, GL_TRUE);

    mmodel.setToIdentity();
    mmodel.translate(0.0f, 1.5f, 0.0f);
    shader->setUniformValue("model", model*mmodel);
    cube->draw(GL_TRUE, GL_TRUE);

    mmodel.setToIdentity();
    mmodel.translate(2.0f, 0.0f, 1.0f);
    shader->setUniformValue("model",model* mmodel);
    cube->draw(GL_TRUE, GL_TRUE);

    mmodel.setToIdentity();
    mmodel.translate(-1.0f, 0.0f, 2.0f);
    mmodel.rotate(60.0f, QVector3D(1.0f, 0.0f, 1.0f));
    mmodel.scale(0.5f);
    shader->setUniformValue("model",model* mmodel);
    cube->draw(GL_TRUE, GL_TRUE);
}
