/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mytreeview.h"
#include "settingsdialog.h"
#include "databaseeditor.h"


QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QPlainTextEdit;
class QListWidget;
class QCommandLineParser;
class QFileSystemModel;
class QCalendarWidget;
class QLineEdit;
//class myTreeView;
QT_END_NAMESPACE
struct mElement
{
    QString name;
    int color;
    int lenght;
    int width;
    int thickness;
    QString modelNo;

};

typedef QLinkedList<mElement> mElementList;

struct mOrder
{
    QString refNo;
    mElementList items; //one order is made of multiple items
    QString cp;
    QDate   date;
    QString destFolder;
};

typedef QLinkedList<mOrder> mOrderList;

struct mHashName
{
    QString name;
    QString dir;
};

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void newFile();
    void open();
    void openFromClick(QString &point);
    bool save();
    bool saveAs();
    void about();
    void settings();
    void itemsList();
    void documentWasModified();
    void eventListUpdate();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    void createDockWindows();
    void initHash();
    void searchHash(const QString &stringIn, QString &stringOut,  QString &directoryOut);
    void createFsTree();
    void parseTextFile(QTextStream &inStream, QString &in_path);
    bool maybeSave();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    //int findOrder(const QDate &date, QStringList &l_orderList);
    int findOrdersByDate(const QDate &date, mOrderList &l_orderList);
    void addOrder(mOrder &order);
    int deleteOrder(const QString &refNo);
    int moveDirectory(QString &in_path, QString &out_path);
    int databaseSetup();
    int loadDatabase();
    int saveDatabase();
    //QPlainTextEdit *textEdit;
    QString curFile;
    QCalendarWidget *calendar;
   // QString newFile;

    //QListWidget *customerList;
    //QListWidget *paragraphsList;

    myTreeView *SourceTree;
    myTreeView *DestTree;
    SettingsDialog *settingsDial;
    QFileSystemModel *SrcModel;
    QFileSystemModel *DestModel;
    QCommandLineParser *parser;
    QListWidget *parsedData;
    QListWidget *eventList;
    QDockWidget* dock;
    QDockWidget* eventDock;
    QHash<QString, mHashName>* hash;
    //temporary for debugging//
    //QLinkedList<mOrder> ordersList;
    mOrderList ordersList;
    QFile *dbase_file;
    QLineEdit *srcAddrBar;
    QDialog *parsedDataWindow;

    DatabaseEditor* dbase_edit;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QMenu *viewMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *settingsAct;
    QAction *itemsListAct;
};
//! [0]

#endif
