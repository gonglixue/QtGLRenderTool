#include "centerglwidget.h"

CenterGLWidget::CenterGLWidget(QWidget* parent):QOpenGLWidget(parent), camera(QVector3D(0, 0.5f, 5.0f)),
    VBO(QOpenGLBuffer::VertexBuffer), planeVBO(QOpenGLBuffer::VertexBuffer)
{
    screenHeight = 500;
    screenHeight = 500;

    vShaderFile = "F:/Documents/QtProject/QtGLRenderTool/basiclambert.vert";
    fShaderFile = "F:/Documents/QtProject/QtGLRenderTool/basiclambert.frag";

    qDebug() << "vertex shader path: " << vShaderFile;
    qDebug() << "fragment shader path: " << fShaderFile;

    lightPos = QVector3D(1.2f, 1.0f, 2.0f);
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
    std::vector<Vertex> plane_vertices(6);
    plane_vertices[0].Position = QVector3D(-1.0f, 0, 1.0f);
    plane_vertices[1].Position = QVector3D(-1.0f, 0, -1.0f);
    plane_vertices[2].Position = QVector3D(1.0, 0, -1.0f);
    plane_vertices[3].Position = QVector3D(-1.0f, 0.0f, 1.0f);
    plane_vertices[4].Position = QVector3D(1.0f, 0, -1.0f);
    plane_vertices[5].Position = QVector3D(1.0f, 0, 1.0f);

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
    this->planeVAO.destroy();
    this->planeVBO.destroy();
    doneCurrent();
}

void CenterGLWidget::loadMeshFromFile(QFile &file)
{
    qDebug() << "begin CenterGLWidget::loadMeshFromFile";
    this->mesh.loadOBJ(file);
    QVector3D c(0.0f, 0.0f, 0.3f);
    this->mesh.normalize(1.0, c);

    // update plane y
    MyMesh::UpdatePlaneY(plane, this->mesh.MinVert().y());

    setupVertexAttribs();
    update();
}

void CenterGLWidget::initializeGL()
{
    qDebug() << "CenterGLWidget Initialize";
    makeCurrent();
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);
    glDisable(GL_CULL_FACE);
    glClearColor(0, 0, 0.4f, 1);

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

}

void CenterGLWidget::setupVertexAttribs()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    if(!VAO.isCreated())
        VAO.create();
    if(!VBO.isCreated())
        VBO.create();
    if(!this->mesh.vertices.empty()){
        VAO.bind();
        VBO.bind();
        VBO.allocate(&this->mesh.vertices[0], this->mesh.vertices.size()*sizeof(Vertex));

        // POSITION
        f->glEnableVertexAttribArray(0);
        f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(0));
        // NORMALS
        f->glEnableVertexAttribArray(1);
        f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, Normal)));
        // TEXCOORDS
        f->glEnableVertexAttribArray(2);
        f->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, TexCoords)));

        VAO.release();
    }

    if(!this->planeVAO.isCreated())
        this->planeVAO.create();
    if(!this->planeVBO.isCreated())
        this->planeVBO.create();
    planeVAO.bind();
    planeVBO.bind();
    planeVBO.allocate(&this->plane.vertices[0], this->plane.vertices.size() * sizeof(Vertex));
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(0));
    // NORMALS
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, Normal)));
    // TEXCOORDS
    f->glEnableVertexAttribArray(2);
    f->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, TexCoords)));
    planeVAO.release();

}

void CenterGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 view_mat = this->camera.GetViewMatrix();

    // arcball
    if(mouseLastPos != mouseCurPos) // TODO: check `!=` overload
    {
        qDebug() << "paintGL::arcball active";
        QVector3D va = getArcballVector(mouseLastPos.x(), mouseLastPos.y());
        QVector3D vb = getArcballVector(mouseCurPos.x(), mouseCurPos.y());
        qDebug() << va;
        qDebug() << vb;

        float dot_va_vb = QVector3D::dotProduct(va, vb); // TODO: check static
        float angle = 30.0f * acosf(3.0f < dot_va_vb ? 3.0f : dot_va_vb);

        qDebug() << "paintGL::arcball active " << angle;

        QVector3D axis_in_camera_coord = QVector3D::crossProduct(va, vb);
        QMatrix4x4 camera2object =  (model).inverted();
        QVector3D axis_in_object_coord = camera2object * axis_in_camera_coord;

        model.rotate(angle, axis_in_object_coord);  // TODO: check state accumulate

        mouseLastPos = mouseCurPos;
    }

    shader_program->bind();
    shader_program->setUniformValue<QMatrix4x4>("model", model);
    shader_program->setUniformValue("view", view_mat);
    shader_program->setUniformValue("projection", projection);
    shader_program->setUniformValue("lightPos", lightPos);
    shader_program->setUniformValue("viewPos", camera.Position);
    shader_program->setUniformValue("lightColor", lightColor);
    shader_program->setUniformValue("objectColor", objectColor);

    planeVAO.bind();
    glDrawArrays(GL_TRIANGLES, 0, this->plane.vertices.size());

    if(!this->mesh.vertices.empty())
    {
        VAO.bind();
        // glDrawElements(GL_TRIANGLES, this->mesh.indices.size(), GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, this->mesh.vertices.size());
    }
    shader_program->unbind();

}


void CenterGLWidget::resizeGL(int w, int h)
{
    screenHeight = h;
    screenWidth = w;
    glViewport(0, 0, w, h);
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
