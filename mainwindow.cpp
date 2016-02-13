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

//! [0]
#include <QtWidgets>

#include "mainwindow.h"

#define DATE_OFFSET 275
#define REF_OFFSET 497
#define SIZE_OFFSET 182
#define SIZE_SPACING 9

static QString mPath = "C:\\Users\\dabi\\Documents\\Mantovana";
static QString errStringHash =  "OGGETTO NON CATEGORIZZATO";
static const QString filePath = "C:\\Users\\dabi\\Documents\\Mantovana\\database.txt";
static QString sourcePath = "C:\\Users\\dabi\\Documents\\Mantovana\\source";
static QString destPath = "C:\\Users\\dabi\\Documents\\Mantovana\\dest";

static bool dbase_status = FALSE;
//! [0]

//! [1]
MainWindow::MainWindow()
//! [1] //! [2]
{
    //textEdit = new QPlainTextEdit;
    //setCentralWidget(textEdit);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    readSettings();
    createDockWindows();
    initHash();

    //connect(textEdit->document(), SIGNAL(contentsChanged()),
            //this, SLOT(documentWasModified()));
    connect(SourceTree, SourceTree->openFile,
            this, openFromClick);
    connect(srcAddrBar, SIGNAL(textChanged(const QString &)), SourceTree , SLOT(slotResetView(const QString &)));

    setCurrentFile("");
    setUnifiedTitleAndToolBarOnMac(true);
    databaseSetup();
}
//! [2]

//! [3]
void MainWindow::closeEvent(QCloseEvent *event)
//! [3] //! [4]
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
    saveDatabase();
    dbase_file->close();

}
//! [4]

//! [5]
void MainWindow::newFile()
//! [5] //! [6]
{
    if (maybeSave()) {
        //textEdit->clear();
        setCurrentFile("");
    }
}
//! [6]

//! [7]
void MainWindow::open()
//! [7] //! [8]
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}
//! [8]
void MainWindow::openFromClick(QString &point)
{
    if (maybeSave()) {
        QString fileName = point;
        if (!fileName.isEmpty())
            loadFile(fileName);
    }

}

//! [9]
bool MainWindow::save()
//! [9] //! [10]
{
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}
//! [10]

//! [11]
bool MainWindow::saveAs()
//! [11] //! [12]
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    QStringList files;
    if (dialog.exec())
        files = dialog.selectedFiles();
    else
        return false;

    return saveFile(files.at(0));
}
//! [12]

//! [13]
void MainWindow::about()
//! [13] //! [14]
{
   QMessageBox::about(this, tr("About Application"),
            tr("The <b>Application</b> example demonstrates how to "
               "write modern GUI applications using Qt, with a menu bar, "
               "toolbars, and a status bar."));
}
//! [14]
//!
void MainWindow::settings()
{
//           QMessageBox::StandardButton ret;
//           ret = QMessageBox::warning(this, tr("Settings"),
//                        tr("Settings for this app goes here Should be QDialog.\n"
//                           "Do you want to save your changes?"),
//                        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
           settingsDial->show();

}

//! [15]
void MainWindow::documentWasModified()
//! [15] //! [16]
{
    //setWindowModified(textEdit->document()->isModified());
}
//! [16]

//! [17]
void MainWindow::createActions()
//! [17] //! [18]
{
    newAct = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

//! [19]
    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
//! [18] //! [19]

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

//! [20]
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
//! [20]
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

//! [21]
    cutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
//! [21]
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    //connect(cutAct, SIGNAL(triggered()), textEdit, SLOT(cut()));

    copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    //connect(copyAct, SIGNAL(triggered()), textEdit, SLOT(copy()));

    pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    //connect(pasteAct, SIGNAL(triggered()), textEdit, SLOT(paste()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

//! [22]
    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
//! [22]
    settingsAct = new QAction(tr("Settings"), this);
    settingsAct->setStatusTip(tr("Main Application Settings"));
    connect(settingsAct, SIGNAL(triggered()), this, SLOT(settings()));

//! [23]
    cutAct->setEnabled(false);
//! [23] //! [24]
    copyAct->setEnabled(false);

    /*QWidgetAction *widgetAddr = new QWidgetAction(this);
    destAddrBar = new QLineEdit(this);
    widgetAddr->setDefaultWidget(destAddrBar);
    menubar()->addAction(widgetAction);*/
    //connect(textEdit, SIGNAL(copyAvailable(bool)),
            //cutAct, SLOT(setEnabled(bool)));
    //connect(textEdit, SIGNAL(copyAvailable(bool)),
            //copyAct, SLOT(setEnabled(bool)));
}
//! [24]

