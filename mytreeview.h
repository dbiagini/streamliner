#ifndef MYTREEVIEW
#define MYTREEVIEW

#include <QtGui>
#include <QtCore>
#include <QTreeView>
#include <QAction>
#include <QMenu>
#include <QFileSystemModel>

QT_BEGIN_NAMESPACE
//class QAction;
//class QMenu;
//class QTreeView;
//class QFileSystemModel;
QT_END_NAMESPACE

//class QTreeView;

class myTreeView : public QTreeView
{
   Q_OBJECT
   public:
        explicit myTreeView(QWidget *parent = 0);
        //~myTreeView();
   private slots:
       void slotCustomContextMenu(QPoint &point);
       void slotOpen();
   signals:
        void openFile(QString &point);

};
#endif // MYTREEVIEW

