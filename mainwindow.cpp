#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionLoadOBJ, SIGNAL(triggered(bool)), this, SLOT(LoadOBJ()));

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
    if(!fn.isEmpty()){
        QFile file(fn);
        if(!file.open(QIODevice::ReadOnly)){
            QMessageBox::critical(this,
                                  tr("Error"),
                                  tr("Could not open this obj file"));
            return;
        }
        qDebug() << "prepare to read obj mesh";
        this->ui->centralWidget->loadMeshFromFile(file);
    }
}