//! [25] //! [26]
void MainWindow::createMenus()
//! [25] //! [27]
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
//! [28]
    fileMenu->addAction(openAct);
//! [28]
    fileMenu->addAction(saveAct);
//! [26]
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addAction(settingsAct);

    viewMenu = menuBar()->addMenu(tr("&View"));

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}
//! [27]

//! [29] //! [30]
void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
//! [29] //! [31]
    fileToolBar->addAction(openAct);
//! [31]
    fileToolBar->addAction(saveAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);
    editToolBar->addSeparator();
    QWidgetAction *widgetAddr = new QWidgetAction(this);
    srcAddrBar = new QLineEdit(this);
    srcAddrBar->setMaximumWidth(600);
    srcAddrBar->setText(sourcePath);
    widgetAddr->setDefaultWidget(srcAddrBar);
    editToolBar->addAction(widgetAddr);

    settingsDial = new SettingsDialog;
    settingsDial->setPath(&destPath);
}
//! [30]

//! [32]
void MainWindow::createStatusBar()
//! [32] //! [33]
{
    statusBar()->showMessage(tr("Ready"));
}
//! [33]

//! [34] //! [35]
void MainWindow::readSettings()
//! [34] //! [36]
{
    QSettings settings("QtProject", "Streamliner");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);
}
//! [35] //! [36]

//! [37] //! [38]
void MainWindow::writeSettings()
//! [37] //! [39]
{
    QSettings settings("QtProject", "Streamliner");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}
//! [38] //! [39]

//! [40]
bool MainWindow::maybeSave()
//! [40] //! [41]
{
//    if (textEdit->document()->isModified()) {
//        QMessageBox::StandardButton ret;
//        ret = QMessageBox::warning(this, tr("Application"),
//                     tr("The document has been modified.\n"
//                        "Do you want to save your changes?"),
//                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
//        if (ret == QMessageBox::Save)
//            return save();
//        else if (ret == QMessageBox::Cancel)
//            return false;
//    }
    return true;
}
//! [41]

//! [42]
void MainWindow::loadFile(const QString &fileName)
//! [42] //! [43]
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }
    QFileInfo l_fileInfo = QFileInfo(file);

    QTextStream in(&file);
    QString path_ofDir = l_fileInfo.path();
    parseTextFile(in, path_ofDir);
    in.seek(0);//reset position
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    //textEdit->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}
//! [43]
void MainWindow::parseTextFile(QTextStream &inStream, QString &in_path)
{
    parsedData = new QListWidget(parsedDataWindow);
    QString  in_line, model_str, parsed_str, out_hash, out_date, l_refNo, out_dir, l_path, out_path, size_str;
    QDate l_date;
    QStringList parsed_models;
    mOrder* l_order = NULL;
    while(!inStream.atEnd()){ 
        mElement l_item;
        in_line = inStream.readLine();
        model_str = in_line.split(" ", QString::SkipEmptyParts).at(1);//from each string take the second word/
        out_date = in_line.mid(DATE_OFFSET, 8); //take date
        l_date = QDate::fromString(out_date, "yyyyMMdd");
        parsed_str = model_str.right(11);
        l_refNo = in_line.mid(REF_OFFSET, 14); //CP
        if((!l_order)||(l_order->refNo != l_refNo)) //different order create new
        {
            if(l_order) {
                out_path.append(l_path+"\\"+out_dir+"\\"+l_order->date.toString("dd_MM_yyyy"));
                l_order->destFolder.append(out_path);
                addOrder(*l_order); //store the previous order
            }
            l_order = new mOrder; //create dynamically
            l_order->refNo = l_refNo;
            l_order->date = l_date;
            l_path.clear();
            settingsDial->getPath(l_path);
            out_path.clear();
        }
        searchHash(parsed_str, out_hash, out_dir);
        l_item.name.append(parsed_str + " - " + out_hash);
        l_item.modelNo = model_str;
        l_item.lenght = in_line.mid(SIZE_OFFSET, 9).toInt();
        l_item.width = in_line.mid(SIZE_OFFSET+ (2*SIZE_SPACING), 9).toInt();
        l_item.thickness = in_line.mid(SIZE_OFFSET+ SIZE_SPACING , 9).toInt();
        QString l_dimensions;
        l_dimensions.sprintf( " - L: %d, W: %d, T: %d",l_item.lenght, l_item.width, l_item.thickness);
        l_item.name.append(l_dimensions);
        //QMessageBox::warning(this, tr("Output Path"), l_order.destFolder);
        parsed_models << model_str.right(11) + " - " + out_hash + "  consegna: "+ l_date.toString("ddd MMMM d yy") + " " + l_refNo + "\n";
        l_order->items.append(l_item); //add Item to the
    }
    if(!l_order->refNo.isEmpty()){
        out_path.append(l_path+"\\"+out_dir+"\\"+l_order->date.toString("dd_MM_yyyy"));
        l_order->destFolder.append(out_path);
        addOrder(*l_order); //last order must be added
    }

    if(model_str.isEmpty())  QMessageBox::warning(this, tr("Error"),
                                                  tr("Application Cannot find refNo"));

    moveDirectory(in_path, out_path); //For now we assume that there is going to be one out date/path for each file in.

    parsedData->addItems(parsed_models);
    parsedData->setMinimumWidth(600);
    //parsedData->setMaximumWidth(600);
    parsedDataWindow->show();
    //dock->setWidget(parsedData);
}

