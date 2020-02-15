#include "centerglwidget.h"

CenterGLWidget::CenterGLWidget()
{

}

CenterGLWidget::~CenterGLWidget(){}

void CenterGLWidget::cleanup()
{

}

void CenterGLWidget::loadMeshFromFile(QFile file)
{
    this->mesh.loadOBJ(file);
}

void CenterGLWidget::initializeGL()
{

}

void CenterGLWidget::resizeGL()
{

}

QSize CenterGLWidget::minimumSizeHint()
{

}

QSize CenterGLWidget::sizeHint()
{

}

void CenterGLWidget::mousePressEvent(QMouseEvent *event)
{

}

void CenterGLWidget::mouseMoveEvent(QMouseEvent *event)
{

}

void CenterGLWidget::wheelEvent(QWheelEvent *event)
{

}

