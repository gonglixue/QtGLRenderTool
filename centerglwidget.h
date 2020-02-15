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
    void loadMeshFromFile(QFile file);

    MyCamera camera;

protected:
    void initializeGL();
    // void paintGL();
    void resizeGL(int width, int height);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);

private:
    QOpenGLShaderProgram* program;
    int screenWidth, screenHeight;

    // Mesh
    MyMesh mesh;

    // transform
    QMatrix4x4 projection;


};

#endif // CENTERGLWIDGET_H
