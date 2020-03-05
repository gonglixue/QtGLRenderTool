#ifndef CENTERGLWIDGET_H
#define CENTERGLWIDGET_H

#include <memory>

#include <QOpenGLWidget>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QWidget>
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
#include <QOpenGLFramebufferObject>
#include <QOpenGLFunctions_3_3_Core>

#include <QDebug>


#include "mymesh.h"
#include "mycamera.h"
#include "myshader.h"
#include "texture2d.h"
#include "littlethings.h"

class CenterGLWidget:public QOpenGLWidget
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
    QOpenGLFunctions_3_3_Core *core;

    MyShader* shader_program;

    // event
    int screenWidth, screenHeight;
    QPoint mouseLastPos;
    QPoint mouseCurPos;
    bool arcball;
    int xRotAngle;
    int yRotAngle;
    int zRotAngle;
    QVector3D getArcballVector(int x, int y);


    // Mesh
    MyMesh* mesh;
    MyMesh* plane;

    // transform
    QMatrix4x4 projection;
    QMatrix4x4 model;

    // render environment config
    QVector3D objectColor;
    QVector3D lightColor;
    QVector3D lightPos;
    GLfloat model_scale;

    // shadow map shader
    MyShader* simple_depth_shader_program;
    MyShader* shadow_mapping_program;

    // little things
    Shadow *shadow;
    Coordinate* world_coordinate;

    void setupVertexAttribs();
    void renderScenne();
    void setShadowLighting();

    // debug
    GLuint quadVBO, quadVAO;
    void renderQuad();
    // MyShader* debug_shader_program;

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void setLightX(GLfloat light_x);
    void setLightY(GLfloat light_y);
    void setLightZ(GLfloat light_z);
    void reLinkShader();

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

};

#endif // CENTERGLWIDGET_H
