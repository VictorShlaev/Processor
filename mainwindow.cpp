#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  plot = std::make_unique<Painter>(ui->mainWidget);
  fs =std::make_unique<FileSystem>( ui->treeFS);
  connect (fs.get (),  &FileSystem::activated, this, [=](const QString& path){
      qDebug()<<path;
    });

}

MainWindow::~MainWindow()
{
  delete ui;
}

