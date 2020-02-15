#ifndef MYMESH_H
#define MYMESH_H

#include <vector>
#include <qopengl.h>
#include <QVector3D>
#include <QVector2D>
#include <QString>
#include <QFile>
#include <QOpenGLFunctions>


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
    MyMesh(std::vector<Vertex> &vertices_in, std::vector<int> &indices_in):vertices(vertices_in),
        indices(indices_in), max_vert(-1e9, -1e9, -1e9), min_vert(1e9, 1e9, 1e9){}
    MyMesh(QFile file):max_vert(-1e9, -1e9, -1e9), min_vert(1e9, 1e9, 1e9) {

        loadObj(file);
        normalize(1.0f, QVector3D(0.0f, 0.0f, 0.0f));
    }
    MyMesh():max_vert(-1e9, -1e9, -1e9), min_vert(1e9, 1e9, 1e9){}


    void loadOBJ(QFile);
    void normalize(float length, QVector3D center);
    void set_center(QVector3D center);

private:
    std::vector<Vertex> vertices;
    std::vector<int> indices;

    QVector3D max_vert, min_vert;

};

#endif // MYMESH_H
