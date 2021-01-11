#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  plot = std::make_unique<Painter>(ui->mainWidget);

}

MainWindow::~MainWindow()
{
  delete ui;
}