//! [44]
bool MainWindow::saveFile(const QString &fileName)
//! [44] //! [45]
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    //out << textEdit->toPlainText();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}
//! [45]

//! [46]
void MainWindow::setCurrentFile(const QString &fileName)
//! [46] //! [47]
{
    curFile = fileName;
    //textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    setWindowFilePath(shownName);
}
//! [47]

//! [48]
QString MainWindow::strippedName(const QString &fullFileName)
//! [48] //! [49]
{
    return QFileInfo(fullFileName).fileName();
}
//! [49]
void MainWindow::createDockWindows()
{

     //dock = new QDockWidget(tr("Parsed Data"), this);//memorize this dock for future update
    //addDockWidget(Qt::TopDockWidgetArea, dock); //do not add to the dock
    //viewMenu->addAction(dock->toggleViewAction());

    parsedDataWindow = new QDialog(this);
    //viewMenu->addAction(parsedDataWindow->);

    createFsTree();
    QDockWidget *dock_destT = new QDockWidget(tr("Organizer"), this);
    //dock_t->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::TopDockWidgetArea, dock_destT);
    dock_destT->setWidget(DestTree);
    dock_destT->setMaximumWidth(600);
    viewMenu->addAction(dock_destT->toggleViewAction());

    QDockWidget *dock_t = new QDockWidget(tr("Files"), this);
    //dock_t->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);


    addDockWidget(Qt::TopDockWidgetArea, dock_t);
    dock_t->setWidget(SourceTree);
    dock_t->setMaximumWidth(600);
    viewMenu->addAction(dock_t->toggleViewAction());

    //calendar events
    eventDock = new QDockWidget(tr("Consegne Selezionate"), this);//memorize this dock for future update
    addDockWidget(Qt::BottomDockWidgetArea, eventDock);
    viewMenu->addAction(eventDock->toggleViewAction());
    //TEMPORARY???
    eventList = new QListWidget(eventDock);
    /*eventList->addItems(QStringList()
            << "21VPPA45B00 - PENSILE JOLLY DA 13-45 H.36  TONY"
            << "22VBSJ30C00 - INSERTO \"C\"X JOLLY EVO L.30 MAYA DESERT OPACO - ARCH. PARENTI"
            << "22WCTL90002 - EL.GIO.PEN.\"TETRIS\"L90 P33.4 H36 3VANI ALEVE'OL.ME - FRANCESCO");*/
    eventDock->setWidget(eventList);

    //calendar widget
    QDockWidget *dock_c = new QDockWidget(tr("Calendar"), this);//memorize this dock for future update
    calendar = new QCalendarWidget();
    addDockWidget(Qt::BottomDockWidgetArea, dock_c);
    dock_c->setWidget(calendar);
    calendar->setMaximumWidth(600);
    calendar->setGridVisible(true);
    viewMenu->addAction(dock_c->toggleViewAction());
    connect(calendar, SIGNAL(selectionChanged()), this, SLOT(eventListUpdate()));

    connect(settingsDial, SIGNAL(saveSettings(const QString &)), DestTree, SLOT(slotResetView(const QString &)));


//    connect(customerList, SIGNAL(currentTextChanged(QString)),
//            this, SLOT(insertCustomer(QString)));
//    connect(paragraphsList, SIGNAL(currentTextChanged(QString)),
//            this, SLOT(addParagraph(QString)));
}
void MainWindow::eventListUpdate()
{
    QDate l_date = calendar->selectedDate();
    mOrderList l_orderList;
    findOrdersByDate(l_date, l_orderList);
    QStringList l_listShown;
    mOrderList::iterator i;
    for(i = l_orderList.begin(); i != l_orderList.end(); i++)
    {
        mElementList::iterator j;
        for(j = i->items.begin(); j != i->items.end(); j++)
        {
            l_listShown.append(j->name);
        }
    }
    eventList->clear();
    eventList->addItems(l_listShown);
    eventDock->setWidget(eventList);
}
void MainWindow::createFsTree()
{


    SrcModel = new QFileSystemModel;

    SrcModel->setRootPath(mPath);
    SourceTree = new myTreeView;
    SourceTree->setModel(SrcModel);
    SourceTree->setRootIndex(SrcModel->index(mPath));

    // Demonstrating look and feel features
    SourceTree->setAnimated(false);
    SourceTree->setIndentation(20);
    SourceTree->setSortingEnabled(true);


    SourceTree->setWindowTitle(QObject::tr("Dir View"));

    DestModel = new QFileSystemModel;

    DestModel->setRootPath(destPath);
    DestTree = new myTreeView;
    DestTree->setModel(DestModel);
    DestTree->setRootIndex(DestModel->index(mPath));

    // Demonstrating look and feel features
    DestTree->setAnimated(false);
    DestTree->setIndentation(20);
    DestTree->setSortingEnabled(true);


    DestTree->setWindowTitle(QObject::tr("Organizer"));

}

