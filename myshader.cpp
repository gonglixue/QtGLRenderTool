#include "myshader.h"

MyShader::MyShader()
{
    this->program = new QOpenGLShaderProgram;
    qDebug() << "default MyShader::Myshader()";
}

MyShader::MyShader(QString vShaderFile, QString fShaderFile)
{
    this->program = new QOpenGLShaderProgram;
    init_shader(vShaderFile, fShaderFile);
}

MyShader::MyShader(QString vShaderFile, QString gShaderFile, QString fShaderFile)
{
    this->program = new QOpenGLShaderProgram;
    init_shader(vShaderFile, gShaderFile, fShaderFile);
}

MyShader::~MyShader()
{
    delete this->program;
}

void MyShader::init_shader(QString vShaderFile, QString fShaderFile)
{
    AddShader(vShaderFile, VERTEX_SHADER);
    AddShader(fShaderFile, FRAGMENT_SHADER);
    qDebug() << "add shader success";
    link();
    qDebug() << "link shader success";
}

void MyShader::init_shader(QString vShaderFile, QString gShaderFile, QString fShaderFile)
{
    AddShader(vShaderFile, VERTEX_SHADER);
    AddShader(gShaderFile, GEOMETRY_SHADER);
    AddShader(fShaderFile, FRAGMENT_SHADER);
    qDebug() << "add shader success";
    link();
    qDebug() << "link shader success";
}


bool MyShader::AddShader(QString file_name, SHADER_TYPE type)
{
    switch(type)
    {
    case VERTEX_SHADER:
        if(!program->addShaderFromSourceFile(QOpenGLShader::Vertex, file_name))
        {
            std::cerr <<"unable to compile Vertx shader: " << file_name.toStdString() << std::endl;
            return false;
        }
        break;
    case FRAGMENT_SHADER:
        if(!program->addShaderFromSourceFile(QOpenGLShader::Fragment, file_name))
        {
            std::cerr <<"unable to compile Fragment shader: "  << file_name.toStdString() << std::endl;
            return false;
        }
        break;
    case GEOMETRY_SHADER:
        if(!program->addShaderFromSourceFile(QOpenGLShader::Geometry, file_name))
        {
            std::cerr <<"unable to compile Geometry shader: "  << file_name.toStdString() << std::endl;
            return false;
        }
        break;
    }

    return true;

}
