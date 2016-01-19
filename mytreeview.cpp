#include "mytreeview.h"

myTreeView::myTreeView(QWidget *parent) :
    QTreeView(parent)
{
    this->setContextMenuPolicy(Qt::ActionsContextMenu);
    connect(this,SIGNAL(customContextMenuRequested(QPoint &point)), this, SLOT(slotCustomContextMenu(QPoint &point)));
    QAction* lol;
    lol = new QAction("Open",this);
    this->addAction(lol);
    lol->setStatusTip(tr("Open an existing file"));
    connect(lol, SIGNAL(triggered()), this, SLOT(slotOpen()));

}

void myTreeView::slotCustomContextMenu(QPoint &point)
{
     QMenu *menu = new QMenu;
     QModelIndex index = this->currentIndex();

     QString fileName = this->model()->data(this->model()->index(index.row(), 0),0).toString();
     //menu->addAction(QString("Import"), this, SLOT(slotTest()));
     //menu->addAction(QString("Export"), this, SLOT(slotTest()));
     //menu->exec(QCursor::pos());
}
void myTreeView::slotOpen()
{
    QModelIndex index = this->currentIndex();
    QFileSystemModel * l_fileSystem = (QFileSystemModel*) this->model();
    QString fileName = l_fileSystem->filePath(index);
    emit openFile(fileName); //signal
}
void myTreeView::slotResetView()
{
    this->setRootIndex(this->model()->index());
}