void MainWindow::initHash()
{
    hash = new QHash<QString, mHashName>;
    hash->insert("21",(const mHashName){"PENSILE", "librerie"});
    hash->insert("22V",(const mHashName){"INSERTO ", "librerie"});
    hash->insert("2V",(const mHashName){"INSERTO ", "librerie"});
    hash->insert("2W",(const mHashName){"INSERTO ", "librerie"});
    hash->insert("22W",(const mHashName){"EL.GIOR.PENS.\"TETRIS\"", "librerie"});
    hash->insert("22",(const mHashName){"LIBRERIA", "librerie"});
    hash->insert("24",(const mHashName){"LIBRERIA", "librerie"});
    hash->insert("29",(const mHashName){"LIBRERIA", "librerie"});
    hash->insert("22..",(const mHashName){"PIANO","piani"});
    hash->insert("5200",(const mHashName){"FASCIA", "piani"});
    hash->insert("52VG",(const mHashName){"TAVOLO+PROLUNGA", "piani"});
    hash->insert("2MA",(const mHashName){"MENSOLA", "mensole"});
    hash->insert("2WL",(const mHashName){"MENSOLA", "mensole"});

}
void MainWindow::searchHash(const QString &stringIn, QString &stringOut, QString &directoryOut)
{
        int retries = 3;
        int characters = 4;
        QString key = stringIn.left(characters);
        while(retries)
        {
            stringOut = (hash->value(key)).name;
            directoryOut = (hash->value(key)).dir;
            if(stringOut.isEmpty())
            {
                characters--;
                retries--;
                key = key.left(characters);
            }else break; //found something

        }
        if(stringOut.isEmpty())
            stringOut.append(errStringHash);
}

