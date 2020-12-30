#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->dockProperty->setVisible(false);
  ui->dockLog->setVisible(false);
  plot = std::make_unique<Painter>(ui->mainWidget);
  connect(ui->actOpenDir, &QAction::triggered, this, [=](){
      QString str = QFileDialog::getExistingDirectory(nullptr,"Open Dial","*.baph");
      QDir dir(str);
      QFileInfoList dirContent = dir.entryInfoList(QStringList()
             << "*.baph",
             QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
      if(dirContent.size() >0){
      ui->dockProperty->setVisible(true);
      for(auto dir: dirContent)
          ui->listProperty->addItem(dir.absoluteFilePath());}

  });

}

MainWindow::~MainWindow()
{
  delete ui;
}

