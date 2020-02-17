#ifndef CENTERGLWIDGET_H
#define CENTERGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QPoint>
#include <QDir>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QVector3D>
#include <QVector2D>

#include <QDebug>

#include "mymesh.h"
#include "mycamera.h"

class CenterGLWidget:public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit CenterGLWidget(QWidget *parent = 0);
    ~CenterGLWidget();

    QString vShaderFile;
    QString fShaderFile;
    void cleanup();
    void loadMeshFromFile(QFile &file);

    MyCamera camera;

    static int qNormalizeAngle(int angle)
    {
        while(angle < 0)
            angle += 360 * 16;
        while(angle > 360*16)
            angle -= 360 * 16;
        return angle;
    }

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    // QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void keyPressEvent(QKeyEvent *event);

private:
    QOpenGLShaderProgram* program;
    int screenWidth, screenHeight;
    QPoint mouseLastPos;
    QPoint mouseCurPos;
    bool arcball;

    // Mesh
    MyMesh mesh;

    // transform
    QMatrix4x4 projection;
    QMatrix4x4 model;

    // render config
    QVector3D objectColor;
    QVector3D lightColor;
    QVector3D lightPos;
    GLfloat model_scale;

    int xRotAngle;
    int yRotAngle;
    int zRotAngle;

    // gl var
    QOpenGLBuffer VBO;
    QOpenGLBuffer EBO;
    QOpenGLVertexArrayObject VAO;

    void setupVertexAttribs();
    QVector3D getArcballVector(int x, int y);

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void setLightX(GLfloat light_x);
    void setLightY(GLfloat light_y);
    void setLightZ(GLfloat light_z);

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

};

#endif // CENTERGLWIDGET_H