int MainWindow::findOrdersByDate(const QDate &date, mOrderList &l_orderList)
{
    int orders_found = 0;
    if(date.isValid())
    {
       //QLinkedList<mOrder>::iterator i;
       mOrderList::iterator i;
       for(i = ordersList.begin(); i != ordersList.end(); i++)
       {
           if(date == i->date)
           {
               l_orderList.append(*i);
               orders_found++;
           }
       }
       if(!orders_found)
       {
           QMessageBox::warning(this, tr("Error"), tr("No Orders found"));
       } return SUCCESS_APP; //found some orders return
    }else QMessageBox::warning(this, tr("Error"), tr("Date Not Valid"));
    return ERROR_APP;
}
void MainWindow::addOrder(mOrder &order)
{
    int order_exists = 0;
    //mOrder l_order;
    int order_inserted = 0;
    if(!order.refNo.isEmpty())
    {
       //QLinkedList<mOrder>::iterator i;
       mOrderList::iterator i;
       for(i = ordersList.begin(); i != ordersList.end(); i++)
       {
           if(order.refNo == i->refNo)
           {
               order_exists = 1;
              // i->name << (*order.name.begin()); //add to pre-existing order
               mElementList::iterator j;
               for(j = order.items.begin(); j != order.items.end(); j++)
               {
                   i->items.append(*j); //append new items
               }
               break;
           }
       }
       if(!order_exists)
       {
           for(i = ordersList.begin(); i != ordersList.end(); i++)
           {
               if(order.date > i->date) //order by date
               {
                   ordersList.insert(i, order);
                   order_inserted = 1;
                   break;
               }
           }
           if (!order_inserted) ordersList.append(order); //add at the end
       }
       else
       {
           //order exists already add items to mOrder
           QMessageBox::warning(this, tr("Error"), tr("Order refNo exists Item Appended"));
       }
    }else QMessageBox::warning(this, tr("Error"), tr("Order refNo Empty"));
}
int MainWindow::deleteOrder(const QString &refNo)
{
    int order_deleted = 0;
    if(!refNo.isEmpty())
    {
       //QLinkedList<mOrder>::iterator i;
       mOrderList::iterator i;
       for(i = ordersList.begin(); i != ordersList.end(); i++)
       {
           if(refNo == i->refNo)
           {
               ordersList.erase(i);
               order_deleted = 1;
               break;
           }
       }
       if(!order_deleted)
       {
           QMessageBox::warning(this, tr("Error"), tr("Cannot find Order refNo"));
           return false;
       }
    }else QMessageBox::warning(this, tr("Error"), tr("Order refNo Empty"));
    return true;
}

