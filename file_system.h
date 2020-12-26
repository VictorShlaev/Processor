#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include <QObject>
#include <QFileSystemModel>
#include <QTreeView>
#include <memory>

class FileSystem : public QTreeView
{
  Q_OBJECT
public:
  FileSystem(QTreeView* _tw = nullptr);
private:
  std::unique_ptr<QFileSystemModel> model;
  std::unique_ptr<QTreeView> tw;
signals:
  void activated(const QString& path);

};

#endif // FILESYSTEM_H
