#include "centerglwidget.h"

CenterGLWidget::CenterGLWidget(QWidget* parent):QOpenGLWidget(parent), camera(QVector3D(0, 1.5f, 6.0f)),
    VBO(QOpenGLBuffer::VertexBuffer), EBO(QOpenGLBuffer::IndexBuffer)
{
    screenHeight = 500;
    screenHeight = 500;

    vShaderFile = "F:/Documents/QtProject/QtGLRenderTool/gooch.vert";
    fShaderFile = "F:/Documents/QtProject/QtGLRenderTool/gooch.frag";

    qDebug() << "vertex shader path: " << vShaderFile;
    qDebug() << "fragment shader path: " << fShaderFile;

    lightPos = QVector3D(1.2f, 1.0f, 2.0f);
    objectColor = QVector3D(0.9f, 0.9f, 0.9f);
    lightColor = QVector3D(1.0f, 1.0f, 1.0f);

    xRotAngle = 0;
    yRotAngle = 0;
    zRotAngle = 0;

    program = new QOpenGLShaderProgram;
}

CenterGLWidget::~CenterGLWidget()
{
    cleanup();
}

void CenterGLWidget::cleanup()
{
    makeCurrent();
    delete this->program;
    this->program = 0;
    doneCurrent();
}

void CenterGLWidget::loadMeshFromFile(QFile &file)
{
    qDebug() << "begin CenterGLWidget::loadMeshFromFile";
    this->mesh.loadOBJ(file);
    QVector3D c(0.0f, 0.0f, 0.3f);
    //this->mesh.normalize(1.0, c);
    setupVertexAttribs();
    // update();
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


    if(!program->addShaderFromSourceFile(QOpenGLShader::Vertex, vShaderFile))
    {
        std::cerr <<"unable to compile vertx shader: " ;
        std::cerr << vShaderFile.toStdString() << std::endl;
    }

    if (!program->addShaderFromSourceFile(QOpenGLShader::Fragment, fShaderFile))
        std::cerr <<"unable to compile fragmet shader: " << fShaderFile.toStdString() << endl;

    if (!program->link())
        std::cerr <<"unable to link shader program\n";
    program->bind();

}

void CenterGLWidget::setupVertexAttribs()
{
    this->VAO.create();
    QOpenGLVertexArrayObject::Binder VAOBinder(&this->VAO);
    this->VBO.create();
    this->EBO.create();

    VBO.bind();
    VBO.allocate(&this->mesh.vertices[0], this->mesh.vertices.size()*sizeof(Vertex));
    EBO.bind();
    EBO.allocate(&this->mesh.indices[0], this->mesh.indices.size()*sizeof(GLuint));

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    // POSITION
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(0));
    // NORMALS
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, Normal)));

    // TODO: TEXCOORDS
}

void CenterGLWidget::paintGL()
{
    qDebug() << "paintGL";
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QOpenGLVertexArrayObject::Binder vaoBinder(&this->VAO);
    QMatrix4x4 model;
    model.setToIdentity();
    //model.rotate(xRotAngle, QVector3D(1, 0, 0));
    //model.rotate(yRotAngle, QVector3D(0, 1, 0));
    //model.rotate(zRotAngle, QVector3D(0, 0, 1));
    //model.scale(10.0f);

    this->program->bind();
    //qDebug() << "program bind ok";
    //this->VAO.bind();
    GLuint objectColorLoc = program->uniformLocation("u_objectColor");
    GLuint coolColorLoc = program->uniformLocation("u_coolColor");
    GLuint warmColorLoc = program->uniformLocation("u_warmColor");
    GLuint lightColorLoc = program->uniformLocation("u_lightColor");
    GLuint lightPosLoc = program->uniformLocation("u_lightPos");
    GLuint viewPosLoc = program->uniformLocation("u_viewPos");
    GLuint alphaLoc = program->uniformLocation("u_alpha");
    GLuint betaLoc = program->uniformLocation("u_beta");
    GLuint modelLoc = program->uniformLocation("u_model_mat");
    GLuint viewLoc = program->uniformLocation("u_view_mat");
    GLuint projLoc = program->uniformLocation("u_projection_mat");

    program->setUniformValue(objectColorLoc, this->objectColor);
    program->setUniformValue(coolColorLoc, QVector3D(0, 0, 1));
    program->setUniformValue(warmColorLoc, QVector3D(1, 0, 0));
    program->setUniformValue(alphaLoc, 0.5f);
    program->setUniformValue(betaLoc, 0.5f);
    program->setUniformValue(lightColorLoc, this->lightColor);
    program->setUniformValue(lightPosLoc, this->lightPos);
    program->setUniformValue(viewPosLoc, this->camera.Position );


    program->setUniformValue(modelLoc, model);
    program->setUniformValue(viewLoc, this->camera.GetViewMatrix());
    program->setUniformValue(projLoc, this->projection);
    //qDebug() << "set uniform ok";
    //glDrawArrays(GL_TRIANGLES, 0, this->mesh.vertices.size());
    glDrawElements(GL_TRIANGLES, this->mesh.indices.size(), GL_UNSIGNED_INT, 0);
    //qDebug() << "draw ok";
    program->release();
}

/*
void CenterGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(this->mesh.vertices.empty())
    {

        // this->program->bind();
        // program->release();
        return;
    }
    qDebug() << "draw";
    QOpenGLVertexArrayObject::Binder vaoBinder(&this->VAO);
    
    QMatrix4x4 model;
    model.setToIdentity();
    // TODO: model transformation
    
    this->program->bind();
    GLuint loc_model = program->uniformLocation("model");
    GLuint loc_view = program->uniformLocation("view");
    GLuint loc_projection = program->uniformLocation("projection");

//    GLuint loc_lightPos = program->uniformLocation("lightPos");
//    GLuint loc_viewPos = program->uniformLocation("viewPos");
//    GLuint loc_lightColor = program->uniformLocation("lightColor");
//    GLuint loc_objColor = program->uniformLocation("objectColor");

    program->setUniformValue(loc_model, model);
    program->setUniformValue(loc_view, this->camera.GetViewMatrix());
    program->setUniformValue(loc_projection, this->projection);

//    program->setUniformValue(loc_lightPos, this->lightPos);
//    program->setUniformValue(loc_viewPos, this->camera.Position);
//    program->setUniformValue(loc_lightColor, this->lightColor);
//    program->setUniformValue(loc_objColor, this->objectColor);

    glDrawElements(GL_TRIANGLES, this->mesh.indices.size(), GL_UNSIGNED_INT, 0);

    program->release();

}
*/

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
    this->mouseLastPos = event->pos();
}

void CenterGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - mouseLastPos.x();
    int dy = event->y() - mouseLastPos.y();
    if(event->button() & Qt::LeftButton)
    {
        qDebug() << "Mouse Event";
        setXRotation(xRotAngle + 8*dy);
        setYRotation(yRotAngle + 8*dx);
    }
    else if(event->button() & Qt::RightButton){
        setXRotation(xRotAngle + 8*dy);
        setZRotation(zRotAngle + 8*dx);
    }

    mouseLastPos = event->pos();
    update();
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
