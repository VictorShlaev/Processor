#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <Vfft.h>

class Worker : public QObject
{
  Q_OBJECT
public:
  explicit Worker(QObject *parent = nullptr);
  explicit Worker (int metNum = 0, size_t koef = 0, const QString& dir = "");
  void stop();
public  slots:
  void startWorker();
private:
 int methodNum;
 size_t kNf;
 QString dirPath;
 bool isStopped;
 //funs

 std::vector<QString> getPaths(const QString& path);
 std::vector<complexArray> getSignalFromFile(const QString &path, double &freq, double &fd);
 void writeFile(QString dir, const std::map<double, std::vector<double> > &data);
 void printLog (QString msg, QString log);

signals:
  void sendFileCnt(size_t cnt);
  void sendCurrentFileNum(size_t num);
  void sendInfo(double freq, double ampl, double phase);
  void sendData(complexArray arr, double freq);
  void sendTotal(std::map<double, std::vector<double>> res);
  void finished();
};

#endif // WORKER_H
