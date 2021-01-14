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
      QString str = QFileDialog::getExistingDirectory(nullptr,"Папка с файлами","*.baph");
      QDir dir(str);
      QFileInfoList dirContent = dir.entryInfoList(QStringList()
             << "*.baph",
             QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
      if(dirContent.size() >0){
      ui->dockProperty->setVisible(true);
      for(auto dir: dirContent)
          ui->listProperty->addItem(dir.absoluteFilePath());}
  });

//if(ui->dockProperty->isVisible ()){
    connect (ui->listProperty,&QListWidget::clicked , this, [=](){
        qDebug()<<"here";
        double fr = 0;
        std::vector<double> imp = getSignalFromFile (ui->listProperty->currentItem ()->text (), fr);
        plot->printArr (imp, fr);
      });
//  }

}

MainWindow::~MainWindow()
{
  delete ui;
}

std::vector<double> MainWindow::getSignalFromFile(const QString &path, double &fd)
{
  QFile fIn(path);
  QByteArray hd;
  QByteArray fullData;
  int headSize = sizeof(InputFileHead);
  hd.reserve (headSize);
  if(fIn.open(QIODevice::ReadOnly)){
      hd = fIn.read (headSize);
      fullData = fIn.readAll();
      fIn.close ();
    }
  InputFileHead* fHead = reinterpret_cast<InputFileHead*>(&hd.data ()[0]);
   fd = fHead->freqChan;
  std::vector<double> signs;
  QDataStream ds(fullData);
  ds.setByteOrder (ds.LittleEndian);
  double a;
  while (!ds.atEnd ()) {
      ds>>a;
       signs.emplace_back(a);
    }
//  delete fHead;
  return signs;
}

