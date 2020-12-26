#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <painter.h>
#include <file_system.h>
#include <QDebug>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();


private:
  Ui::MainWindow *ui;

  std::unique_ptr<Painter> plot;
 std::unique_ptr< FileSystem> fs;

};
#endif // MAINWINDOW_H
