#ifndef MYMESH_H
#define MYMESH_H

#include <vector>
#include <iostream>
#include <qopengl.h>
#include <QVector3D>
#include <QVector2D>
#include <QString>
#include <QFile>
#include <QOpenGLFunctions>
#include <QDebug>

struct Vertex{
    QVector3D Position;
    QVector3D Normal;
    QVector2D TexCoords;
};

struct Texture{
    GLuint id;
    QString type;
};

class MyMesh
{
public:
    MyMesh(std::vector<Vertex> &vertices_in, std::vector<GLuint> &indices_in):vertices(vertices_in),
        indices(indices_in), max_vert(-1e9, -1e9, -1e9), min_vert(1e9, 1e9, 1e9){}
    MyMesh(QFile &file):max_vert(-1e9, -1e9, -1e9), min_vert(1e9, 1e9, 1e9) {

        loadOBJ(file);
        QVector3D c(.0f, .0f, .0f);
        //normalize(1.0f, c);
    }
    MyMesh():max_vert(-1e9, -1e9, -1e9), min_vert(1e9, 1e9, 1e9){}


    void loadOBJ(QFile& file);
    void normalize(float length, QVector3D& center);
    void set_center(QVector3D& center);

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
private:


    QVector3D max_vert, min_vert;

};

#endif // MYMESH_H
