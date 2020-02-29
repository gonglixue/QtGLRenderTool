#ifndef MYMESH_H
#define MYMESH_H

#include <vector>
#include <iostream>
#include <qopengl.h>
#include <QVector3D>
#include <QVector2D>
#include <QMatrix4x4>
#include <QString>
#include <QFile>
#include <QDebug>

#include <QOpenGLFunctions_3_3_Core>

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
    MyMesh(std::vector<Vertex> &vertices_in, std::vector<GLuint> &indices_in);
    MyMesh(std::vector<Vertex> &vertices_in);
    MyMesh(QFile &file):max_vert(-1e9, -1e9, -1e9), min_vert(1e9, 1e9, 1e9) {
        has_tex = false;
        loadOBJ(file);
        QVector3D c(.0f, .0f, .0f);
        core = NULL;
    }
    MyMesh():max_vert(-1e9, -1e9, -1e9), min_vert(1e9, 1e9, 1e9){
        has_tex = false;
        core = NULL;
    }
    ~MyMesh();

    static void UpdatePlaneY(MyMesh& m, float y);


    void loadOBJ(QFile& file);
    void normalize(float length, QVector3D& center);
    void set_center(QVector3D& center);
    int TriangleNum() { return triangles_num_; }
    QVector3D MinVert() {return min_vert; }

    std::vector<Vertex> vertices;
    // std::vector<GLuint> indices;

    void draw();
    void setUpAttribute();

private:

    // QMatrix4x4 model;
    QVector3D max_vert, min_vert;
    bool has_tex;
    int triangles_num_;

    QOpenGLFunctions_3_3_Core *core;
    GLuint VAO=0, VBO=0;


};

#endif // MYMESH_H
