#pragma once

#include <QObject>
#include <qcustomplot.h>
#include<memory>
class Painter: public QWidget
{
  Q_OBJECT
public:
   Painter (QCustomPlot *plt, QWidget *parent = nullptr);
private:
 std::unique_ptr<QCustomPlot> plot;
  std::unique_ptr<QShortcut> keyCtrlPlus ;
  std::unique_ptr<QShortcut> keyCtrlMinus;
  std::unique_ptr<QShortcut> keyShiftPlus;
  std::unique_ptr<QShortcut> keyShiftMinus;
  std::unique_ptr<QShortcut> keyUp;
  std::unique_ptr<QShortcut> keyRight;
  std::unique_ptr<QShortcut> keyDown;
  std::unique_ptr<QShortcut> keyLeft;
  void init();
public slots:
  void printArr(std::vector<double> &arr, double fd);
};