int MainWindow::databaseSetup()
{
    dbase_file = new QFile(filePath);
    if (!dbase_file->open(QIODevice::ReadWrite))
    {
        QMessageBox::warning(this, tr("Error"), tr("Cannot Open File"));
        statusBar()->showMessage(tr("Database Disconnected"), 3000);
        return ERROR_APP;
    }
    dbase_status = TRUE;
    loadDatabase();
    statusBar()->showMessage(tr("Database Connected"), 3000);
    return SUCCESS_APP;
}
int MainWindow::loadDatabase()
{
    if(dbase_file){
        mOrder in_item;
        QString l_date;
        QStringList l_FullName;
        mElementList l_elements;
        QTextStream in(dbase_file);
        QString id;
        while(!in.atEnd())
        {
            char x;
            QString *out_item;
            in >> x;
            if (x == '_')
            {
                in >> x;
                switch(x){
                    case 'O':
                        if(!in_item.refNo.isEmpty()){  //if this is not the first in file
                            if(!l_date.isEmpty())
                            {
                                in_item.date = QDate::fromString(l_date, "yyyyMMdd");
                                l_date.clear();
                            }
                            ordersList.append(in_item); //add previous order
                           // memset(&in_item, 0 , sizeof(in_item));
                            in_item.cp.clear(); //reset mOrder
                            in_item.refNo.clear();
                            in_item.destFolder.clear();
                            in_item.items.clear();
                        }
                        break;
                    case 'D':
                        out_item = &l_date;
                        break;
                    case 'R':
                        out_item = &in_item.refNo;
                        break;
                    case 'F':
                        out_item = &in_item.destFolder;
                        break;
                    case 'C':
                        out_item = &in_item.cp;
                        break;
                    case 'I':
                        out_item = NULL; //should be reset
                        mElement l_element;
                        while(!in.atEnd())
                        {
                            in >> x;
                            if (x != '_') l_element.name.append(x); //parse name
                            else break;
                        }
                        while(!in.atEnd())
                        {
                            in >> x;
                            if (x != '_') l_element.modelNo.append(x); //parse name
                            else break;
                        }
                        in >> l_element.lenght;
                        in >> l_element.width;
                        in >> l_element.thickness;
                        in_item.items.append(l_element);
                        break;
                }
            } else if(out_item)
            {
                out_item->append(x);   //output the char read
            }else if(x == ' ') //out_item not set_space between fields
            {
                //just go further
            }
            else QMessageBox::warning(this, tr("Error"), tr("Parsing Database Issue: Field not recognized"));
        }
        if(in_item.items.count()){
            if(!l_date.isEmpty())
            {
                in_item.date = QDate::fromString(l_date, "yyyyMMdd");
                l_date.clear();
            }
            ordersList.append(in_item); //add last order
        }
        QMessageBox::warning(this, tr("Success"), tr("Database Loaded!"));
        return SUCCESS_APP;

    } QMessageBox::warning(this, tr("Warning"), tr("No database found, Opening New"));
     return SUCCESS_APP;
}
int MainWindow::saveDatabase()
{
    if(dbase_file->isWritable())
    {
        dbase_file->seek(0);//reset Save always full list
        //QLinkedList<mOrder>::iterator i;
        mOrderList::iterator i;
        QTextStream out(dbase_file);
        for(i = ordersList.begin(); i != ordersList.end(); i++)
        {
             out << "_O";
             out << "_D";
             out << i->date.toString("yyyyMMdd");
             out << "_R";
             out << i->refNo;
              out << "_F";
             out << i->destFolder;
             out << "_C";
             out << i->cp;
             mElementList::iterator j;
             for(j = i->items.begin(); j!= i->items.end(); j++)
             {
                out << "_I";
                out << j->name;
                out << "_";
                out << j->modelNo;
                out << "_";
                out << (uint32_t) j->lenght;
                out << " ";
                out << (uint32_t) j->width;
                out << " ";
                out << (uint32_t) j->thickness;
                out << " ";
             }
        }
        return SUCCESS_APP;
    }else QMessageBox::warning(this, tr("Error"), tr("Cannot Save Database"));
     return ERROR_APP;
}
static bool copyRecursively(const QString &srcDirPath,
                            const QString &tgtDirPath)
{
    QFileInfo srcFileInfo(srcDirPath);
    if (srcFileInfo.isDir()) {
        QDir targetDir(tgtDirPath);
        targetDir.cdUp();
        targetDir.mkdir(QFileInfo(tgtDirPath).fileName());
            //return false; //don't return false just doesn' create the directory
        QDir sourceDir(srcDirPath);
        QStringList fileNames = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
        foreach (const QString &fileName, fileNames) {
            const QString newSrcDirPath
                    = srcDirPath + QLatin1Char('/') + fileName;
            const QString newTgtDirPath
                    = tgtDirPath + QLatin1Char('/') + fileName;
            if (!copyRecursively(newSrcDirPath, newTgtDirPath))
                return false;
        }
    } else {
        if (!QFile::copy(srcDirPath, tgtDirPath))
            return false;
    }
    return true;
}

int MainWindow::moveDirectory(QString &in_path, QString &out_path)
{
    QDir src_dir(in_path);
    if (!src_dir.exists())
    {
        QMessageBox::warning(this, tr("Warning"), tr("Source Directory not found!"));
        return ERROR_APP;
    }

    QDir dst_dir(out_path);
    if (!dst_dir.exists())
    {
        QMessageBox::warning(this, tr("Warning"), tr("Destination Directory not found, creating new"));
        if(!dst_dir.mkpath(out_path))
        {
            QMessageBox::warning(this, tr("Error"), tr("Cannot create destination directory"));
            return ERROR_APP;
        }
    }
    //at this point we are sure that source and destination paths exist.
    if(!copyRecursively(in_path, out_path))
    {
        QMessageBox::warning(this, tr("Error"), tr("Not all content was copied"));
    }
    return SUCCESS_APP;

}
