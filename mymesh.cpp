#include "mymesh.h"
#include <QTextStream>

using namespace std;


void MyMesh::loadOBJ(QFile& file)
{
    qDebug() << "enter MyMesh::LoadOBJ";
    cout << "enter MyMesh::loadOBJ" << endl;
    QTextStream in(&file);

    int vCount=0, vnCount=0, vtCount=0;
    QString line = in.readLine();

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

            Vertex vert;
            vert.Position = QVector3D(x, y, z);
            vertices.push_back(vert);
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

            vertices[vnCount].Normal = QVector3D(x, y, z);
            vnCount++;
        }
        else if(line.left(2) == "vt")
        {
            has_tex = true;

            line.remove(0, 2);
            QStringList vertex_texcoords = line.split(' ',QString::SkipEmptyParts);
            GLfloat x = vertex_texcoords[0].toFloat();
            GLfloat y = vertex_texcoords[1].toFloat();

            vertices[vtCount].TexCoords = QVector2D(x, y);
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
                GLuint v_index = aVert_index[0].toInt() - 1;
                //int t_index = aVert_index[1].toInt();
                GLuint n_index = aVert_index[2].toInt() - 1;

                if(v_index != n_index){
                    cout << "v_index != n_index";
                }
                else{
                    //qDebug() << n_index << "/";
                    indices.push_back(n_index);
                }
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
