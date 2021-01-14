#include "worker.h"

Worker::Worker(QObject *parent) : QObject(parent){}

Worker::Worker(int metNum, size_t koef , const QString& dir ): methodNum(metNum), dirPath (dir){
  isStopped = false;
  kNf = static_cast<size_t>(pow(2, koef));
  qRegisterMetaType<complexArray>("complexArray");
  qRegisterMetaType<std::map<double,std::vector<double>>>("std::map<double,std::vector<double>>");
  qRegisterMetaType<size_t>("size_t");
}

void Worker::stop()
{
  isStopped = true;
}

void Worker::startWorker()
{
  std::vector<QString> paths = getPaths (dirPath);
  emit sendFileCnt (paths.size ());
  myFFT worker;
  std::vector<double> amps;
  complexArray phases;
  std::map<double, std::vector<double>> listFA;
  double freq = 0;
  double fd =0;
  size_t fNum =1;
  std::vector<complexArray> signs ;
  complexArray arrForPrint;
  for(const auto& path: paths){
      if(!isStopped){
          emit sendCurrentFileNum (fNum);
          amps.clear ();
          signs = getSignalFromFile (path, freq,fd);

          for(size_t i =0; i < signs.size (); ++i){
              phases.emplace_back(worker.processPhase (signs[i], freq, fd, kNf, arrForPrint));
              emit sendData (arrForPrint, freq);
              if(methodNum == 0){
                  amps.emplace_back(worker.processSpectrum (signs[i], freq, fd, kNf, i ,arrForPrint));
                  emit sendData (arrForPrint, freq);}
              if(methodNum ==1)
                amps.emplace_back(worker.processHilbert (signs[i]));
            }

          double ampl =amps[1]/amps[0];
          std::complex<double> phDiv =phases[1]/phases[0];
          double phase = arg (phDiv);
          emit sendInfo (freq, ampl, phase);
          listFA[freq].emplace_back(ampl);
          listFA[freq].emplace_back(phase);
          fNum++;
        }else{
          emit finished ();
        }
    }
  emit sendTotal (listFA);
  emit finished ();
  writeFile (dirPath, listFA);
}

std::vector<QString> Worker::getPaths(const QString &path)
{
  QDir dir(path);
  std::vector<QString> paths;
  if(dir.exists ()){
      dir.setNameFilters (QStringList()<<"*.baph");
      dir.setSorting (QDir::Name);
      QFileInfoList fList = dir.entryInfoList ();
      for(int i = 0; i < fList.size (); ++i){
          paths.emplace_back(fList.at (i).filePath ());
        }
    }
  return  paths;
}

std::vector<complexArray> Worker::getSignalFromFile(const QString &path, double &freq, double &fd)
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
  freq = fHead->freqSign;
  fd = fHead->freqChan;
  std::vector<complexArray> signs(2);

  QDataStream ds(fullData);
  ds.setByteOrder (ds.LittleEndian);
  double a;
  int cntr = 0;
  while (!ds.atEnd ()) {
      ds>>a;
       signs[cntr%2].emplace_back(a,0);
     ++cntr;
    }

  return signs;
}

void Worker::writeFile(QString dir, const std::map<double, std::vector<double> > &data)
{
  QFile fOut (dir+"/res.csv");
  if(fOut.open (QIODevice::WriteOnly | QIODevice::Text)){
      QTextStream ts (&fOut);
      for(const auto& val: data)
        ts<<QString("%L1").arg(val.first)<<";" <<QString("%L1").arg(val.second[0])<<";" <<QString("%L1").arg(val.second[1])<<"\n";
      fOut.close ();
    }
}

void Worker::printLog(QString msg, QString log)
{
   QFile fOut("d:/log.txt");
   if(fOut.open (QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)){
       QTextStream ts (&fOut);
       ts<<msg<<log<<"\n";
       fOut.close ();
     }
}



