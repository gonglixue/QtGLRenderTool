#include "centerglwidget.h"

CenterGLWidget::CenterGLWidget(QWidget* parent):QOpenGLWidget(parent), camera(QVector3D(0, 0.5f, 5.0f))
{
    screenHeight = 500;
    screenHeight = 500;

    vShaderFile = "F:/Documents/QtProject/QtGLRenderTool/basiclambert.vert";
    fShaderFile = "F:/Documents/QtProject/QtGLRenderTool/basiclambert.frag";

    qDebug() << "vertex shader path: " << vShaderFile;
    qDebug() << "fragment shader path: " << fShaderFile;

    lightPos = QVector3D(0.5, 1.0f, 1.5f);
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

    quadVAO = 0;
    quadVBO = 0;

}

CenterGLWidget::~CenterGLWidget()
{
    cleanup();
}

void CenterGLWidget::cleanup()
{
    makeCurrent();
    delete shader_program;
    delete simple_depth_shader_program;
    delete shadow_mapping_program;
    delete world_coordinate;
    delete shadow;
    delete mesh;
    delete plane;
    // delete debug_shader_program;
    doneCurrent();
}

void CenterGLWidget::loadMeshFromFile(QFile &file)
{
    qDebug() << "begin CenterGLWidget::loadMeshFromFile";
    this->mesh->loadOBJ(file);
    QVector3D c(0.0f, 0.0f, 0.3f);
    this->mesh->normalize(1.0, c);


    // update plane y
    MyMesh::UpdatePlaneY(*plane, this->mesh->MinVert().y());
    setupVertexAttribs();


    update();
    qDebug() << "reload succesfully";
}

void CenterGLWidget::initializeGL()
{
    qDebug() << "CenterGLWidget Initialize";

    core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();

    core->glEnable(GL_DEPTH_TEST);
    core->glDisable(GL_CULL_FACE);
    core->glClearColor(0, 0, 0.4f, 1);

    // load obj once program starts


    shader_program = new MyShader(vShaderFile, fShaderFile);
    simple_depth_shader_program = new MyShader("F:/Documents/QtProject/QtGLRenderTool/shadow_mapping_depth.vs",
                                               "F:/Documents/QtProject/QtGLRenderTool/shadow_mapping_depth.frag");
    shadow_mapping_program = new MyShader("F:/Documents/QtProject/QtGLRenderTool/shadow_mapping.vs",
                                          "F:/Documents/QtProject/QtGLRenderTool/shadow_mapping.frag");

//    debug_shader_program = new MyShader("F:/Documents/QtProject/QtGLRenderTool/debug_quad.vs",
//                                        "F:/Documents/QtProject/QtGLRenderTool/debug_quad.frag");


    /* objects init */
    shadow = new Shadow();
    shadow->initFBO();
    // shadow->initQFBO();

    // init plane
    std::vector<Vertex> plane_vertices(6);

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

    plane = new MyMesh(plane_vertices);
    this->plane->setUpAttribute();
    mesh = new MyMesh();
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


    world_coordinate = new Coordinate();
    world_coordinate->init();

    setShadowLighting();

    qDebug() << "finish initialGL";
}

void CenterGLWidget::setShadowLighting()
{
    QMatrix4x4 lightOrtho, lightView, lightSpaceMatrixOrtho;
     float near_plane = 1.0f, far_plane = 12.0f;  // TODO: check
    lightOrtho.ortho(-10.0, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

    lightView.lookAt(lightPos, QVector3D(0, 0, 0), QVector3D(0, 1, 0));
    lightSpaceMatrixOrtho = lightOrtho * lightView;

    simple_depth_shader_program->bind();
    simple_depth_shader_program->setUniformValue("lightSpaceMatrix", lightSpaceMatrixOrtho);

//    debug_shader_program->bind();
//    debug_shader_program->setUniformValue("depthMap", 0);
//    debug_shader_program->setUniformValue("near_plane", near_plane);
//    debug_shader_program->setUniformValue("far_plane", far_plane);

    shadow_mapping_program->bind();
    shadow_mapping_program->setUniformValue("lightSpaceMatrix", lightSpaceMatrixOrtho);
    shadow_mapping_program->setUniformValue("lightPos", lightPos);
}

void CenterGLWidget::setupVertexAttribs()
{
    this->plane->setUpAttribute();
    this->mesh->setUpAttribute();
    qDebug() << "setupVertexAttribs successfully";
}

void CenterGLWidget::paintGL()
{
    qDebug() << "update gl";
    core->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 view_mat = this->camera.GetViewMatrix();

    // arcball
    if(mouseLastPos != mouseCurPos)
    {
        // qDebug() << "paintGL::arcball active";
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

    // 2. render
    world_coordinate->draw(model, view_mat, projection);
    shader_program->bind();
    shader_program->setUniformValue<QMatrix4x4>("model", model);
    shader_program->setUniformValue("view", view_mat);
    shader_program->setUniformValue("projection", projection);
    shader_program->setUniformValue("lightPos", lightPos);
    shader_program->setUniformValue("viewPos", camera.Position);
    shader_program->setUniformValue("lightColor", lightColor);
    shader_program->setUniformValue("objectColor", objectColor);

    renderScenne();
    shader_program->unbind();

//    core->glViewport(0, 0, screenWidth, screenHeight);
//    core->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    world_coordinate->draw(model, view_mat, projection);

//    shadow_mapping_program->bind();
//    shadow_mapping_program->setUniformValue("model", model);
//    shadow_mapping_program->setUniformValue("view", view_mat);
//    shadow_mapping_program->setUniformValue("projection", projection);
//    shadow_mapping_program->setUniformValue("viewPos", camera.Position);
//    shadow_mapping_program->setUniformValue("lightColor", lightColor);
//    shadow_mapping_program->setUniformValue("kernel_radius", 2);
//    core->glActiveTexture(GL_TEXTURE0);
//    core->glBindTexture(GL_TEXTURE_2D, shadow->getDepthMapFBO());
//    renderScenne();
//    qDebug() << "   update gl:pass 1";

//    // off-screen render to get depthmap
//    shadow->bindFBO();
//    simple_depth_shader_program->bind();
//    simple_depth_shader_program->setUniformValue("model", model);
//    renderScenne();
//    shadow->releaseFBO();
//    qDebug() << "   update gl:pass 2";




}

void CenterGLWidget::renderScenne()
{
    this->plane->draw();
    this->mesh->draw();
}

void CenterGLWidget::renderQuad()
{
    if (quadVAO==0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        core->glGenBuffers(1, &quadVBO);
        core->glGenVertexArrays(1, &quadVAO);
        core->glBindVertexArray(quadVAO);

        core->glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        core->glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);


        core->glEnableVertexAttribArray(0);
        core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        core->glEnableVertexAttribArray(1);
        core->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

        core->glBindVertexArray(0);
    }
    core->glBindVertexArray(quadVAO);
    core->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    core->glBindVertexArray(0);
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

void CenterGLWidget::reLinkShader()
{
    this->shadow_mapping_program->reLoad();
    setShadowLighting();
    update();
}
