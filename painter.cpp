#include "painter.h"


Painter::Painter(QCustomPlot *plt,QWidget *parent):QWidget (parent)
{
  plot =static_cast<std::unique_ptr<QCustomPlot>>(plt);
  init();
}


void Painter::init()
{
  //CTRL + Plus
  keyCtrlPlus =std::make_unique<QShortcut>(plot.get ());
  keyCtrlPlus->setKey (static_cast<Qt::Key>(Qt::CTRL)+ Qt::Key_Plus);
  connect (keyCtrlPlus.get (), &QShortcut::activated, this, [=](){
      plot->yAxis->scaleRange(.50,plot->yAxis->range().center());
      plot->replot();
    });
  //CTRL + Minus
  keyCtrlMinus = std::make_unique<QShortcut>(plot.get ());
  keyCtrlMinus->setKey (static_cast<Qt::Key>(Qt::CTRL) + Qt::Key_Minus);
  connect (keyCtrlMinus.get(), &QShortcut::activated, this, [=](){
      plot->yAxis->scaleRange(1/.50,plot->yAxis->range().center());
      plot->replot();
    });
  //Shift + Plus
  keyShiftPlus = std::make_unique<QShortcut>(plot.get ());
  keyShiftPlus->setKey (static_cast<Qt::Key>(Qt::SHIFT) + Qt::Key_Plus);
  connect (keyShiftPlus.get(), &QShortcut::activated, this, [=](){
      plot->xAxis->scaleRange(.50,plot->xAxis->range().center());
      plot->replot();
    });
  //Shift + Minus
  keyShiftMinus= std::make_unique<QShortcut>(plot.get ());
  keyShiftMinus->setKey (static_cast<Qt::Key>(Qt::SHIFT) + Qt::Key_Minus);
  connect (keyShiftMinus.get(), &QShortcut::activated, this, [=](){
      plot->xAxis->scaleRange(1/.50,plot->xAxis->range().center());
      plot->replot();
    });

  //Up
  keyUp = std::make_unique<QShortcut>(plot.get ());
  keyUp->setKey (Qt::Key_Up);
  connect (keyUp.get(), &QShortcut::activated, this, [=](){
      if(plot->yAxis->range ().upper <= 20){
          double step =plot->yAxis->range ().size ()/3.;
          plot->yAxis->setRange (  plot->yAxis->range ().lower + step,
                                   plot->yAxis->range ().upper + step);
          plot->replot ();
        }
    });
  //Right
  keyRight = std::make_unique<QShortcut>(plot.get ());
  keyRight->setKey (Qt::Key_Right);
  connect (keyRight.get(), &QShortcut::activated, this, [=](){
      if(plot->xAxis->range ().upper <= 200){
          double step =plot->xAxis->range ().size ()/3.;
          plot->xAxis->setRange (  plot->xAxis->range ().lower + step,
                                   plot->xAxis->range ().upper + step);
          plot->replot ();
        }
    });

  //Down
  keyDown = std::make_unique<QShortcut>(plot.get ());
  keyDown->setKey (Qt::Key_Down);
  connect (keyDown.get(), &QShortcut::activated, this, [=](){
      if(plot->yAxis->range ().lower >= -20){
          double step = plot->yAxis->range ().size ()/3.;
          plot->yAxis->setRange (plot->yAxis->range ().lower - step,
                                 plot->yAxis->range ().upper - step);
          plot->replot ();
        }
    });
  //Left
  keyLeft = std::make_unique<QShortcut>(plot.get ());
  keyLeft->setKey (Qt::Key_Left);
  connect (keyLeft.get(), &QShortcut::activated, this, [=](){
      if(plot->xAxis->range ().lower >0 ){
          double step =plot->xAxis->range ().size ()/3.;
          plot->xAxis->setRange (  plot->xAxis->range ().lower - step,
                                   plot->xAxis->range ().upper - step);
          plot->replot ();
        }
    });

  plot->setInteractions (QCP::iRangeZoom | QCP::iRangeDrag );
  plot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
  plot->axisRect()->setRangeZoom(Qt::Horizontal);
  plot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
  plot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
  plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);
}



void Painter::printArr(std::vector<double> &arr, double fd)
{
  std::array<Qt::GlobalColor, 5> colors{Qt::darkRed, Qt::darkYellow, Qt::darkBlue, Qt::darkGreen, Qt::black};
  int chCnt = 2;
  QVector <QVector <double>> u (chCnt);
  for(auto i = 0; i < static_cast<int>(arr.size ()); ++i){
      u[i%chCnt].append (arr[static_cast<size_t>(i)]);
    }
  QVector <double> t;
  for(int i = 0; i < static_cast<int>(arr.size ())/chCnt; ++i){
      t.append ( i*1000./fd);
    }
  plot->legend->setVisible (true);
  plot->clearGraphs ();
  for(int i = 0; i < chCnt; ++i){
      plot->addGraph ();
      plot->graph (i)->setData (t, u[i]);
      plot->graph (i)->setPen (QColor(colors[static_cast<size_t>(i)]));
      plot->graph (i)->setLineStyle (QCPGraph::lsLine);
      if(i == 0)
        plot->graph (i)->setName ("Канал 1");
      if(i == 1)
        plot->graph (i)->setName ("Канал 2");
    }
  plot->xAxis->setLabel ("Время, мс");
  plot->yAxis->setLabel ("Амплитуда, В");
  plot->xAxis->setRange (0, t[t.size ()-1]);
  plot->yAxis->setRange (-5, 5);
  plot->replot ();

}
