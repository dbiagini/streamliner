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

static QString mPath = "C:\\Users\\dabi\\Documents\\Mantovana";
static QString errStringHash =  "OGGETTO NON CATEGORIZZATO";
//! [0]

//! [1]
MainWindow::MainWindow()
//! [1] //! [2]
{
    textEdit = new QPlainTextEdit;
    setCentralWidget(textEdit);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    readSettings();
    createDockWindows();
    initHash();

    connect(textEdit->document(), SIGNAL(contentsChanged()),
            this, SLOT(documentWasModified()));
    connect(tree, tree->openFile,
            this, openFromClick);

    setCurrentFile("");
    setUnifiedTitleAndToolBarOnMac(true);
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
}
//! [4]

//! [5]
void MainWindow::newFile()
//! [5] //! [6]
{
    if (maybeSave()) {
        textEdit->clear();
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

//! [15]
void MainWindow::documentWasModified()
//! [15] //! [16]
{
    setWindowModified(textEdit->document()->isModified());
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
    connect(cutAct, SIGNAL(triggered()), textEdit, SLOT(cut()));

    copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, SIGNAL(triggered()), textEdit, SLOT(copy()));

    pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, SIGNAL(triggered()), textEdit, SLOT(paste()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

//! [22]
    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
//! [22]

//! [23]
    cutAct->setEnabled(false);
//! [23] //! [24]
    copyAct->setEnabled(false);
    connect(textEdit, SIGNAL(copyAvailable(bool)),
            cutAct, SLOT(setEnabled(bool)));
    connect(textEdit, SIGNAL(copyAvailable(bool)),
            copyAct, SLOT(setEnabled(bool)));
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
    QSettings settings("QtProject", "Application Example");
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
    QSettings settings("QtProject", "Application Example");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}
//! [38] //! [39]

//! [40]
bool MainWindow::maybeSave()
//! [40] //! [41]
{
    if (textEdit->document()->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"),
                     tr("The document has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
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

    QTextStream in(&file);
    parseTextFile(in);
    in.seek(0);//reset position
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    textEdit->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}
//! [43]
void MainWindow::parseTextFile(QTextStream &inStream)
{
    parsedData = new QListWidget(dock);
    QString  in_line, model_str, parsed_str, out_hash, out_date;
    QDate l_date;
    QStringList parsed_models;
    while(!inStream.atEnd()){
        in_line = inStream.readLine();
        model_str = in_line.split(" ", QString::SkipEmptyParts).at(1);//from each string take the second word/
        out_date = in_line.mid(275, 8); //take date
        l_date = QDate::fromString(out_date, "yyyyMMdd");
        parsed_str = model_str.right(11);
        searchHash(parsed_str, out_hash);
        parsed_models << model_str.right(11) + " - " + out_hash + "\n";
    }
    if(model_str.isEmpty())  QMessageBox::warning(this, tr("Error"),
                                                  tr("Application Cannot find refNo"));

    parsedData->addItems(parsed_models);
    dock->setWidget(parsedData);
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
    out << textEdit->toPlainText();
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
    textEdit->document()->setModified(false);
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
    QDockWidget *dock_t = new QDockWidget(tr("Files"), this);
    dock_t->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    createFsTree();
    dock_t->setWidget(tree);
    addDockWidget(Qt::RightDockWidgetArea, dock_t);
    viewMenu->addAction(dock_t->toggleViewAction());

    dock = new QDockWidget(tr("Parsed Data"), this);//memorize this dock for future update
    //parsedData = new QListWidget(dock);
    /*parsedData->addItems(QStringList()
            << "Thank you for your payment which we have received today."
            << "Your order has been dispatched and should be with you "
               "within 28 days."
            << "You made an overpayment (more than $5). Do you wish to "
               "buy more items, or should we return the excess to you?");*/
    //dock->setWidget(parsedData);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());

    //calendar widget
    QDockWidget *dock_c = new QDockWidget(tr("Calendar"), this);//memorize this dock for future update
    calendar = new QCalendarWidget();
    addDockWidget(Qt::BottomDockWidgetArea, dock_c);
    dock_c->setWidget(calendar);
    calendar->setMaximumWidth(600);
    calendar->setGridVisible(true);
    viewMenu->addAction(dock_c->toggleViewAction());


//    connect(customerList, SIGNAL(currentTextChanged(QString)),
//            this, SLOT(insertCustomer(QString)));
//    connect(paragraphsList, SIGNAL(currentTextChanged(QString)),
//            this, SLOT(addParagraph(QString)));
}
void MainWindow::createFsTree()
{
//    parser = new QCommandLineParser;
//    parser->setApplicationDescription("Qt Dir View Example");
//    parser->addHelpOption();
//    parser->addVersionOption();
//    parser->addPositionalArgument("directory", "The directory to start in.");
//    parser->process(QApplication::arguments());
//    const QString rootPath = parser->positionalArguments().isEmpty()
//        ? QString() : parser->positionalArguments().first();

    model = new QFileSystemModel;
    model->setRootPath(mPath);
    tree = new myTreeView;
    tree->setModel(model);
//    if (!rootPath.isEmpty()) {
//        const QModelIndex rootIndex = model->index(QDir::cleanPath(rootPath));
//        if (rootIndex.isValid())
//            tree->setRootIndex(rootIndex);
//    }

    // Demonstrating look and feel features
    tree->setAnimated(false);
    tree->setIndentation(20);
    tree->setSortingEnabled(true);
//    const QSize availableSize = QApplication::desktop()->availableGeometry(tree).size();
//    tree->resize(availableSize / 2);
//    tree->setColumnWidth(0, tree->width() / 3);

    tree->setWindowTitle(QObject::tr("Dir View"));
//    tree->setContextMenuPolicy(Qt::ActionsContextMenu);
//    connect(tree, SIGNAL(customContextMenuRequested(const QPoint&)),
//        this, SLOT(ShowContextMenu(const QPoint&)));
}

void MainWindow::initHash()
{
    hash = new QHash<QString, QString>;
    hash->insert("21","PENSILE");
    hash->insert("22V","INSERTO ");
    hash->insert("2V","INSERTO ");
    hash->insert("2W","INSERTO ");
    hash->insert("22W","EL.GIOR.PENS.\"TETRIS\"");
    hash->insert("22","LIBRERIA");
    hash->insert("24","LIBRERIA");
    hash->insert("29","LIBRERIA");
    hash->insert("22..","PIANO");
    hash->insert("5200","FASCIA");
    hash->insert("52VG","TAVOLO+PROLUNGA");
    hash->insert("2MA","MENSOLA");
    hash->insert("2WL","MENSOLA");

}
void MainWindow::searchHash(const QString &stringIn, QString &stringOut)
{
        int retries = 3;
        int characters = 4;
        QString key = stringIn.left(characters);
        while(retries)
        {
            stringOut = hash->value(key);
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
