#include "file_system.h"
#include <QDebug>

FileSystem::FileSystem(QTreeView *_tw): tw(_tw)
{
  model = std::make_unique< QFileSystemModel>(this);
  model->setRootPath (QDir::rootPath ());
  model->setNameFilterDisables(false);
  tw->setModel (model.get());
  connect (tw.get (), &QTreeView::activated, this, [=](const QModelIndex& ind){
      emit activated (model->filePath (ind));
    });

}

