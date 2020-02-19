#ifndef MYSHADER_H
#define MYSHADER_H
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QString>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix2x2>
#include <QMatrix3x3>
#include <QMatrix4x4>
#include <QDebug>
#include <iostream>

enum SHADER_TYPE{
    VERTEX_SHADER,
    FRAGMENT_SHADER,
    GEOMETRY_SHADER
};

class MyShader
{
public:
    MyShader();
    MyShader(QString vShaderFile, QString fSahderFile);
    MyShader(QString vShaderFile, QString gShaderFile, QString fSahderFile);
    ~MyShader();

    void init_shader(QString vShaderFile, QString fShaderFile);
    void init_shader(QString vShaderFile, QString gShaderFile, QString fShaderFile);
    bool AddShader(QString file_name, SHADER_TYPE type);

    bool bind(){
        if(!program->bind())
            std::cerr << "unable to bind program";
    }
    void unbind(){
        program->release();
    }

    template <typename T>
    void setUniformValue(const char* name, const T& value) const
    {
        GLuint loc = program->uniformLocation(name);
        program->setUniformValue(loc, value);
    }


private:
    QOpenGLShaderProgram* program;
    bool link(){
        if(!program->link())
            std::cerr << "unable to link shader program\n";
    }
};

#endif // MYSHADER_H
