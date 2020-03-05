#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionLoadOBJ, SIGNAL(triggered(bool)), this, SLOT(LoadOBJ()));
    connect(ui->refresh_shader_btn, SIGNAL(released()), this, SLOT(ReFreshShader()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::LoadOBJ()
{
    qDebug() << "load file action slot in mainwindow";
    QString fn = QFileDialog::getOpenFileName(
                this,
                tr("Open an OBJ Model File"),
                QString(),
                tr("OBJ File(*.obj)")
                );
    // QString fn = "F:/Documents/QtProject/GLSLQt/OpenGL-in-Qt-with-GLEW/testdata.obj";
    if(!fn.isEmpty()){
        QFile file(fn);
        if(!file.open(QIODevice::ReadOnly)){
            QMessageBox::critical(this,
                                  tr("Error"),
                                  tr("Could not open this obj file"));
            return;
        }
        qDebug() << "prepare to read obj mesh";
        this->ui->mainGLWidget->loadMeshFromFile(file);
    }
}

void MainWindow::ReFreshShader()
{
    this->ui->mainGLWidget->reLinkShader();
}
