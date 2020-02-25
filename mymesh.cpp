#include "mymesh.h"
#include <QTextStream>

using namespace std;

MyMesh::MyMesh(std::vector<Vertex> &vertices_in, std::vector<GLuint> &indices_in):max_vert(-1e9, -1e9, -1e9), min_vert(1e9, 1e9, 1e9)
{
    this->triangles_num_ = indices_in.size() / 3;
    for(int i=0; i<indices_in.size(); i++)
    {
        int vert_ind = indices_in[i];  // ATTENTION: vert_ind starts from zero
        Vertex v = vertices_in[vert_ind];
        this->vertices.push_back(v);
        float x = v.Position.x();
        float y = v.Position.y();
        float z = v.Position.z();
        if (x < min_vert.x())
            min_vert.setX(x);
        if (x > max_vert.x())
            max_vert.setX(x);
        if (y < min_vert.y())
            min_vert.setY(y);
        if (y > max_vert.y())
            max_vert.setY(y);
        if (z < min_vert.z())
            min_vert.setZ(z);
        if (z > max_vert.z())
            max_vert.setZ(z);
    }

    has_tex = false;
}

MyMesh::MyMesh(std::vector<Vertex> &vertices_in):max_vert(-1e9, -1e9, -1e9), min_vert(1e9, 1e9, 1e9)
{
    this->triangles_num_ = vertices_in.size() / 3;
    this->vertices = vertices_in;
    for(int i=0; i<vertices_in.size(); i++)
    {
        Vertex v = vertices_in[i];
        float x = v.Position.x();
        float y = v.Position.y();
        float z = v.Position.z();
        if (x < min_vert.x())
            min_vert.setX(x);
        if (x > max_vert.x())
            max_vert.setX(x);
        if (y < min_vert.y())
            min_vert.setY(y);
        if (y > max_vert.y())
            max_vert.setY(y);
        if (z < min_vert.z())
            min_vert.setZ(z);
        if (z > max_vert.z())
            max_vert.setZ(z);
    }

    has_tex = false;
}

void MyMesh::loadOBJ(QFile& file)
{
    qDebug() << "enter MyMesh::LoadOBJ";
    this->vertices.clear();

    QTextStream in(&file);

    int vCount=0, vnCount=0, vtCount=0;
    QString line = in.readLine();

    vector<QVector3D> temp_vertices;
    vector<QVector3D> temp_normals;
    vector<QVector2D> temp_texcoords;

    while(true)
    {
        if(line.isEmpty()){
            line = in.readLine();
            continue;
        }

        if(line[0] == '#'){
            cout << line.toStdString();
        }
        else if(line.left(2) == "v "){
            line.remove(0, 1);
            QStringList vertex_coords = line.split(' ', QString::SkipEmptyParts);
            GLfloat x = vertex_coords[0].toFloat();
            GLfloat y = vertex_coords[1].toFloat();
            GLfloat z = vertex_coords[2].toFloat();

//            Vertex vert;
//            vert.Position = QVector3D(x, y, z);
//            vertices.push_back(vert);
            temp_vertices.push_back(QVector3D(x, y, z));
            vCount++;

            {
                if (x < min_vert.x())
                    min_vert.setX(x);
                if (x > max_vert.x())
                    max_vert.setX(x);
                if (y < min_vert.y())
                    min_vert.setY(y);
                if (y > max_vert.y())
                    max_vert.setY(y);
                if (z < min_vert.z())
                    min_vert.setZ(z);
                if (z > max_vert.z())
                    max_vert.setZ(z);
            }
        }
        else if(line.left(2) == "vn")
        {
            line.remove(0, 2);
            QStringList vertex_normal = line.split(' ', QString::SkipEmptyParts);
            GLfloat x = vertex_normal[0].toFloat();
            GLfloat y = vertex_normal[1].toFloat();
            GLfloat z = vertex_normal[2].toFloat();

            // vertices[vnCount].Normal = QVector3D(x, y, z);
            temp_normals.push_back(QVector3D(x, y, z));
            vnCount++;
        }
        else if(line.left(2) == "vt")
        {
            has_tex = true;

            line.remove(0, 2);
            QStringList vertex_texcoords = line.split(' ',QString::SkipEmptyParts);
            GLfloat x = vertex_texcoords[0].toFloat();
            GLfloat y = vertex_texcoords[1].toFloat();

            // vertices[vtCount].TexCoords = QVector2D(x, y);
            temp_texcoords.push_back(QVector2D(x, y));
            vtCount++;
        }
        else if(line.left(2) == "f ")
        {
            line.remove(0, 1);
            QStringList face_index = line.split(' ',QString::SkipEmptyParts);
            //int v_testIndex[3];

            for(int i=0;i<3;i++)
            {
                // TODO: texture coordinates
                QString aVert = face_index[i];
                QStringList  aVert_index = aVert.split('/');

                Vertex vert;
                int v_index = aVert_index[0].toInt() - 1;
                vert.Position = temp_vertices[v_index];


                if(aVert_index[1].length() > 0 && temp_texcoords.size() > 0){
                    int t_index = aVert_index[1].toInt() - 1;
                    vert.TexCoords = temp_texcoords[t_index];
                }

                if(aVert_index[2].length() > 0 && temp_normals.size() > 0)
                {
                    int n_index = aVert_index[2].toInt() - 1;
                    vert.Normal = temp_normals[n_index];
                }
                this->vertices.push_back(vert);
            }
        }

        if(in.atEnd())
            break;
        line = in.readLine();
    }

    cout << "load obj count" << vCount << ", " << vnCount;
    file.close();

    qDebug() << "max vert:" << max_vert;
    qDebug() << "min vert:" << min_vert;
    this->triangles_num_ = this->vertices.size() / 3;

}

void MyMesh::normalize(float length, QVector3D& center)
{
    float scale = fmax(max_vert.x() - min_vert.x(), max_vert.y() - min_vert.y());
    scale = fmax(scale, max_vert.z() - min_vert.z());
    scale = 1.0 / scale * length;

    for(int i=0;i<vertices.size();i++)
    {
        vertices[i].Position = scale * (this->vertices[i].Position - center);
    }

    max_vert = scale * (max_vert - center);
    min_vert = scale * (min_vert - center);

    qDebug() << "max vert:" << max_vert;
    qDebug() << "min vert:" << min_vert;
}

void MyMesh::set_center(QVector3D& center)
{
    for(int i=0;i<vertices.size();i++)
    {
        vertices[i].Position = (this->vertices[i].Position - center);
    }

    max_vert = max_vert - center;
    min_vert = min_vert - center;

    qDebug() << "max vert:" << max_vert;
    qDebug() << "min vert:" << min_vert;
}

void MyMesh::UpdatePlaneY(MyMesh &m, float y)
{
    for(int i=0;i<m.vertices.size();i++)
    {
        m.vertices[i].Position.setY(y);
        if (y < m.min_vert.y())
            m.min_vert.setY(y);
        if (y > m.max_vert.y())
            m.max_vert.setY(y);
    }
}
